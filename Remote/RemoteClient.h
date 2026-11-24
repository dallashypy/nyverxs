// ---------------------------------------------------------------------------
// NYVERXS remote client - Discord ID only, no launcher required.
//
// The phone authenticates against the NoLogs site (POST /api/device/auth),
// pulls the shared panel state and opens the loader WebSocket. The site panel
// then drives the tweak through the same `set` contract used by the Windows
// loader: { action, param, value }.
// ---------------------------------------------------------------------------

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NYVRemoteClient : NSObject

@property (atomic, copy, readonly) NSString *discordID;
@property (atomic, copy, readonly) NSString *serverURL;
@property (atomic, copy, readonly) NSString *lastStatus;
@property (atomic, copy, readonly) NSString *lastError;
@property (atomic, readonly) BOOL connected;
@property (atomic, readonly) BOOL running;

+ (instancetype)shared;

// Stores the credentials and restarts the link when it is already running.
- (void)configureWithDiscordID:(NSString *)discordID server:(NSString *)server;
- (void)start;
- (void)stop;
- (void)openPanel;
- (NSString *)panelURLString;

@end

NS_ASSUME_NONNULL_END

#ifdef __cplusplus
extern "C" {
#endif

// C surface consumed by the ImGui menu and by the draw loop.
void nyv_remote_configure(const char *discord_id, const char *server);
void nyv_remote_start(void);
void nyv_remote_stop(void);
void nyv_remote_open_panel(void);

int nyv_remote_is_connected(void);
int nyv_remote_is_running(void);

const char *nyv_remote_status_text(void);
const char *nyv_remote_error_text(void);
const char *nyv_remote_config_discord(void);
const char *nyv_remote_config_server(void);
const char *nyv_remote_panel_url(void);

#ifdef __cplusplus
}
#endif
