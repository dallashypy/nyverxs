// ---------------------------------------------------------------------------
// NYVERXS remote client - see RemoteClient.h for the contract.
//
// Ownership model: one serial queue owns authentication, the WebSocket and all
// retries. UI-facing properties are atomic. Commands never touch game memory
// here: they are forwarded to nyv_remote_apply/nyv_remote_action, which live in
// lostwq.mm and know the tweak globals.
// ---------------------------------------------------------------------------

#import "RemoteClient.h"
#import <UIKit/UIKit.h>
#import <CoreFoundation/CoreFoundation.h>

#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>

// Implemented by lostwq.mm.
extern "C" {
void nyv_remote_apply(const char *param, const char *value);
void nyv_remote_action(const char *action, const char *param, const char *value,
                       char *reply, size_t cap);
void nyv_remote_status_json(char *out, size_t cap);
}

static NSString *const kNYVDiscordKey = @"nyv_remote_discord";
static NSString *const kNYVServerKey  = @"nyv_remote_server";
static NSString *const kNYVSidKey     = @"nyv_remote_device_sid";
static NSString *const kNYVDefaultServer = @"https://nologsontop.squareweb.app";

static NSString *NYVTrim(NSString *value) {
    if (![value isKindOfClass:NSString.class]) return @"";
    return [value stringByTrimmingCharactersInSet:
            [NSCharacterSet whitespaceAndNewlineCharacterSet]];
}

static NSString *NYVNormalizeServer(NSString *raw) {
    NSString *server = NYVTrim(raw);
    if (server.length == 0) server = kNYVDefaultServer;
    if ([server rangeOfString:@"://"].location == NSNotFound)
        server = [@"https://" stringByAppendingString:server];
    while (server.length > 1 && [server hasSuffix:@"/"])
        server = [server substringToIndex:server.length - 1];
    return server;
}

static BOOL NYVValidDiscord(NSString *value) {
    if (value.length < 17 || value.length > 20) return NO;
    for (NSUInteger i = 0; i < value.length; i++) {
        unichar c = [value characterAtIndex:i];
        if (c < '0' || c > '9') return NO;
    }
    return YES;
}

static NSString *NYVSID(void) {
    NSUserDefaults *defaults = NSUserDefaults.standardUserDefaults;
    NSString *sid = [defaults stringForKey:kNYVSidKey];
    if (![sid isKindOfClass:NSString.class] || sid.length < 8) {
        sid = [@"ios-" stringByAppendingString:NSUUID.UUID.UUIDString];
        [defaults setObject:sid forKey:kNYVSidKey];
    }
    return sid;
}

static NSString *NYVWireValue(id value) {
    if ([value isKindOfClass:NSString.class]) return value;
    if ([value isKindOfClass:NSNumber.class]) {
        NSNumber *number = (NSNumber *)value;
        if (CFGetTypeID((__bridge CFTypeRef)number) == CFBooleanGetTypeID())
            return number.boolValue ? @"true" : @"false";
        return number.stringValue;
    }
    return [value description] ?: @"";
}

static NSString *NYVString(NSDictionary *json, NSString *key) {
    id value = json[key];
    if ([value isKindOfClass:NSString.class]) return value;
    if ([value isKindOfClass:NSNumber.class]) return [value stringValue];
    return nil;
}

static NSDictionary *NYVParseJSON(NSData *data) {
    if (data.length == 0) return nil;
    id parsed = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
    return [parsed isKindOfClass:NSDictionary.class] ? parsed : nil;
}

static NSData *NYVEncodeJSON(id object) {
    return [NSJSONSerialization dataWithJSONObject:object options:0 error:nil];
}

@interface NYVRemoteClient () <NSURLSessionWebSocketDelegate>
@property (atomic, copy, readwrite) NSString *discordID;
@property (atomic, copy, readwrite) NSString *serverURL;
@property (atomic, copy, readwrite) NSString *lastStatus;
@property (atomic, copy, readwrite) NSString *lastError;
@property (atomic, readwrite) BOOL connected;
@property (atomic, readwrite) BOOL running;
@end

@implementation NYVRemoteClient {
    dispatch_queue_t _queue;
    NSURLSession *_session;
    NSURLSessionWebSocketTask *_task;
    dispatch_source_t _keepAlive;

    NSString *_token;
    NSString *_userDataJSON;

    NSUInteger _generation;
    NSInteger _retry;
    BOOL _retryScheduled;
    BOOL _authInFlight;
}

+ (instancetype)shared {
    static NYVRemoteClient *client;
    static dispatch_once_t once;
    dispatch_once(&once, ^{ client = [[NYVRemoteClient alloc] init]; });
    return client;
}

- (instancetype)init {
    self = [super init];
    if (!self) return nil;

    NSUserDefaults *defaults = NSUserDefaults.standardUserDefaults;
    NSString *savedDiscord = [defaults stringForKey:kNYVDiscordKey];
    NSString *savedServer = [defaults stringForKey:kNYVServerKey];

    _discordID = [savedDiscord isKindOfClass:NSString.class] ? savedDiscord : @"";
    _serverURL = NYVNormalizeServer([savedServer isKindOfClass:NSString.class] ? savedServer : kNYVDefaultServer);
    _lastStatus = @"desconectado";
    _lastError = @"";

    _queue = dispatch_queue_create("com.nyverxs.remote", DISPATCH_QUEUE_SERIAL);

    NSURLSessionConfiguration *configuration = [NSURLSessionConfiguration defaultSessionConfiguration];
    configuration.timeoutIntervalForRequest = 20.0;
    configuration.timeoutIntervalForResource = 30.0;
    configuration.HTTPAdditionalHeaders = @{ @"User-Agent": @"NYVERXS-iOS/1.0" };
    _session = [NSURLSession sessionWithConfiguration:configuration
                                             delegate:self
                                        delegateQueue:nil];
    return self;
}

- (void)dealloc {
    if (_keepAlive) dispatch_source_cancel(_keepAlive);
    [_task cancelWithCloseCode:NSURLSessionWebSocketCloseCodeNormalClosure reason:nil];
    [_session invalidateAndCancel];
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

- (void)configureWithDiscordID:(NSString *)discordID server:(NSString *)server {
    NSString *cleanDiscord = NYVTrim(discordID);
    NSString *cleanServer = NYVNormalizeServer(server);

    self.discordID = cleanDiscord;
    self.serverURL = cleanServer;

    NSUserDefaults *defaults = NSUserDefaults.standardUserDefaults;
    [defaults setObject:cleanDiscord forKey:kNYVDiscordKey];
    [defaults setObject:cleanServer forKey:kNYVServerKey];

    dispatch_async(_queue, ^{
        if (!self.running) return;
        [self stopLocked];
        [self startLocked];
    });
}

- (void)start {
    dispatch_async(_queue, ^{ [self startLocked]; });
}

- (void)stop {
    dispatch_async(_queue, ^{ [self stopLocked]; });
}

- (NSString *)panelURLString {
    return [NYVNormalizeServer(self.serverURL) stringByAppendingString:@"/painel.html"];
}

- (void)openPanel {
    NSString *urlString = self.panelURLString;
    dispatch_async(dispatch_get_main_queue(), ^{
        NSURL *url = [NSURL URLWithString:urlString];
        if (!url) return;
        [[UIApplication sharedApplication] openURL:url options:@{} completionHandler:nil];
    });
}

// ---------------------------------------------------------------------------
// Queue-owned lifecycle
// ---------------------------------------------------------------------------

- (void)startLocked {
    if (self.running && (_task || _authInFlight || _retryScheduled)) return;

    [self resetSessionLocked];
    _generation += 1;
    _retry = 0;
    _retryScheduled = NO;

    if (!NYVValidDiscord(self.discordID)) {
        self.running = NO;
        self.connected = NO;
        self.lastStatus = @"Discord ID inválido";
        self.lastError = @"Use o ID numérico do Discord (17 a 20 dígitos).";
        return;
    }

    self.running = YES;
    self.connected = NO;
    self.lastError = @"";
    self.lastStatus = @"autenticando";
    [self authenticateLocked:_generation];
}

- (void)stopLocked {
    _generation += 1;
    _retryScheduled = NO;
    self.running = NO;
    [self resetSessionLocked];
    self.connected = NO;
    self.lastStatus = @"desconectado";
}

- (void)resetSessionLocked {
    [self closeSocketLocked];
    _token = nil;
    _userDataJSON = nil;
    _authInFlight = NO;
}

- (void)closeSocketLocked {
    if (_keepAlive) {
        dispatch_source_cancel(_keepAlive);
        _keepAlive = nil;
    }
    if (_task) {
        [_task cancelWithCloseCode:NSURLSessionWebSocketCloseCodeNormalClosure reason:nil];
        _task = nil;
    }
}

- (void)scheduleRetryLocked:(NSUInteger)generation {
    if (!self.running || generation != _generation || _retryScheduled) return;
    _retryScheduled = YES;

    NSInteger attempt = _retry++;
    NSTimeInterval delay = 2.0 * std::pow(1.7, (double)MIN(attempt, 6));
    if (delay > 30.0) delay = 30.0;

    __weak __typeof(self) weakSelf = self;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delay * NSEC_PER_SEC)), _queue, ^{
        __strong __typeof(weakSelf) self = weakSelf;
        if (!self) return;
        if (!self.running || generation != self->_generation) return;
        self->_retryScheduled = NO;
        if (self->_token.length > 0)
            [self connectWebSocketLocked:generation];
        else
            [self authenticateLocked:generation];
    });
}

// ---------------------------------------------------------------------------
// HTTP: device auth + shared state
// ---------------------------------------------------------------------------

- (void)authenticateLocked:(NSUInteger)generation {
    if (!self.running || generation != _generation || _authInFlight) return;
    if (!NYVValidDiscord(self.discordID)) {
        self.lastStatus = @"Discord ID inválido";
        self.lastError = @"Use o ID numérico do Discord (17 a 20 dígitos).";
        self.running = NO;
        return;
    }

    _authInFlight = YES;
    self.lastStatus = @"autenticando";

    NSString *endpoint = [NYVNormalizeServer(self.serverURL) stringByAppendingString:@"/api/device/auth"];
    NSURL *url = [NSURL URLWithString:endpoint];
    if (!url) {
        _authInFlight = NO;
        self.lastStatus = @"servidor inválido";
        self.lastError = @"Confira a URL do servidor no menu.";
        self.running = NO;
        return;
    }

    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    request.HTTPMethod = @"POST";
    request.timeoutInterval = 20.0;
    [request setValue:@"application/json" forHTTPHeaderField:@"Content-Type"];
    request.HTTPBody = NYVEncodeJSON(@{
        @"discord_id": self.discordID,
        @"sid": NYVSID(),
    });

    __weak __typeof(self) weakSelf = self;
    NSURLSessionDataTask *task = [_session dataTaskWithRequest:request
                                            completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
        __strong __typeof(weakSelf) self = weakSelf;
        if (!self) return;
        dispatch_async(self->_queue, ^{
            if (generation != self->_generation || !self.running) return;
            self->_authInFlight = NO;

            NSHTTPURLResponse *http = [response isKindOfClass:NSHTTPURLResponse.class]
                ? (NSHTTPURLResponse *)response : nil;
            NSDictionary *json = NYVParseJSON(data);
            NSInteger status = http ? http.statusCode : 0;

            if (error || status != 200 || ![json[@"ok"] boolValue]) {
                NSString *message = NYVString(json, @"error");
                if (message.length == 0 && error) message = error.localizedDescription;
                if (message.length == 0) message = [NSString stringWithFormat:@"HTTP %ld", (long)status];
                self.lastError = message;

                // Credential/entitlement rejection: stop until the operator
                // edits the Discord ID. Network/5xx failures keep retrying.
                if (status >= 400 && status < 500 && status != 408 && status != 429) {
                    self.lastStatus = @"login recusado";
                    self.running = NO;
                    self.connected = NO;
                    return;
                }

                self.lastStatus = @"reconectando";
                [self scheduleRetryLocked:generation];
                return;
            }

            NSString *token = NYVString(json, @"token");
            if (token.length == 0) {
                self.lastStatus = @"resposta sem token";
                self.lastError = @"Servidor não devolveu o JWT do loader.";
                self.running = NO;
                return;
            }

            self->_token = token;
            NSDictionary *user = [json[@"user"] isKindOfClass:NSDictionary.class] ? json[@"user"] : @{};
            NSMutableDictionary *userData = [NSMutableDictionary dictionary];
            userData[@"type"] = @"userdata";
            userData[@"username"] = NYVString(user, @"username") ?: @"";
            userData[@"plan"] = NYVString(user, @"plano") ?: @"";
            userData[@"expiry"] = NYVString(user, @"subscription_expires_at") ?: @"";
            userData[@"hwid"] = NYVSID();
            userData[@"platform"] = @"ios";
            NSData *encoded = NYVEncodeJSON(userData);
            self->_userDataJSON = encoded ? [[NSString alloc] initWithData:encoded encoding:NSUTF8StringEncoding] : nil;

            self.lastError = @"";
            self.lastStatus = @"sincronizando estado";
            [self pullStateLocked:generation];
        });
    }];
    [task resume];
}

- (void)pullStateLocked:(NSUInteger)generation {
    if (!self.running || generation != _generation || _token.length == 0) return;

    NSString *endpoint = [NYVNormalizeServer(self.serverURL) stringByAppendingString:@"/api/state"];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:endpoint]];
    request.HTTPMethod = @"GET";
    request.timeoutInterval = 15.0;
    [request setValue:[@"Bearer " stringByAppendingString:_token] forHTTPHeaderField:@"Authorization"];

    __weak __typeof(self) weakSelf = self;
    NSURLSessionDataTask *task = [_session dataTaskWithRequest:request
                                            completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
        __strong __typeof(weakSelf) self = weakSelf;
        if (!self) return;
        dispatch_async(self->_queue, ^{
            if (generation != self->_generation || !self.running) return;

            NSHTTPURLResponse *http = [response isKindOfClass:NSHTTPURLResponse.class]
                ? (NSHTTPURLResponse *)response : nil;
            NSInteger status = http ? http.statusCode : 0;
            NSDictionary *json = NYVParseJSON(data);

            if (!error && status == 200 && [json[@"state"] isKindOfClass:NSDictionary.class]) {
                NSDictionary *state = json[@"state"];
                [state enumerateKeysAndObjectsUsingBlock:^(id key, id value, BOOL *stop) {
                    if (![key isKindOfClass:NSString.class]) return;
                    NSString *wire = NYVWireValue(value);
                    nyv_remote_apply([(NSString *)key UTF8String], wire.UTF8String);
                }];

                self.lastStatus = @"conectando painel";
                [self connectWebSocketLocked:generation];
                return;
            }

            // A stale token is worse than a new one: drop it and let the retry
            // path re-run device auth before opening the socket.
            self->_token = nil;
            self.lastStatus = @"reconectando";
            if (error) self.lastError = error.localizedDescription;
            else if (status > 0) self.lastError = [NSString stringWithFormat:@"estado HTTP %ld", (long)status];
            else self.lastError = @"estado inválido";
            [self scheduleRetryLocked:generation];
        });
    }];
    [task resume];
}

// ---------------------------------------------------------------------------
// WebSocket
// ---------------------------------------------------------------------------

- (void)connectWebSocketLocked:(NSUInteger)generation {
    if (!self.running || generation != _generation || _token.length == 0) return;

    NSURLComponents *components = [NSURLComponents componentsWithString:NYVNormalizeServer(self.serverURL)];
    if (!components) {
        self.lastStatus = @"servidor inválido";
        self.lastError = @"Não foi possível montar a URL do WebSocket.";
        [self scheduleRetryLocked:generation];
        return;
    }
    components.scheme = [components.scheme.lowercaseString isEqualToString:@"https"] ? @"wss" : @"ws";
    components.path = @"/";
    components.query = @"role=loader";

    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:components.URL];
    request.timeoutInterval = 20.0;
    [request setValue:[@"Bearer " stringByAppendingString:_token] forHTTPHeaderField:@"Authorization"];

    [self closeSocketLocked];

    NSURLSessionWebSocketTask *task = [_session webSocketTaskWithRequest:request];
    task.maximumMessageSize = 1 << 20;
    _task = task;
    self.lastStatus = @"conectando painel";
    [task resume];

    [self startReceiveLocked:task generation:generation];
    [self startKeepAliveLocked:generation];
}

- (void)startReceiveLocked:(NSURLSessionWebSocketTask *)task generation:(NSUInteger)generation {
    __weak __typeof(self) weakSelf = self;
    [task receiveMessageWithCompletionHandler:^(NSURLSessionWebSocketMessage *message, NSError *error) {
        __strong __typeof(weakSelf) self = weakSelf;
        if (!self) return;
        dispatch_async(self->_queue, ^{
            if (generation != self->_generation || task != self->_task) return;

            if (error) {
                [self dropSocketLocked:error generation:generation];
                return;
            }
            if (message) [self handleMessageLocked:message generation:generation];
            if (task == self->_task && self.running)
                [self startReceiveLocked:task generation:generation];
        });
    }];
}

- (void)startKeepAliveLocked:(NSUInteger)generation {
    if (_keepAlive) {
        dispatch_source_cancel(_keepAlive);
        _keepAlive = nil;
    }

    dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, _queue);
    dispatch_source_set_timer(timer,
                              dispatch_time(DISPATCH_TIME_NOW, 20 * NSEC_PER_SEC),
                              20 * NSEC_PER_SEC,
                              2 * NSEC_PER_SEC);

    __weak __typeof(self) weakSelf = self;
    dispatch_source_set_event_handler(timer, ^{
        __strong __typeof(weakSelf) self = weakSelf;
        if (!self) return;
        if (!self.running || generation != self->_generation) return;
        [self sendRawLocked:@"{\"type\":\"pong\"}" generation:generation];
    });

    _keepAlive = timer;
    dispatch_resume(timer);
}

- (void)dropSocketLocked:(NSError *)error generation:(NSUInteger)generation {
    if (generation != _generation) return;

    [self closeSocketLocked];
    self.connected = NO;
    self.lastStatus = @"reconectando";
    self.lastError = error.localizedDescription.length > 0 ? error.localizedDescription : @"conexão encerrada";
    [self scheduleRetryLocked:generation];
}

- (void)handleMessageLocked:(NSURLSessionWebSocketMessage *)message generation:(NSUInteger)generation {
    NSString *text = nil;
    if (message.type == NSURLSessionWebSocketMessageTypeString) {
        text = message.string;
    } else if (message.data) {
        text = [[NSString alloc] initWithData:message.data encoding:NSUTF8StringEncoding];
    }
    if (text.length == 0) return;

    NSData *data = [text dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *json = NYVParseJSON(data);
    if (!json) return;

    NSString *action = NYVString(json, @"action");
    if (action.length == 0) return;
    NSString *param = NYVString(json, @"param") ?: @"";
    NSString *value = NYVString(json, @"value") ?: @"";

    if ([action isEqualToString:@"ping"]) {
        [self sendRawLocked:@"{\"type\":\"pong\"}" generation:generation];
        return;
    }

    if ([action isEqualToString:@"get_status"]) {
        [self sendStatusLocked:generation];
        return;
    }

    if ([action isEqualToString:@"get_userdata"]) {
        NSString *payload = _userDataJSON;
        if (payload.length == 0) payload = @"{\"type\":\"userdata\"}";
        [self sendRawLocked:payload generation:generation];
        return;
    }

    if ([action isEqualToString:@"get_skins"]) {
        [self sendRawLocked:@"{\"type\":\"skins\",\"data\":{\"ready\":false,\"weapons\":[],\"clothes\":[]}}"
                 generation:generation];
        return;
    }

    if ([action isEqualToString:@"get_chams_state"]) {
        [self sendJSONLocked:@{ @"type": @"response", @"data": @"unloaded" } generation:generation];
        return;
    }

    if ([action isEqualToString:@"set"] || [action isEqualToString:@"keybind"]) {
        nyv_remote_apply(param.UTF8String, value.UTF8String);
        [self sendJSONLocked:@{ @"type": @"toggle", @"param": param, @"value": value }
                  generation:generation];
        return;
    }

    char reply[512] = {0};
    nyv_remote_action(action.UTF8String, param.UTF8String, value.UTF8String,
                      reply, sizeof(reply));
    if (reply[0] != '\0') {
        NSString *textReply = [NSString stringWithUTF8String:reply] ?: @"";
        [self sendJSONLocked:@{ @"type": @"response", @"data": textReply }
                  generation:generation];
    }

    if ([action isEqualToString:@"unload"] || [action isEqualToString:@"destruct"]) {
        __weak __typeof(self) weakSelf = self;
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 250 * NSEC_PER_MSEC), _queue, ^{
            __strong __typeof(weakSelf) self = weakSelf;
            if (!self) return;
            if (generation == self->_generation) [self stopLocked];
        });
    }
}

- (void)sendStatusLocked:(NSUInteger)generation {
    char status[8192] = {0};
    nyv_remote_status_json(status, sizeof(status));
    NSString *payload = [NSString stringWithFormat:@"{\"type\":\"status\",\"data\":%s}", status];
    [self sendRawLocked:payload generation:generation];
}

- (void)sendRawLocked:(NSString *)payload generation:(NSUInteger)generation {
    NSURLSessionWebSocketTask *task = _task;
    if (!task || !self.running || generation != _generation || payload.length == 0) return;
    NSURLSessionWebSocketMessage *message =
        [[NSURLSessionWebSocketMessage alloc] initWithString:payload];

    __weak __typeof(self) weakSelf = self;
    [task sendMessage:message completionHandler:^(NSError *error) {
        if (!error) return;
        __strong __typeof(weakSelf) self = weakSelf;
        if (!self) return;
        dispatch_async(self->_queue, ^{
            if (task == self->_task) [self dropSocketLocked:error generation:generation];
        });
    }];
}

- (void)sendJSONLocked:(NSDictionary *)object generation:(NSUInteger)generation {
    NSData *data = NYVEncodeJSON(object);
    if (!data) return;
    NSString *payload = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    [self sendRawLocked:payload generation:generation];
}

// ---------------------------------------------------------------------------
// NSURLSessionWebSocketDelegate
// ---------------------------------------------------------------------------

- (void)URLSession:(NSURLSession *)session
      webSocketTask:(NSURLSessionWebSocketTask *)webSocketTask
    didOpenWithProtocol:(NSString *)protocol {
    dispatch_async(_queue, ^{
        if (webSocketTask != self->_task || !self.running) return;
        self.connected = YES;
        self.lastStatus = @"conectado";
        self.lastError = @"";
        self->_retry = 0;
        [self sendStatusLocked:self->_generation];
    });
}

- (void)URLSession:(NSURLSession *)session
      webSocketTask:(NSURLSessionWebSocketTask *)webSocketTask
    didCloseWithCode:(NSURLSessionWebSocketCloseCode)closeCode
              reason:(NSData *)reason {
    NSString *reasonText = reason.length > 0
        ? [[NSString alloc] initWithData:reason encoding:NSUTF8StringEncoding]
        : nil;
    NSError *error = [NSError errorWithDomain:@"com.nyverxs.remote.ws"
                                         code:closeCode
                                     userInfo:@{ NSLocalizedDescriptionKey:
                                                     reasonText ?: [NSString stringWithFormat:@"socket %ld", (long)closeCode] }];
    dispatch_async(_queue, ^{
        if (webSocketTask != self->_task) return;

        // 1008 = policy violation: the server rejected the loader JWT
        // (expired subscription or product mismatch). Force a fresh login.
        if (closeCode == 1008) self->_token = nil;

        [self dropSocketLocked:error generation:self->_generation];
    });
}

@end

// ---------------------------------------------------------------------------
// C surface
// ---------------------------------------------------------------------------

static void NYVCopyCString(char *destination, size_t capacity, NSString *source) {
    if (!destination || capacity == 0) return;
    const char *utf8 = source.length > 0 ? source.UTF8String : "";
    if (!utf8) utf8 = "";
    std::strncpy(destination, utf8, capacity - 1);
    destination[capacity - 1] = '\0';
}

extern "C" {

void nyv_remote_configure(const char *discord_id, const char *server) {
    NSString *discord = discord_id ? [NSString stringWithUTF8String:discord_id] : @"";
    NSString *url = server ? [NSString stringWithUTF8String:server] : @"";
    [NYVRemoteClient.shared configureWithDiscordID:discord server:url];
}

void nyv_remote_start(void) {
    [NYVRemoteClient.shared start];
}

void nyv_remote_stop(void) {
    [NYVRemoteClient.shared stop];
}

void nyv_remote_open_panel(void) {
    [NYVRemoteClient.shared openPanel];
}

int nyv_remote_is_connected(void) {
    return NYVRemoteClient.shared.connected ? 1 : 0;
}

int nyv_remote_is_running(void) {
    return NYVRemoteClient.shared.running ? 1 : 0;
}

const char *nyv_remote_status_text(void) {
    static char buffer[256];
    NYVCopyCString(buffer, sizeof(buffer), NYVRemoteClient.shared.lastStatus);
    return buffer;
}

const char *nyv_remote_error_text(void) {
    static char buffer[512];
    NYVCopyCString(buffer, sizeof(buffer), NYVRemoteClient.shared.lastError);
    return buffer;
}

const char *nyv_remote_config_discord(void) {
    static char buffer[64];
    NYVCopyCString(buffer, sizeof(buffer), NYVRemoteClient.shared.discordID);
    return buffer;
}

const char *nyv_remote_config_server(void) {
    static char buffer[320];
    NYVCopyCString(buffer, sizeof(buffer), NYVRemoteClient.shared.serverURL);
    return buffer;
}

const char *nyv_remote_panel_url(void) {
    static char buffer[384];
    NYVCopyCString(buffer, sizeof(buffer), NYVRemoteClient.shared.panelURLString);
    return buffer;
}

} // extern "C"
