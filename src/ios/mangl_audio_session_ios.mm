#include "../core/audio/mangl_audio_system.h"

#include "../apple/mangl_imacos_util.h"
#include "../core/mangl_debug.h"


@import AVFoundation;
@import Foundation;


@interface ManglAVSessionHandler: NSObject

+ (instancetype)instance;
@end

@implementation ManglAVSessionHandler

using namespace mangl;


+ (instancetype)instance {
    static ManglAVSessionHandler *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[ManglAVSessionHandler alloc] init];
    });
    return sharedInstance;
}


- (void)onChangeInterruptionType:(NSNotification *)notification
{
    NSNumber *interruptionType = [notification.userInfo objectForKey:AVAudioSessionInterruptionTypeKey];
    NSNumber *interruptionOption = [notification.userInfo objectForKey:AVAudioSessionInterruptionOptionKey];

    switch (interruptionType.unsignedIntegerValue) {
    case AVAudioSessionInterruptionTypeBegan: 
        AudioSystem::interruptionActive_ = true;
        break;

    case AVAudioSessionInterruptionTypeEnded:
        AudioSystem::interruptionActive_ = false;
        if (interruptionOption.unsignedIntegerValue == AVAudioSessionInterruptionOptionShouldResume) {
            // should resume the play
        }
        break;
    default: break;
    }
}

- (void)onMediaServicesReset:(NSNotification *)notification
{
}

@end

namespace mangl {

void AudioSystem::initializePlatform()
{
    auto handler = ManglAVSessionHandler.instance;
    auto sess = AVAudioSession.sharedInstance;

    BOOL result{};
    NSError *error{};

    result = [sess setCategory:AVAudioSessionCategoryAmbient error:&error];
    if (!result) {
        MDebugFunc("%s", error);
    }
    
    result = [sess setMode:AVAudioSessionModeDefault error:&error];
    if (!result) {
        MDebugFunc("%s", error);
    }

    result = [sess setActive:YES error:&error];
    if (!result) {
        MDebugFunc("%s", error);
    }

    auto nc = NSNotificationCenter.defaultCenter;

    [nc addObserver:handler 
           selector:@selector(onChangeInterruptionType:)
               name:AVAudioSessionInterruptionNotification
             object:sess];
    
    [nc addObserver:handler 
           selector:@selector(onMediaServicesReset:)
               name:AVAudioSessionMediaServicesWereResetNotification
             object:sess];
}

bool AudioSystem::detectOtherAudioPlaying() {
    return [AVAudioSession.sharedInstance isOtherAudioPlaying];
}

}   // namespace mangl
