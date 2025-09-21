#include "../core/mangl_common.h"

#import "mangl_main_app.h"

#import "../apple/mangl_imacos_util.h"

#if MANGL_FIREBASE
@import FirebaseCore;
@import FirebaseCrashlytics;
#endif


#include <stdexcept>

int main(int argc, char *argv[])
{
    int retCode = 1;

    @autoreleasepool {
        @try {
            try {
                auto mainAppDelegate = NSStringFromClass(ManglMainApplicationDelegate.class);
                retCode = UIApplicationMain(argc, argv, nil, mainAppDelegate);
            } catch (const std::exception &e) {
                MDebugFunc("%s", e.what());
                @throw [NSException exceptionWithName:@"Main exception" 
                                               reason:mangl::toNSString(e.what())
                                             userInfo:nil];
            } catch (...) {
                MDebugFunc("Unhandled Exception!");
                @throw [NSException exceptionWithName:@"Main exception" 
                                               reason:@"Unhandled Exception!"
                                             userInfo:nil];
            }
        } @catch (NSException *e) {
            DLog(@"Exception:%@:%@", e.name, e.reason);
            DLog(@"Stack trace: %@", [e callStackSymbols]);
            
#if MANGL_FIREBASE
            NSError* error = [NSError errorWithDomain:e.name 
                                                 code:0
                                             userInfo:@{
                NSUnderlyingErrorKey: e,
                NSDebugDescriptionErrorKey: e.userInfo ?: @{ },
                NSLocalizedFailureReasonErrorKey: (e.reason ?: @"???") 
            }
            ];
        
            [FIRCrashlytics.crashlytics recordError:error];
#endif

            @throw e;  // rethrow, let it crash
        }
    }

    return retCode;
}
