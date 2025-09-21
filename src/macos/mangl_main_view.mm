#import "mangl_main_view.h"

#import <CoreGraphics/CoreGraphics.h>
#import <QuartzCore/CVDisplayLink.h>


#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>

#include "../core/mangl_framework.h"
#import "../apple/mangl_imacos_util.h"


//#define MANGL_RENDER_ON_MAIN_THREAD 1

#define MANGL_CALLBACK_BEGIN  @autoreleasepool { @try { try {

#define MANGL_CALLBACK_END \
    } catch (const std::exception& e) { [self handleException:e.what()]; }\
      catch (...) { [self handleException:"Unhandled Exception"]; }\
    } @catch (NSException *e) { [self handleNSException:e]; }}




namespace mangl {

using LockGuard = MFrameworkNative::TimerLockGuard;
inline auto& getMutex() {  return MFrameworkNative::getTimerMutex();  }

static ManglMainView* manglMainViewSharedInstance__{};

namespace internal {
using RuntimeInitTable = std::vector<std::function<void()>>;
static RuntimeInitTable *manglRuntimeInitTable_{};
}  // namespace mangl::internal

bool manglPushInit(std::function<void()> f)
{
    using namespace mangl::internal;
    
    if (!manglRuntimeInitTable_)
        manglRuntimeInitTable_ = new RuntimeInitTable;
    
    manglRuntimeInitTable_->push_back(f);
    
    return true;
}


static bool haveTimebase_{};
static long double timeMul_ = 0;

static MFrameworkNative* fw_{};
static SceneRenderer* renderer_{};

}  // namespace mangl

using namespace mangl;

static CVReturn displayLinkCallback(CVDisplayLinkRef displayLink,
                                    const CVTimeStamp* now,
                                    const CVTimeStamp* outputTime,
                                    CVOptionFlags flagsIn,
                                    CVOptionFlags *flagsOut,
                                    void *displayLinkContext)
{
    auto view = (__bridge ManglMainView *)displayLinkContext;

    long double machTime = mach_absolute_time();
    
    if (!haveTimebase_) {
        mach_timebase_info_data_t ti;
        mach_timebase_info(&ti);
        timeMul_ = ((long double)ti.numer) / ((long double)ti.denom);
        haveTimebase_ = true;
    }
    
    auto timestamp = static_cast<Timestamp>(machTime * timeMul_ / 1000000000.);

    
    //
    // Rendering on the main thread
    //
    
    //[mainView setNeedsDisplay:YES];
    return [view getFrameForTime:timestamp];
}


@interface ManglMainView() {
    CVDisplayLinkRef displayLink_;
    dispatch_source_t dispatchSource_;
    
    bool displayLinkCreated_;
    bool needInit_, needViewport_, needDestroy_;
    bool needResumeTextures_;
    bool needTimerStop_;
    
    Timestamp timestamp_;
}

@end

@implementation ManglMainView

+ (instancetype)instance {  return manglMainViewSharedInstance__;  }

- (BOOL)acceptsFirstResponder { return YES; }

- (void)ctor
{
    displayLinkCreated_ = false;
    needInit_ = needViewport_ = needDestroy_ = false;
    needResumeTextures_ = false;
    needTimerStop_ = false;
    
    timestamp_ = 0;
    
    self.wantsLayer = YES;
    
    // Initialize the components
    using namespace mangl::internal;
    
    if (manglRuntimeInitTable_) {
        for (auto &f: *manglRuntimeInitTable_)
            f();
        delete manglRuntimeInitTable_;   // free up some memory
        manglRuntimeInitTable_ = nullptr;
    }
    
    //auto nc = NSNotificationCenter.defaultCenter;
    
    //[nc addObserver:self selector:@selector(onBoundsDidChange:) name:NSViewBoundsDidChangeNotification object:self];
}


- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    [self ctor];
    return self;
}

-(void)dealloc {
    ADebugPrintFunc();
    
    //auto nc = NSNotificationCenter.defaultCenter;
    //[nc removeObserver:self name:NSViewBoundsDidChangeNotification object:self];
}


- (void)handleException:(StrView)message
{
    String exceptionMessage{message};
    
    dispatch_async(dispatch_get_main_queue(), ^{
        CVDisplayLinkStop(self->displayLink_);
        
        auto msg = exceptionMessage;
        
#ifdef MANGL_SUPPORT_EMAIL
        auto types = [NSArray arrayWithObject:NSStringPboardType];
        [NSPasteboard.generalPasteboard declareTypes:types owner:nil];
        auto res = [NSPasteboard.generalPasteboard setString:toNSString(msg) forType:NSStringPboardType];
        if (res)
            msg += "\nThe message has been copied to clipboard.\n";
        
        msg += "\nPlease send this message to: ";
        msg += MANGL_SUPPORT_EMAIL;
#endif
        
        auto alert = [[NSAlert alloc] init];
        alert.alertStyle = NSAlertStyleCritical;
        alert.messageText = @"Critical Error";
        alert.informativeText = toNSString(msg);
        [alert runModal];
        
        [NSApplication.sharedApplication terminate:self];
        
    });
    
    self->needDestroy_ = true;
}

-(void)handleNSException:(NSException*)exception
{
    [self handleException:toString(exception.reason)];
}

-(void)createDisplayLink
{
    //[self attachRender:mangl_framework_->render_];
    
    void (^assertCVResult)(CVReturn, StrView text) = ^(CVReturn ret, StrView text) {
        if (ret == kCVReturnSuccess)
            return;
        apl::throwError("%d:%s", (int)ret, text);
        //MDebugFunc("%d:%d", line, ret);
    };
    
#if MANGL_RENDER_ON_MAIN_THREAD
    dispatchSource_ = dispatch_source_create(DISPATCH_SOURCE_TYPE_DATA_ADD, 0, 0, dispatch_get_main_queue());
    __weak auto weakSelf = self;
    dispatch_source_set_event_handler(dispatchSource_, ^(){
        @autoreleasepool {
            [weakSelf render];
        }
    });
    dispatch_resume(dispatchSource_);
#endif

    CVReturn ret{};
    
    ret = CVDisplayLinkCreateWithActiveCGDisplays(&displayLink_);
    assertCVResult(ret, "CVDisplayLinkCreateWithActiveCGDisplays");

#if MANGL_RENDER_ON_MAIN_THREAD
    ret = CVDisplayLinkSetOutputCallback(displayLink_, &displayLinkCallback, (__bridge void *)dispatchSource_);
    assertCVResult(ret, "CVDisplayLinkSetOutputCallback");
#else
    ret = CVDisplayLinkSetOutputCallback(displayLink_, &displayLinkCallback, (__bridge void *)self);
    assertCVResult(ret, "CVDisplayLinkSetOutputCallback");
#endif
    
    auto viewDisplayID = (CGDirectDisplayID)[self.window.screen.deviceDescription[@"NSScreenNumber"] unsignedIntegerValue];
   
    ret = CVDisplayLinkSetCurrentCGDisplay(displayLink_, viewDisplayID);
    assertCVResult(ret, "CVDisplayLinkSetCurrentCGDisplay");

    
    needInit_ = true;
    
    ret = CVDisplayLinkStart(displayLink_);
    assertCVResult(ret, "CVDisplayLinkStart");
    
    displayLinkCreated_ = true;
}

-(void)onLaunchCore
{
    fw_ = MFrameworkNative::instance();
    fw_->onStart();
    renderer_ = fw_->getRenderer();
    //renderer_->setViewport(Env::applicationRect.subframe());
    
    if (!displayLinkCreated_) {
        [self createDisplayLink];
    }
    //mangl_framework_->getRenderer()->setViewport({0, 0, Env::applicationFrame.w, Env::applicationFrame.h});
    
    //[self createOpenGLContext] ;
}

-(void)onLaunch
{
    MANGL_CALLBACK_BEGIN
    [self onLaunchCore];
    MANGL_CALLBACK_END
}

-(void)onViewport {
    needViewport_ = true ;
}

-(void)onResumeTextures
{
    //mangl_opengl::Lock renderLock(mangl_opengl::renderMutex_) ;
    //mangl_framework_->render_->pauseTextures() ;
    //need_resume_textures_ = true ;
}

-(void)onPause {
    LockGuard lock{getMutex()};
    mangl_framework_->onPause();
    //needTimerStop_ = true ;
}

-(void)onResume {
    LockGuard lock{getMutex()};
    needTimerStop_ = false;
    mangl_framework_->onResume();
}

-(void)pauseRefresh {
    needTimerStop_ = true;
}

-(void)resumeRefresh {
    needTimerStop_ = false;
}

- (void)onTerminate
{
    LockGuard lock{getMutex()};

    needDestroy_ = true;
    CVDisplayLinkStop(displayLink_);
    //[self releaseOpenGLContext];

    fw_->onTerminate();
    fw_->getRenderer()->release();
}

//- (void)onBoundsDidChange:(NSNotification*)notification {
//    [self update];
//}

//- (void)update {
//}

// MARK: Util
- (MPoint)eventToTouchPoint:(NSEvent *)event
{
    auto ep = event.locationInWindow;
    auto f = self.frame;
    ep.x -= f.origin.x;
    ep.y -= f.origin.y;
    return Env::fromScreen(MPoint{ep.x, ep.y});
}


// MARK: Frame rendering

- (void)getFrameForTimeCore
{
    if (needInit_) {
        LockGuard lock{getMutex()};
        
        needInit_ = false;
        
        renderer_->pauseTextures();
        needResumeTextures_ = true;
        
        fw_->onInitialized();
    }
    
    if (needViewport_) {
        needViewport_ = false;
        [self updateViewport];
        fw_->onViewport();
    }
    
    
    if (needResumeTextures_) {
        LockGuard lock{getMutex()};
        
        renderer_->resumeTextures();
        needResumeTextures_ = false;
    }
    
    
    LockGuard lock{getMutex()};
    fw_->processTimer(timestamp_);
    renderer_->onTimer(timestamp_);
    renderer_->prepareScene();
    renderer_->onRender();
    
    [self renderScene];
}

- (CVReturn)getFrameForTime:(Timestamp)timestamp
{
    timestamp_ = timestamp;

    if (needDestroy_)
        return kCVReturnSuccess;

    if (needTimerStop_)
        return kCVReturnSuccess;
    
    MANGL_CALLBACK_BEGIN
    [self getFrameForTimeCore];
    MANGL_CALLBACK_END
    
    return kCVReturnSuccess;
}

// MARK: Mouse
- (void)mouseDown:(NSEvent *)event
{
    MANGL_CALLBACK_BEGIN
    TouchesInfo ti{.count = 1};
    
    ti.touches[0].p = [self eventToTouchPoint:event];
    ti.touches[0].timestamp = timestamp_;
    
    LockGuard lock{getMutex()};
    mangl_framework_->onTouchesBegin(ti);
    MANGL_CALLBACK_END
}

- (void)mouseUp:(NSEvent *)event
{
    MANGL_CALLBACK_BEGIN
    TouchesInfo ti{.count = 1};
    
    ti.touches[0].p = [self eventToTouchPoint:event];
    ti.touches[0].timestamp = timestamp_;
    
    LockGuard lock{getMutex()};
    mangl_framework_->onTouchesEnd(ti);
    MANGL_CALLBACK_END
}

- (void)mouseDragged:(NSEvent *)event
{
    MANGL_CALLBACK_BEGIN
    TouchesInfo ti{.count = 1};
    
    ti.touches[0].p = [self eventToTouchPoint:event];
    ti.touches[0].timestamp = timestamp_;
    
    LockGuard lock{getMutex()};
    mangl_framework_->onTouchesMove(ti);
    MANGL_CALLBACK_END
}

@end

