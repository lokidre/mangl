/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_sound_openal.h"

#include "mangl_audio_file.h"

#include "../mangl_common.h"

#include <apeal/buffer.hpp>
#include <apeal/file.hpp>

#if MANGL_AUDIO_OPENAL

//#if APEAL_OS_MAC
//#include <AudioToolbox/AudioToolbox.h>
//#include <CoreFoundation/CoreFoundation.h>
//#endif

namespace mangl {

#if _DEBUG

static void alErrorAssertBody(ALCdevice* device, std::string_view file, int line) {
    auto errorCode = alGetError();
    auto lastErrorCode = errorCode;
    while (errorCode != AL_NO_ERROR) {
        auto errorString = alcGetString(device, errorCode);
        MDebugPrint(
            "OpenAL Error: %s : %d : %d (0x%04X) : %s", apl::extractFileName(file), line, errorCode,
            errorCode, errorString
        );
        errorCode = alGetError();
        if (errorCode == lastErrorCode)
            break;
        lastErrorCode = errorCode;
    }
}

#define alErrorAssert() alErrorAssertBody(device_, __FILE__, __LINE__)

#else

#define alErrorAssert() (void(0))

#endif


void OpenALEngine::initialize() {
    if (device_)
        return;

#if !defined(__EMSCRIPTEN__) and !defined(APEAL_OS_MAC)
    alutInit(nullptr, nullptr);
#endif

    alGetError(); // clear errors

    device_ = alcOpenDevice(nullptr);
    alErrorAssert();

    if (!device_) {
        MDebugFunc("Cannot open OpenAL device");
        return;
    }
}

void OpenALEngine::destroy() {
    if (!device_)
        return;
    alcCloseDevice(device_);
    device_ = nullptr;
}

void OpenALEngine::shutdownContext() {
    alcMakeContextCurrent(nullptr);
    alcSuspendContext(context_);
}

void OpenALEngine::restoreContext() {
    alcMakeContextCurrent(context_);
    alcProcessContext(context_);
}


bool OpenALEngine::load(CtxP& ctx, StrView filename) {
    if (!device_)
        initialize();

    auto ectx = makeEngineContext(ctx);


    if (!context_) {
        context_ = alcCreateContext(device_, nullptr);
        alErrorAssert();
        alcMakeContextCurrent(context_);
        alErrorAssert();
    }

     ALenum alFormat{};
     ALsizei alFreq{};
    
     AudioFileLoader afl;
     afl.load(filename);
    
     if (afl.numChannels == 1) {
         if (8 == afl.bitsPerSample) {
             alFormat = AL_FORMAT_MONO8;
         } else { // assume 16
             alFormat = AL_FORMAT_MONO16;
         }
     } else {
         if (8 == afl.bitsPerSample) {
             alFormat = AL_FORMAT_STEREO8;
         } else { // assume 16
             alFormat = AL_FORMAT_STEREO16;
         }
     }

     alFreq = afl.sampleRate;


//#if APEAL_OS_MAC
//
//    UInt32 bufferSize{};
//
//    OSStatus failed{};
//    AudioFileID fileHandle{};
//
//    CFURLRef url = CFURLCreateFromFileSystemRepresentation(kCFAllocatorDefault, (UInt8*)filename.data(), (CFIndex)filename.size(), false);
//    failed = AudioFileOpenURL(url, kAudioFileReadPermission, 0, &fileHandle);
//    CFRelease(url);
//
//    if (failed) {
//        MDebugFunc("Error: %s", filename);
//        return false;
//    }
//
//    // read file properties
//    UInt64 audioDataSize{};
//    UInt32 paramSize = sizeof(audioDataSize);
//    failed = AudioFileGetProperty(fileHandle, kAudioFilePropertyAudioDataByteCount, &paramSize, &audioDataSize);
//
//    if (failed) {
//        MDebugFunc("Error::AudioFileGetProperty");
//        return false;
//    }
//
//    AudioStreamBasicDescription fileFormat{};
//    paramSize = sizeof(fileFormat);
//
//    failed = AudioFileGetProperty(fileHandle, kAudioFilePropertyDataFormat, &paramSize, &fileFormat);
//
//    if (failed) {
//        MDebugFunc("Error::AudioFileGetProperty");
//        return false;
//    }
//
//    if (1 == fileFormat.mChannelsPerFrame) {
//        if (8 == fileFormat.mBitsPerChannel) {
//            alFormat = AL_FORMAT_MONO8;
//        } else { // assume 2
//            alFormat = AL_FORMAT_MONO16;
//        }
//    } else { // assume 2
//        if (8 == fileFormat.mBitsPerChannel) {
//            alFormat = AL_FORMAT_STEREO8;
//        } else { // assume 16
//            alFormat = AL_FORMAT_STEREO16;
//        }
//    }
//
//    alFreq = fileFormat.mSampleRate;
//
//    bufferSize = (UInt32)audioDataSize;
//
//    bufferData.alloc(bufferSize);
//
//    failed = AudioFileReadBytes(fileHandle, false, 0, &bufferSize, bufferData.data());
//
//    if (failed) {
//        MDebugFunc("Error::AudioFileReadBytes");
//        return false;
//    }
//
//    AudioFileClose(fileHandle);
//#else
//
//#if !defined(__EMSCRIPTEN__)
//    ectx->bufferId = alutCreateBufferFromFile(String{filename}.c_str());
//    if (ectx->bufferId == AL_NONE) {
//        auto errorCode = alutGetError();
//        MDebugFunc("ALUT error:%s:%s", filename, alutGetErrorString(errorCode));
//        return false;
//    }
//#endif  // emscripten
//
//#endif


#if APEAL_OS_MAC
    alGenBuffers(1, &ectx->bufferId);
    alErrorAssert();
    alBufferData(ectx->bufferId, alFormat, afl.buffer.data(), afl.buffer.size(), alFreq);
    alErrorAssert();
#endif

    alGenSources(1, &ectx->sourceId);
    alErrorAssert();
    alSourcei(ectx->sourceId, AL_BUFFER, ectx->bufferId);
    alErrorAssert();


    alSourcef(ectx->sourceId, AL_PITCH, 1);
    // alSourcef ( ectx->sourceId, AL_GAIN, 1 ) ;
    alSourcei(ectx->sourceId, AL_LOOPING, AL_FALSE);

    updateVolume(ectx, volume_);

    return true;
}

void OpenALEngine::updateVolume(EctxP ectx, Real volume) {
    auto avol = static_cast<decltype(ectx->volume)>(volume);
    if (ectx->volume != avol) {
        ectx->volume = avol;
        // Some attempt to convert volume to gain
        auto gain = std::pow(10.0f, ectx->volume / 3.321f) - 1.0f;
        ectx->gain = std::clamp(gain, 0.0f, 1.0f);
        alSourcef(ectx->sourceId, AL_GAIN, ectx->gain);
    }
}


bool OpenALEngine::play(CtxP& ctx, Real volume) {
    auto ectx = getEngineContext(ctx);

    if (!context_) {
        context_ = alcCreateContext(device_, nullptr);
        alErrorAssert();
        alcMakeContextCurrent(context_);
        alErrorAssert();
    }

    updateVolume(ectx, volume);
    alSourcePlay(ectx->sourceId);

    alErrorAssert();

    // #if _DEBUG
    //	if(alGetError() != AL_NO_ERROR)
    //		return false ;
    // #endif

    return true;
}

} // namespace mangl

#endif // OpenAL
