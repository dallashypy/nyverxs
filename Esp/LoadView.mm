#import <UIKit/UIKit.h>
#include "Includes.h"
#import "menuIcon.h"
#import "Esp/Obfuscate.h"
#import "Remote/RemoteClient.h"
#define timer(sec) dispatch_after(dispatch_time(DISPATCH_TIME_NOW, sec * NSEC_PER_SEC), dispatch_get_main_queue(), ^

@interface MenuLoad()
@property (nonatomic, strong) ImGuiDrawView *vna;

- (ImGuiDrawView*) GetImGuiView;
@end

static MenuLoad *extraInfo;

UIButton* InvisibleMenuButton;
UIButton* VisibleMenuButton;
MenuInteraction* menuTouchView;
UITextField* hideRecordTextfield;
UIView* hideRecordView;

@interface MenuInteraction()
@end

@implementation MenuInteraction

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    if ([ImGuiDrawView isMenuShowing]) { [[extraInfo GetImGuiView] updateIOWithTouchEvent:event]; }
    [super touchesBegan:touches withEvent:event]; // repassa para o jogo
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    if ([ImGuiDrawView isMenuShowing]) { [[extraInfo GetImGuiView] updateIOWithTouchEvent:event]; }
    [super touchesMoved:touches withEvent:event];
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    if ([ImGuiDrawView isMenuShowing]) { [[extraInfo GetImGuiView] updateIOWithTouchEvent:event]; }
    [super touchesCancelled:touches withEvent:event];
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    if ([ImGuiDrawView isMenuShowing]) { [[extraInfo GetImGuiView] updateIOWithTouchEvent:event]; }
    [super touchesEnded:touches withEvent:event];
}

@end

@implementation MenuLoad

- (ImGuiDrawView*) GetImGuiView
{
    return _vna;
}


static void didFinishLaunching(CFNotificationCenterRef center, void *observer, CFStringRef name, const void *object, CFDictionaryRef info)
{
    timer(7) {
        extraInfo = [MenuLoad new];
        [extraInfo initTapGes];

        // Discord ID only: no vendor license, no launcher. When a Discord ID
        // was saved by a previous session the link comes up on its own; the
        // REMOTE tab in the menu handles first setup.
        const char *savedDiscord = nyv_remote_config_discord();
        if (savedDiscord && savedDiscord[0] != '\0')
            nyv_remote_start();
    });
}

__attribute__((constructor)) static void initialize()
{
    CFNotificationCenterAddObserver(CFNotificationCenterGetLocalCenter(), NULL, &didFinishLaunching, (__bridge CFStringRef)UIApplicationDidFinishLaunchingNotification, NULL, CFNotificationSuspensionBehaviorDrop);
}

-(void)initTapGes
{
    UIView* mainView = [UIApplication sharedApplication].windows[0].rootViewController.view;
    hideRecordTextfield = [[UITextField alloc] init];
    hideRecordView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, UIScreen.mainScreen.bounds.size.width, UIScreen.mainScreen.bounds.size.height)];
    [hideRecordView setBackgroundColor:[UIColor clearColor]];
    [hideRecordView setUserInteractionEnabled:YES];
    hideRecordTextfield.secureTextEntry = true;
    [hideRecordView addSubview:hideRecordTextfield];
    CALayer *layer = hideRecordTextfield.layer;
    if ([layer.sublayers.firstObject.delegate isKindOfClass:[UIView class]]) {
        hideRecordView = (UIView *)layer.sublayers.firstObject.delegate;
    } else {
        hideRecordView = nil;
    }

    [[UIApplication sharedApplication].keyWindow addSubview:hideRecordView];
    
    if (!_vna) {
         ImGuiDrawView *vc = [[ImGuiDrawView alloc] init];
         _vna = vc;
         _vna.view.multipleTouchEnabled = YES; // Garantir multitouch no ImGui
    }
     
    [ImGuiDrawView showChange:false];
_vna.view.userInteractionEnabled = NO;
    [hideRecordView addSubview:_vna.view];

    menuTouchView = [[MenuInteraction alloc] initWithFrame:mainView.frame];
    menuTouchView.multipleTouchEnabled = YES;
menuTouchView.multipleTouchEnabled = YES;
    [[UIApplication sharedApplication].windows[0].rootViewController.view addSubview:menuTouchView];

    NSData* data = [[NSData alloc] initWithBase64EncodedString:menuIcon options:0];
    UIImage* menuIconImage = [UIImage imageWithData:data];

    InvisibleMenuButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    InvisibleMenuButton.frame = CGRectMake(10, 10, 50, 50);
    InvisibleMenuButton.backgroundColor = [UIColor clearColor];
    InvisibleMenuButton.multipleTouchEnabled = YES;
    InvisibleMenuButton.multipleTouchEnabled = YES;
[InvisibleMenuButton addTarget:self action:@selector(buttonDragged:withEvent:) forControlEvents:UIControlEventTouchDragInside];
    UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(showMenu:)];
    [InvisibleMenuButton addGestureRecognizer:tapGestureRecognizer];
    [[UIApplication sharedApplication].windows[0].rootViewController.view addSubview:InvisibleMenuButton];
    
    VisibleMenuButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    VisibleMenuButton.frame = CGRectMake(10, 10, 50, 50);
    VisibleMenuButton.backgroundColor = [UIColor clearColor];
    VisibleMenuButton.layer.cornerRadius = VisibleMenuButton.frame.size.width * 0.5f;
    [VisibleMenuButton setBackgroundImage:menuIconImage forState:UIControlStateNormal];
    VisibleMenuButton.multipleTouchEnabled = YES;
VisibleMenuButton.multipleTouchEnabled = YES;
    [hideRecordView addSubview:VisibleMenuButton];

    // Triple tap (3 dedos) para abrir menu
    UITapGestureRecognizer *tripleTapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(showMenu:)];
    tripleTapGesture.numberOfTapsRequired = 2;
    tripleTapGesture.numberOfTouchesRequired = 3;
    [mainView addGestureRecognizer:tripleTapGesture];
}


- (void)showMenu:(UITapGestureRecognizer *)tapGestureRecognizer {
    if (tapGestureRecognizer.state == UIGestureRecognizerStateEnded) {
        BOOL open = ![ImGuiDrawView isMenuShowing];
        [ImGuiDrawView showChange:open];
        if (_vna && _vna.view) {
            _vna.view.userInteractionEnabled = open;
        }
    }
}

- (void)buttonDragged:(UIButton *)button withEvent:(UIEvent *)event
{
    UITouch *touch = [[event touchesForView:button] anyObject];

    CGPoint previousLocation = [touch previousLocationInView:button];
    CGPoint location = [touch locationInView:button];
    CGFloat delta_x = location.x - previousLocation.x;
    CGFloat delta_y = location.y - previousLocation.y;

    button.center = CGPointMake(button.center.x + delta_x, button.center.y + delta_y);

    VisibleMenuButton.center = button.center;
    VisibleMenuButton.frame = button.frame;
}

@end
