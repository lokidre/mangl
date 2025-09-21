/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_music_audiounit.h"

#include "../core/mangl_common.h"
#include "../core/mangl_analytics.h"

#include "../core/audio/mangl_music.h"

#if MANGL_MUSIC_AUDIOUNIT

#include <AudioToolbox/AudioFile.h>
#include <AudioToolbox/AudioToolbox.h>

//#include <AudioToolbox/AUGraph.h>
#include <AudioToolbox/AUComponent.h>


namespace mangl {

static std::pair<OSStatus, const char *> audioQueueErrorCodes_[] = {
    {kAudioFileUnspecifiedError,               "UnspecifiedError"},
    {kAudioFileUnsupportedFileTypeError,       "UnsupportedFileTypeError"},
    {kAudioFileUnsupportedDataFormatError,     "UnsupportedDataFormatError"},
    {kAudioFileUnsupportedPropertyError,       "UnsupportedPropertyError"},
    {kAudioFileBadPropertySizeError,           "BadPropertySizeError"},
    {kAudioFilePermissionsError,               "PermissionsError"},
    {kAudioFileNotOptimizedError,              "NotOptimizedError"},
    {kAudioFileInvalidChunkError,              "InvalidChunkError"},
    {kAudioFileDoesNotAllow64BitDataSizeError, "DoesNotAllow64BitDataSizeError" },
    {kAudioFileInvalidPacketOffsetError,       "InvalidPacketOffsetError" },
    {kAudioFileInvalidFileError,               "InvalidFileError" },
    {kAudioFileOperationNotSupportedError,     "OperationNotSupportedError" },
    {kAudioFileNotOpenError,                   "NotOpenError" },
    {kAudioFileEndOfFileError,                 "EndOfFileError" },
    {kAudioFilePositionError,                  "PositionError" },
    {kAudioFileFileNotFoundError,              "FileNotFoundError" },
};

const char *MusicEngineDevice::audioQueueErrorString(OSStatus result) {
    // Do it slow, but save memory. We don't load the map
    for (auto &ec: audioQueueErrorCodes_)
        if (ec.first == result)
            return ec.second;

    return "AudiodQueue::Unknown Error";
}


void MusicEngineDevice::handleAudioQueueWarnings(OSStatus result, int line) {
    if (result == 0)
        return;

#if _DEBUG
    String body;
    static int messageCount = 0;
    if (messageCount >= 10)
        return;
    ++messageCount;

    auto errorString = audioQueueErrorString(result);

    MDebugPrint("AudioQueue::%d:%s:%d: %s",
                line,
                apl::extractFileName(currentTrack_->filename),
                int(result),
                errorString);

    if (messageCount == 10)
        MDebugPrint("AudioQueue: Too many errors, supressing...");
#endif
}





#if _DEBUG
#define handleResult(_Result) handleAudioQueueWarnings(_Result, __LINE__)
#else
#define handleResult    ADebugNoop
#endif



struct EngineContext {
    AudioStreamBasicDescription audioDesc;
    AudioFileID audioFileId;
  
};

static AudioUnit defaultOutputAudioUnit{};

using EngineContextP = EngineContext*;

static auto getEngineContext(MusicTrackContextPtr &ctx) {
    return static_cast<EngineContextP>(ctx->engineContext.get());
}

static auto makeEngineContext(MusicTrackContextPtr &ctx) {
    ctx->engineContext = std::make_unique<EngineContextP>();
    return getEngineContext(ctx);
}


void MusicEngineDevice::play(MusicTrackContextPtr track) {
    paused_ = false;
    stopped_ = false;


//    // Check if the audio queue is still busy
//    // we just schedule the track
//    if (isQueueRunning()) {
//        playScheduled_ = 0.5;  // check every half second
//        scheduledTrack_ = track;
//        return ;
//    }

    playTrackBody(track);
}

void MusicEngineDevice::stop() {
    if (!defaultOutputAudioUnit)
        return;
    
    if (!stopped_) {
        
        stopped_ = true;
        
        
        AudioOutputUnitStop(defaultOutputAudioUnit);
    }
    
    if (!currentTrack_)
       return;

}

void MusicEngineDevice::pause() {
 
    if (defaultOutputAudioUnit)
        AudioOutputUnitStop(defaultOutputAudioUnit);
    
    paused_ = true;
}

void MusicEngineDevice::resume() {
    paused_ = false;
    
    if (defaultOutputAudioUnit)
        AudioOutputUnitStart(defaultOutputAudioUnit);
}

void MusicEngineDevice::setVolume(Real volume, Real ramp) {
    //kAudioUnitParameterUnit_LinearGain
//    kAudioUnitScope_Global
}

bool MusicEngineDevice::playing() {
    return isRunning();
}

void MusicEngineDevice::playTrackBody(MusicTrackContextPtr track) {

    currentTrack_ = track;
    OSStatus result{};
    
    auto ectx = getEngineContext(track);

    try {
        CFURLRef url = CFURLCreateFromFileSystemRepresentation(kCFAllocatorDefault,
                                                               (UInt8 *)currentTrack_->filename.data(),
                                                               (CFIndex)currentTrack_->filename.size(),
                                                               false);
        result = AudioFileOpenURL(url, kAudioFileReadPermission, 0, &ectx->audioFileId);
        CFRelease(url);

        assertResult(result, "Cannot Opeen Music Track: %s", currentTrack_->filename);
        
        AudioStreamBasicDescription *af = &ectx->audioDesc;
        UInt32 propSize = sizeof(*af);
        
        
//        AudioFileGetPropertyInfo(iAudioFileHandler,kAudioFilePropertyDataFormat,&propertyValueSize,&isWritable);
        

        result = AudioFileGetProperty(ectx->audioFileId, kAudioFilePropertyDataFormat, &propSize, af);
        assertResult(result, "Can't read file format");
        
        
        AudioUnit fileAudioUnit;

        AudioComponentDescription filePlayerAudioComponentDescription{};
        
        filePlayerAudioComponentDescription.componentType = kAudioUnitType_Generator;
        filePlayerAudioComponentDescription.componentSubType = kAudioUnitSubType_AudioFilePlayer;
        filePlayerAudioComponentDescription.componentManufacturer = kAudioUnitManufacturer_Apple;
        
        
        AudioComponent fileAudioComponent = AudioComponentFindNext(NULL, &filePlayerAudioComponentDescription);
        if (!fileAudioComponent) {
//            fprintf(stderr, "Cannot get the audio unit matching given description\n");
//            exit(1);
        }
        
        
        result = AudioComponentInstanceNew(fileAudioComponent, &fileAudioUnit);
        assertResult(result, "AudioComponentInstanceNew");
        
        result = AudioUnitInitialize(fileAudioUnit);
        assertResult(result, "AudioUnitInitialize");
        
        
       
        
        AudioComponentDescription outputDefaultDescription{};
        outputDefaultDescription.componentType = kAudioUnitType_Output;
        outputDefaultDescription.componentSubType = kAudioUnitSubType_GenericOutput;
        outputDefaultDescription.componentManufacturer = kAudioUnitManufacturer_Apple;

        auto outputAudioComponent = AudioComponentFindNext(NULL, &outputDefaultDescription);
        if (!outputAudioComponent) {
//            fprintf(stderr, "Cannot get the audio unit matching given description\n");
//            exit(1);
        }
        
        result = AudioComponentInstanceNew(outputAudioComponent, &defaultOutputAudioUnit);
        assertResult(result, "AudioComponentInstanceNew");
        
        result = AudioUnitInitialize(defaultOutputAudioUnit);
        assertResult(result, "AudioUnitInitialize");

        
        AudioUnitConnection connection{};
        connection.destInputNumber = 0;
        connection.sourceAudioUnit = fileAudioUnit;
        connection.sourceOutputNumber = 0;
        
        result = AudioUnitSetProperty(defaultOutputAudioUnit,
                                      kAudioUnitProperty_MakeConnection,
                                      kAudioUnitScope_Input,
                                      0,
                                      &connection,
                                      sizeof(AudioUnitConnection)),



        result = AudioOutputUnitStart(defaultOutputAudioUnit);
        assertResult(result, "AudioOutputUnitStart");

    }
    catch(const std::exception& e) {
        MDebugFunc("%s", e.what());
        MAnalyticsException(e.what());
    }
    catch(...) {
        MDebugFunc("Exception!");
    }
}

void MusicEngineDevice::onTimer(Timestamp timestamp)
{
    // Scheduled start playing
    if (playScheduled_) {
        if (isRunning()) {
            playScheduled_ = 0.5;  // check every half second
        } else {
            if (!stopped_ && !paused_) {
                playTrackBody(scheduledTrack_);
                scheduledTrack_ = 0;
            }
        }
    }

    // Scheduled volume change
    if (volumeScheduled_) {
        setVolume(scheduledVolume_, scheduledRamp_);
    }

    // check if we are done with the queue
    if (queueDisposeScheduled_) {
        if (isRunning()) {
            // Wait a little longer
            queueDisposeScheduled_ = 0.3;
        } else {
//            disposeQueue();
//            closeFile();

              // Notify about the track
            Music::trackFinishedTrigger = true;
        }
    }

}

bool MusicEngineDevice::isRunning() {
    
    OSStatus status{};
    
    UInt32 propertyValueSize{sizeof(UInt32)};
    Boolean isWritable{};
    status = AudioUnitGetPropertyInfo(defaultOutputAudioUnit,
                                      kAudioOutputUnitProperty_IsRunning,
                                      kAudioUnitScope_Output,
                                      0,
                                      &propertyValueSize,
                                      &isWritable);
    
    
    UInt32 value{};
    status = AudioUnitGetProperty(defaultOutputAudioUnit,
                                  kAudioOutputUnitProperty_IsRunning,
                                  kAudioUnitScope_Output,
                                  0,
                                  &value,
                                  &propertyValueSize);


    return value != 0 ? true : false;
}


}  // namespace mangl

#endif

