#include "../core/mangl_analytics.h"

#include "../apple/mangl_imacos_util.h"


#if MANGL_FIREBASE

@import FirebaseAnalytics;

namespace mangl {

void Analytics::log(StrView name, const Params& params) {
    @autoreleasepool {
        auto nsname = toNSString(name);

        auto dict = NSMutableDictionary.dictionary;

        for (auto& p: params) {
            if (p.second.empty())
                continue;
            [dict setObject:toNSString(p.second) forKey:toNSString(p.first)];
        }

        [FIRAnalytics logEventWithName:nsname parameters:dict];
    }
}

}  // namespace mangl

#endif  // If MANGL_FIREBASE
