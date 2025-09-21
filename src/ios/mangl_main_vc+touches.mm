#import "mangl_main_vc+touches.h"

#import "mangl_main_vc+except.h"

#import "mangl_main_app.h"

#include "../core/mangl_common.h"
#include "../core/mangl_framework.h"

//#undef  DebugPrintFunc
//#define DebugPrintFunc DebugNoop

#undef MDebugFunc
#define MDebugFunc MDebugNoop



using namespace mangl;

namespace mangl_impl_ {

inline static mangl::Point cgpToPoint(const CGPoint& cgp) {
    return Env::fromScreen(mangl::Point{cgp.x, cgp.y});
}

struct TouchStatus {
    bool down;  // If this touch is down already
    bool sent;  // If the notification about this touch was sent
    Timestamp timestamp;
    CGPoint cgpDown, cgpMove, cgpUp;
    mangl::Point pos;
    mangl::Point lastPos;
    
    UITouch* touchPtr;
    
    void setDown(UITouch* touch) {
        auto cgp = [touch locationInView:touch.view];
        auto p = cgpToPoint(cgp);
        
        touchPtr = touch;
        pos = p;
        timestamp = touch.timestamp;

        down = true;
        sent = true;
        cgpDown = cgp;
        lastPos = p;
    }

    void setMove(UITouch* touch) {
        auto cgp = [touch locationInView:touch.view];
        auto p = cgpToPoint(cgp);
        
        pos = p;
        timestamp = touch.timestamp;

        sent = true;
        lastPos = p;
    }

    void setUp(UITouch* touch) {
        auto cgp = [touch locationInView:touch.view];
        auto p = cgpToPoint(cgp);

        touchPtr = nullptr;
        pos = p;
        timestamp = touch.timestamp;

        touchPtr = nullptr;
        down = false;
    }
};

static std::array<TouchStatus, MaxTouches> touchStatuses_{};
constexpr static int touchStatusesCount_ = MaxTouches;


}  // namespace mangl_impl

using namespace mangl_impl_;


@implementation ManglMainViewController (Touches)

- (void)onTouchesInit
{
}

// UIViewController
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    MDebugFunc("c: %d, ca: %d", touches.count, event.allTouches.count);

    if (!applicationActivated_ || event.type != UIEventTypeTouches) {
//        [super touchesBegan:touches withEvent:event];
        return;
    }

    int touchesCount = event.allTouches.count;

    for (UITouch *touch in touches) {
        for (int i = 0; i < touchStatusesCount_; ++i) {
            if (!touchStatuses_[i].down) {
                auto& ti = touchStatuses_[i];
                ti.setDown(touch);

                Framework::instance()->onTouchDown({
                    .p = ti.pos,
                    .timestamp = ti.timestamp,
                    .index = i,
                    .count = touchesCount,
                });
                break;
            }
        }
    }  // For touches
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    MDebugFunc("c: %d, ca: %d", touches.count, event.allTouches.count);
    
    if (!applicationActivated_ || event.type != UIEventTypeTouches)
        return;

    int touchesCount = event.allTouches.count;
        
    for (UITouch *touch in touches) {
        for (int i = 0; i < touchStatusesCount_; ++i) {
            auto& ti = touchStatuses_[i];
            if (ti.touchPtr == touch) {
                ti.setUp(touch);
                
                Framework::instance()->onTouchUp({
                    .p = ti.pos,
                    .timestamp = touch.timestamp,
                    .index = i,
                    .count = touchesCount,
                });
                break;
            }
        }
    }  // for touches
}



- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
//    MDebugFunc("c: %d", touches.count);
    if (!applicationActivated_ || event.type != UIEventTypeTouches)
        return;

    int touchesCount = event.allTouches.count;

    for (UITouch *touch in touches) {
        for (int i = 0; i < touchStatusesCount_; ++i) {
            auto& ti = touchStatuses_[i];
            if (ti.touchPtr == touch) {
                ti.setMove(touch);
                
                Framework::instance()->onTouchMove({
                    .p = ti.pos,
                    .timestamp = touch.timestamp,
                    .index = i,
                    .count = touchesCount,
                });
                
                break;
            }
        }
    }  // For touches
}


- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    MDebugFunc("c: %d, ca: %d", touches.count, event.allTouches.count);
    
    if (!applicationActivated_ || event.type != UIEventTypeTouches) {
//        [super touchesCancelled:touches withEvent:event];
        return;
    }

    int touchesCount = event.allTouches.count;
    
    for (UITouch* touch in touches) {
        for (int i = 0; i < touchStatusesCount_; ++i) {
            auto& ti = touchStatuses_[i];
            if (ti.touchPtr == touch) {
                ti.setUp(touch);
                
                Framework::instance()->onTouchCancel({
                    .p = ti.pos,
                    .timestamp = touch.timestamp,
                    .index = i,
                    .count = touchesCount,
                });
                break;
            }

        }
    }  // For touch
    
}

- (void)touchesEstimatedPropertiesUpdated:(NSSet<UITouch *> *)touches {
//    [super touchesEstimatedPropertiesUpdated:touches];
}

@end
