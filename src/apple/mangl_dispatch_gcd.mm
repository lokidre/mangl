#include "../core/mangl_dispatch.h"

@import Foundation;

namespace mangl {

void Dispatch::asyncMain(DispatchBlock block) {
    
    dispatch_async(dispatch_get_main_queue(), ^{
        block();
    });
    
}

void Dispatch::asyncMainDelayed(DispatchBlock block, Timestamp when) {
    auto nsec = static_cast<int64_t>(when * NSEC_PER_SEC);
    
    dispatch_time_t delay = dispatch_time(DISPATCH_TIME_NOW, nsec);
    
    dispatch_after(delay, dispatch_get_main_queue(), ^{
        block();
    });
}


}  // namespace mangl

