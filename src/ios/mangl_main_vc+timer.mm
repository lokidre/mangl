#import "mangl_main_vc+timer.h"

#import "mangl_main_vc+except.h"

#import "mangl_main_app.h"

#include "mangl_ios_static.h"

#include "../core/mangl_common.h"
#include "../core/mangl_framework.h"


@import QuartzCore;

#include <mach/mach.h>
#include <mach/mach_time.h>


static mangl::Framework *fw_{};

static std::uint64_t zeroTimestamp_{};
static struct mach_timebase_info statsTimebase_{};

#if MANGL_DEBUG_STATS_FPS
static mangl::Timestamp debugStatsLastTimestamp_{};
static std::uint64_t debugStatsTimerBegin_{}, debugStatsTimerRuntime_{};
static std::uint64_t debugStatsTimerScene_{}, debugStatsTimerRender_{}, debugStatsTimerEnd_{};
#endif


@implementation ManglMainViewController (Timer)

- (void)onTimerInit
{
    using namespace mangl;
    
    mach_timebase_info(&statsTimebase_);
    zeroTimestamp_ = mach_absolute_time();

    impl_::timerJustEnabled_ = false;
    
#ifdef MANGL_FPS
    fps_ = MANGL_FPS;
#else
    fps_ = 60;
#endif
    
//    fps_ = 30;
#if MANGL_IOS_FPS_60
    fps_ = 60;
#endif
    
    fpsRecip_ = 1.0 / fps_;
    impl_::currentTimestamp_ = 0;

    self.displayLink = nil;

#if MANGL_IOS_TIMER_THREADED
    timerThread_ = [[NSThread alloc] initWithTarget:self selector:@selector(runtimeThreadBody:) object:nil];
#endif
}

- (void)onDisplayLink:(id)sender
{
    using namespace mangl;
    using namespace impl_;

    @try { try {
        
        if (timerEnabled_) {
#if MANGL_DEBUG_STATS_FPS
            debugStatsTimerBegin_ = mach_absolute_time();
#endif
            fw_->processTimer(currentTimestamp_);
            
            
#if MANGL_DEBUG_STATS_FPS
            debugStatsTimerRuntime_ = mach_absolute_time();
#endif
            fw_->render_->onTimer(currentTimestamp_);
        }
        

#if MANGL_DEBUG_STATS_FPS
        debugStatsTimerScene_ = mach_absolute_time();
#endif
        fw_->render_->prepareScene();
        fw_->render_->onRender();

        [self.renderView renderScene];

#if MANGL_DEBUG_STATS_FPS
        debugStatsTimerRender_ = mach_absolute_time();
        debugStatsTimerEnd_ = debugStatsTimerRender_;
#endif

        if (timerEnabled_) {
            
//            displayLinkTimestamp_ = self.displayLink.timestamp;
            
#if MANGL_DEBUG_STATS_FPS
            if (auto elapsed = currentTimestamp_ - debugStatsLastTimestamp_; elapsed > 0.00001) {
                auto &st = fw_->debugStats;
                
                st.fps_ = st.addData(st.fpsBuffer_, 1 / elapsed);
                
                uint64_t totalTimeInt = (debugStatsTimerEnd_ - debugStatsTimerBegin_) * statsTimebase_.numer / statsTimebase_.denom;
                uint64_t runtimeTimeInt = (debugStatsTimerRuntime_ - debugStatsTimerBegin_) * statsTimebase_.numer / statsTimebase_.denom;
                uint64_t sceneTimeInt = (debugStatsTimerScene_ - debugStatsTimerRuntime_) * statsTimebase_.numer / statsTimebase_.denom;
                uint64_t renderTimeInt = (debugStatsTimerRender_ - debugStatsTimerScene_) * statsTimebase_.numer / statsTimebase_.denom;
                
                double loadValue = double(totalTimeInt) * (100. / 1000000000.) / elapsed;
                
                st.load_ = st.addData(st.loadBuffer_, loadValue);
                
                st.runtimeLoad_ = st.addData(st.runtimeLoadBuffer_, double(runtimeTimeInt) / 1000000000.);
                st.sceneLoad_ = st.addData(st.sceneLoadBuffer_, double(sceneTimeInt) / 1000000000.);
                st.renderLoad_ = st.addData(st.renderLoadBuffer_, double(renderTimeInt) / 1000000000.);
            }
            
            debugStatsLastTimestamp_ = currentTimestamp_;
#endif
            
            displayLinkTimestamp_ = self.displayLink.timestamp;
            auto delta = displayLinkTimestamp_ - displayLinkLastTimestamp_;
            currentTimestamp_ += delta;
            displayLinkLastTimestamp_ = displayLinkTimestamp_;
        }
        
        if (timerJustEnabled_) {
            timerJustEnabled_ = false;
            timerEnabled_ = true;
            displayLinkTimestamp_ = self.displayLink.timestamp;
            displayLinkLastTimestamp_ = displayLinkTimestamp_;
        }


    } catch (const std::exception& e) {
        [self handleCriticalExceptionStdExcept:e];
    }} @catch (NSException *e) {
        [self handleCriticalExceptionNSException:e];
    } @catch (...) {
        [self handleCriticalExceptionUnhandled];
    }
}


- (void)startTimer {
    using namespace mangl;
    
    MDebugFunc();
    
    fw_ = Framework::instance();

    if (!self.displayLink) {
        self.displayLink = [CADisplayLink displayLinkWithTarget:self
                                                       selector:@selector(onDisplayLink:)];
        self.displayLink.preferredFramesPerSecond = fps_;
        [self.displayLink addToRunLoop:NSRunLoop.currentRunLoop 
                               forMode:NSDefaultRunLoopMode];
        
        
        
    }

}

- (void)killTimer {
    using namespace mangl;

    MDebugFunc();
    
    if (self.displayLink) {
        [self.displayLink removeFromRunLoop:NSRunLoop.currentRunLoop 
                                    forMode:NSDefaultRunLoopMode];
        [self.displayLink invalidate];
        self.displayLink = nil;
    }
}


@end
