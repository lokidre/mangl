#import "mangl_main_window.h"

#import "mangl_main_view.h"
#import "mangl_main_vc.h"
#import "mangl_rate_annoyer.h"

#import "../apple/mangl_imacos_util.h"

#include "../core/audio/mangl_music.h"
#include "../core/audio/mangl_sound.h"
#include "../core/mangl_framework.h"


namespace mangl {

struct MMainWindowSettings {
    Setting<RectI> windowRect{"mangl-state-window-rect"};
    Setting<bool> windowZoomed{"mangl-state-window-zoomed"} ;
    Setting<PointI> windowZoomedPos{"mangl-state-window-zoomed"};
    Setting<bool> windowFullscreen{"mangl-state-window-fullscreen"};
};

}  // namespace mangl

@interface ManglMainWindowObjC() {
    mangl::Rect normalWindowRect_, zoomedWindowRect_;

    mangl::Size originalViewSize_;
    double viewRatio_;
    
    bool fullscreen_;
    bool performingFullscreen_;
    bool viewStarted_;
    
    mangl::MMainWindowSettings settings_;
}

@property (nonatomic) ManglMainViewController *mainViewController;
@property (nonatomic) BOOL zooming;
//@property (nonatomic) NSView* backgroundView;
@property (nonatomic) ManglMainView* runtimeView;

@end



using namespace mangl;

@implementation ManglMainWindowObjC

- (instancetype)initWithContentRect:(NSRect)contentRect
                          styleMask:(NSWindowStyleMask)style
                            backing:(NSBackingStoreType)backingStoreType
                              defer:(BOOL)flag
{
    MDebugFunc();
    
    self = [super initWithContentRect:contentRect 
                            styleMask:style
                              backing:backingStoreType
                                defer:flag];
    
    [self ctor: contentRect];
    
    return self;
}

-(void)ctor:(NSRect)contentRect
{
    //launchFrame_ = contentRect;
    
    
    fullscreen_ = false;
    performingFullscreen_ = false;
    
    viewStarted_ = true;
}

-(void)beginLaunch
{
    if (self.isVisible) {
        MDebugFunc("Warning: Main NSWindow is visible at launch. Set it invisible in XIB designer.");
    }
    
    
    self.delegate = self;

    //_mainViewController = [[ManglMainViewController alloc] initWithNibName:@"MainMenu" bundle:nil];
    _mainViewController = [[ManglMainViewController alloc] initWithNibName:nil bundle:nil];
    self.contentViewController = _mainViewController;
    
    
    //_backgroundView = [[NSView alloc] initWithFrame:frame];
    //self.contentView = _backgroundView;
    //self.contentView.wantsLayer = YES;
    //self.contentView.layer.backgroundColor = [[NSColor yellowColor] CGColor];
    
    //auto contentView = self.contentView;
   
    
    //_runtimeView = [[ManglMainView alloc] initWithFrame:CGRectZero];
    //_mainViewController.view = _runtimeView;
    
    //[self.contentView addSubview:_runtimeView];
    
    _runtimeView = _mainViewController.runtimeView;
}

- (mangl::Rect)contentRectToWindow:(RectA)rect
{
    auto w = [self frameRectForContentRect:toNSRect(rect)];
    return fromNSRect(w);
}

- (mangl::Rect)windowRectToContent:(RectA)rect
{
    auto w = [self contentRectForFrameRect:toNSRect(rect)];
    return fromNSRect(w);
}

- (mangl::Rect)adjustWindowRectWidth:(RectA)rect
{
    auto contentRect = [self windowRectToContent:rect];
    contentRect.w = contentRect.h / viewRatio_;

    return [self contentRectToWindow:contentRect];
}

-(void)continueLaunch
{
    originalViewSize_ = Env::layoutSize;
    viewRatio_ = originalViewSize_.h / originalViewSize_.w;
    
    bool windowZoomed = settings_.windowZoomed();
    bool windowFullScreened = settings_.windowFullscreen();
    
    auto contentsRect = originalViewSize_.rect<MRect>();
    
    // Initialize the normal window frame
    //normalRect_.resize(Env::layoutSize);// size = CGSizeMake(Env::layoutSize.w, Env::layoutSize.h);
    
    auto visibleFrame = NSScreen.mainScreen.visibleFrame;
    auto visibleRect = fromNSRect(visibleFrame);
    
    NSUInteger styleMask = (NSWindowStyleMaskTitled | NSWindowStyleMaskClosable
                            | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable);
    
    //auto windowFrame = [self frameRectForContentRect:toNSRect(contentsRect)];
    //auto windowRect = fromNSRect(windowFrame);
    auto windowRect = [self contentRectToWindow:contentsRect];
    
    // make initial screen size = 80% of the user's screen
    if (windowRect.h >= visibleRect.h * 0.8) {
        windowRect.h = visibleRect.h * 0.8;
        windowRect = [self adjustWindowRectWidth:windowRect];
    }
   
    auto savedRectI = settings_.windowRect({});
    auto savedRect = savedRectI.convert<MRect>();

    if (savedRectI.w && savedRectI.h) {
        windowRect.resize(savedRect.size());
    }
    
    if (windowRect.h > visibleRect.h) {
        windowRect.h = visibleRect.h;
        windowRect = [self adjustWindowRectWidth:windowRect];
    }
    
    // Initial window positions
    auto diff = visibleRect - windowRect;
    windowRect.move(visibleRect.position() + MPoint{diff.w, diff.h} / 2);
    
    // Get the saved position
    if (savedRectI.w && savedRectI.h) {
        windowRect.move(savedRect.position());
    }
    
    // check if the window is in the screen
    if (windowRect.right() > visibleRect.w)
        windowRect.x = visibleRect.w - windowRect.w;
    
    if (windowRect.top() > visibleRect.h)
        windowRect.y = visibleRect.h - windowRect.h;
    
   
    //
    // Make the window zoomed nicely
    //
    zoomedWindowRect_.x = windowRect.x - (visibleRect.h / viewRatio_ - windowRect.w) / 2;
    zoomedWindowRect_.y = visibleRect.y;
    
    //auto zoomedPosI = settings_.windowZoomedPos(zoomedWindowRect_.position().convert<PointI>());
    auto zoomedPosI = settings_.windowZoomedPos({-1, -1});//zoomedWindowRect_.position().convert<PointI>());
    if (zoomedPosI.x != -1 && zoomedPosI.y != -1)
        zoomedWindowRect_.move(zoomedPosI.convert<MPoint>());
    
    normalWindowRect_ = windowRect;
    auto windowFrame = toNSRect(windowRect);

    //self.backgroundColor = [NSColor colorWithCalibratedWhite:0.8 alpha:1];
    self.styleMask = styleMask;
    self.collectionBehavior = NSWindowCollectionBehaviorFullScreenPrimary;
    self.contentAspectRatio = toNSSize(originalViewSize_);
    self.showsResizeIndicator = NO;
    self.preservesContentDuringLiveResize = NO;
    //self.preservesContentDuringLiveResize = YES;

    [self setFrame:windowFrame display:NO];
    
    self.opaque = YES;

#ifdef MANGL_APP_TITLE
    self.title = @MANGL_APP_TITLE;
#endif

    [_runtimeView attachRender:mangl_framework_->render_];
                       
    [self updateWindowRect:windowFrame];

    if (windowFullScreened)
        [self toggleFullScreen:self];
    else if (windowZoomed)
        [self zoom:self];
    
    if (auto music = Music::instance(); !music->settingEnabled())
        music->enable(false);
    
    if (auto sfx = Sfx::instance(); !sfx->settingEnabled())
        sfx->enable(false);
    
    [self makeMainWindow];
    [self makeKeyAndOrderFront:self];
    [self makeFirstResponder:_runtimeView];
}

- (BOOL)canBecomeMainWindow { return YES; }

- (BOOL)canBecomeKeyWindow { return YES; }


-(void)onLaunch {
    [self beginLaunch];
    [_runtimeView onLaunch];
    [self continueLaunch];
}

-(void)onTerminate {
    [_runtimeView onTerminate];
}


- (void)zoom:(id)sender
{
    _zooming = true;
	
    [super zoom:sender];
}

- (BOOL)isZoomed
{
    auto visibleFrame = NSScreen.mainScreen.visibleFrame;
    auto windowFrame = self.frame;
    
    return std::fabs(visibleFrame.size.height - windowFrame.size.height) < 3;
}

- (mangl::Rect)updateWindowRect:(NSRect)windowRect {
    auto viewRect = [self contentRectForFrameRect:windowRect];
    auto rect = fromNSRect(viewRect);
    return [self updateContentsLayout:rect];
}

- (mangl::Rect)updateContentsLayout:(mangl::Rect)rect
{
    MDebugFunc("%.0z", rect);
    
    if (!_runtimeView)
        return rect;
    
    // TODO: vertical and horizontal cases
    auto ratioWidth = rect.h / viewRatio_;
    rect.x = (rect.w - ratioWidth) / 2;
    //rect.x = 0;
    rect.w = ratioWidth;
    rect.y = 0;
    
    Env::applicationRect.resize(rect.size());
    Env::screenScale = Env::applicationRect.w * Env::pixelDensity / Env::layoutSize.w;
    Env::logicalScreenSize = Env::applicationRect.size() / Env::pixelDensity;
    Env::updateConversionData();
    
    _runtimeView.frame = toNSRect(rect);
    
    [_runtimeView onViewport];
    
    MDebugPrint("    out: %.0z", rect);
    
    return rect;
}

-(void)pauseView {
    if (!viewStarted_)
        return ;
    [_runtimeView onPause];
    //[_mainView pauseRefresh];
    viewStarted_ = false;
}

-(void)pauseRefresh {
    [_runtimeView pauseRefresh];
}

-(void)resumeView {
    if (viewStarted_)
        return ;
    [_runtimeView onResumeTextures];
    [_runtimeView resumeRefresh];
    viewStarted_ = true;
}



// MARK: NSWindowDelegate

- (void)windowWillMiniaturize:(NSNotification *)notification {
    MDebugFunc();
    [self pauseView];
    [self pauseRefresh];
}

- (void)windowWillClose:(NSNotification *)notification
{
    MDebugFunc();
    
    settings_.windowRect = normalWindowRect_.convert<RectI>();
    settings_.windowZoomedPos = zoomedWindowRect_.position().convert<PointI>();
    
    settings_.windowZoomed = self.isZoomed;
    settings_.windowFullscreen = fullscreen_;
    
    // close all child windows
    for (NSWindow *child in self.childWindows) {
        [child orderOut:self];
        [child close];
    }
}


- (void)windowDidResignMain:(NSNotification *)notification
{
    MDebugFunc();
    [self pauseView];
}


- (void)windowDidBecomeMain:(NSNotification *)notification
{
    MDebugFunc();
    [self resumeView];
}


// MARK: Zoom Support

- (NSRect)windowWillUseStandardFrame:(NSWindow *)window defaultFrame:(NSRect)frame
{
    auto rect = fromNSRect(frame);

    auto normalCenter = normalWindowRect_.center();
    
    auto contentRect = [self windowRectToContent:rect];
    auto updated = [self updateContentsLayout:contentRect];

    //auto center = rect.center();
    updated.x = normalCenter.x - rect.w/2;
    updated.y = normalCenter.y - rect.h/2;

    if (fullscreen_ || performingFullscreen_) {
        return toNSRect(updated);
    }
    
    
    self.zooming = false;
    updated = [self contentRectToWindow:updated];
    
    auto screen = [self windowRectToContent:fromNSRect(NSScreen.mainScreen.frame)];
    
    if (updated.y < rect.y)
        updated.y = rect.y;
    if (updated.top() > screen.top())
        updated.y = screen.y;
    
    if (updated.right() > screen.right())
        updated.x = screen.right() - updated.w;
    if (updated.x < rect.x)
        updated.x = rect.x;

    MDebugFunc("%.0z -> %.0z", rect, updated);

    return toNSRect(updated);
}


-(BOOL)windowShouldZoom:(NSWindow *)window toFrame:(NSRect)newFrame {
    return YES;
}


- (void)windowWillStartLiveResize:(NSNotification *)notification {
    MDebugFunc();
//    [self pauseView];
//    [self pauseRefresh];
}

- (void)windowDidEndLiveResize:(NSNotification *)notification
{
    auto frame = self.frame;
    auto rect = fromNSRect(frame);

    MDebugFunc("%.0z", rect);

    if (!fullscreen_ && !performingFullscreen_) {
        (self.isZoomed ? zoomedWindowRect_ : normalWindowRect_) = rect;
    }

    [self updateWindowRect:frame];
    [self resumeView];
}

- (void)windowDidResize:(NSNotification *)notification
{
    auto frame = self.frame;
    //MDebugFunc("%.0z", fromNSRect(frame));

    [self updateWindowRect:frame];
}

- (void)windowDidMove:(NSNotification *)notification
{
    auto frame = self.frame;
    auto rect = fromNSRect(frame);
    
    //MDebugFunc("%.0z", rect);
    
    if (!fullscreen_ && !performingFullscreen_) {
        (self.isZoomed ? zoomedWindowRect_ : normalWindowRect_) = rect;
    }
}


// MARK: Full Screen Support

- (void)windowWillEnterFullScreen:(NSNotification *)notification
{
    MDebugFunc();
    
    performingFullscreen_ = true;
    
    if (self.isZoomed)
        zoomedWindowRect_ = fromNSRect(self.frame);
    
    //[self pauseView];
    //[self pauseRefresh];
}



- (void)windowDidEnterFullScreen:(NSNotification *)notification
{
    performingFullscreen_ = false;
    fullscreen_ = true;
    
    auto frame = NSScreen.mainScreen.frame;
    
    MDebugFunc("%.0z", fromNSRect(frame));
    
    [self updateWindowRect:frame];
    [self resumeView];
}


- (void)windowWillExitFullScreen:(NSNotification *)notification
{
    MDebugFunc();
    
    performingFullscreen_ = true;
    //[self pauseView];
    //[self pauseRefresh];
}

- (void)windowDidExitFullScreen:(NSNotification *)notification
{
    auto frame = self.frame;

    MDebugFunc("%.0z", fromNSRect(frame));
    
    performingFullscreen_ = false;
    fullscreen_ = false;


    [self updateWindowRect:frame];
    [self resumeView];
}


- (void)windowDidFailToEnterFullScreen:(NSWindow *)window {
    MDebugFunc();
    
    performingFullscreen_ = false;
    [self resumeView];
}

- (void)windowDidFailToExitFullScreen:(NSWindow *)window {
    MDebugFunc();
    
    performingFullscreen_ = false;
    [self resumeView];
}


@end


