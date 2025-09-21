#include "../core/audio/mangl_audio_system.h"

@import CoreFoundation;

namespace mangl {

//void AudioSystem::initializePlatform()
//{
//}

void AudioSystem::destroyPlatform()
{

}

void AudioSystem::threadPlatformInit()
{
}

void AudioSystem::threadPlatformDestroy()
{
}

void AudioSystem::threadPlatformTimer(Timestamp)
{
//    CFRunLoopRun();
    auto result = CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, true);
    //    auto result = CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.01, true);
//    auto result = CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.01, false);
}


}  // namespace mangl


