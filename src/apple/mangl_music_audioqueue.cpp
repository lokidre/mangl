/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_music_audioqueue.h"

#include "../core/mangl_common.h"
#include "../core/mangl_analytics.h"
#include "../core/mangl_framework.h"

#include "../core/audio/mangl_music.h"
#include "../core/mangl_dispatch.h"

#include <apeal/file.hpp>


#undef  MDebugFunc
#define MDebugFunc  MDebugNoop


#if MANGL_MUSIC_AUDIOQUEUE


#if _DEBUG
#define handleResult(_Result) handleAudioQueueWarnings(_Result, __LINE__)
#else
#define handleResult    ADebugNoop
#endif


namespace mangl {

namespace mangl_impl_ {

constexpr static std::pair<OSStatus, const char *> audioQueueErrorCodes_[] = {
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
    
    {kAudioQueueErr_InvalidBuffer        , "InvalidBuffer" },
    {kAudioQueueErr_BufferEmpty          , "BufferEmpty" },
    {kAudioQueueErr_DisposalPending      , "DisposalPending" },
    {kAudioQueueErr_InvalidProperty      , "InvalidProperty" },
    {kAudioQueueErr_InvalidPropertySize  , "InvalidPropertySize" },
    {kAudioQueueErr_InvalidParameter     , "InvalidParameter" },
    {kAudioQueueErr_CannotStart          , "CannotStart" },
    {kAudioQueueErr_InvalidDevice        , "InvalidDevice" },
    {kAudioQueueErr_BufferInQueue        , "BufferInQueue" },
    {kAudioQueueErr_InvalidRunState      , "InvalidRunState" },
    {kAudioQueueErr_InvalidQueueType     , "InvalidQueueType" },
    {kAudioQueueErr_Permissions          , "Permissions" },
    {kAudioQueueErr_InvalidPropertyValue , "InvalidPropertyValue" },
    {kAudioQueueErr_PrimeTimedOut        , "PrimeTimedOut" },
    {kAudioQueueErr_CodecNotFound        , "CodecNotFound" },
    {kAudioQueueErr_InvalidCodecAccess   , "InvalidCodecAccess" },
    {kAudioQueueErr_QueueInvalidated     , "QueueInvalidated" },
    {kAudioQueueErr_TooManyTaps          , "TooManyTaps" },
    {kAudioQueueErr_InvalidTapContext    , "InvalidTapContext" },
    {kAudioQueueErr_RecordUnderrun       , "RecordUnderrun" },
    {kAudioQueueErr_InvalidTapType       , "InvalidTapType" },
    {kAudioQueueErr_BufferEnqueuedTwice  , "BufferEnqueuedTwice" },
    {kAudioQueueErr_CannotStartYet       , "CannotStartYet" },
    {kAudioQueueErr_EnqueueDuringReset   , "EnqueueDuringReset" },
    {kAudioQueueErr_InvalidOfflineMode   , "InvalidOfflineMode" },
} ;


struct AQPlayerState {
    AudioStreamBasicDescription audioFormat{};
    AudioFileID fileHandle{};
    AudioQueueRef queue{};
    constexpr static int AudioBuffersCount = 3;
    AudioQueueBufferRef buffers[AudioBuffersCount]{};
    bool isRunning{};
    bool stopped{};
    bool runningCheck{};
    SInt64 currentPacket{};
    UInt32 bufferSize{};
    UInt32 packetsReadCount{};
    AudioStreamPacketDescription* packetDesc{};
    apl::Buffer<AudioStreamPacketDescription> packetDescBuffer;
};

static AQPlayerState aqState_{};
static Timestamp currentTimestamp_{};


struct MusicScheduleItemInfo {
    bool scheduled;  // If is active
    
    Timestamp when;  // When to activate
    
    MusicTrackContextP track;
    Real volume;
    Timestamp ramp;
    
    
    void schedule(Timestamp delay) {
//        when = MFrameworkNative::timestampAtomic() + delay;
        when = currentTimestamp_ + delay;
        scheduled = true;
    }
    
    bool check() noexcept {
        if (!scheduled || currentTimestamp_ < when)
            return false;
        scheduled = false;
        return true;
    }
    
    void reset() {
        MusicScheduleItemInfo t{};
        std::swap(*this, t);
    }
};

static MusicScheduleItemInfo triggerQueueDispose_{};

static MusicScheduleItemInfo triggerStopRamp_{};
static MusicScheduleItemInfo triggerStop_{};

static MusicScheduleItemInfo triggerPlay_{};
static MusicScheduleItemInfo triggerPlayVolume_{};

static MusicScheduleItemInfo triggerPause_{};
static MusicScheduleItemInfo triggerResume_{};


static bool stopReported_{false};


// Some attempt to convert volume to gain
static inline Real volumeToGain(Real volume) {
    if (volume == 0)
        return 0;
    if (volume >= 1)
        return 1;
    auto gain = std::pow(10., volume/3.321) - 1;
    if (gain > 1)
        gain = 1;
    return gain;
}


static inline bool success(OSStatus result) {  return result == 0; }



} // namespace mangl_impl_

using namespace mangl_impl_;

const char *MusicEngineAudioQueue::audioQueueErrorString(OSStatus result) {
    // Do it slow, but save memory. We don't load the map
    for (auto &ec: audioQueueErrorCodes_)
        if (ec.first == result)
            return ec.second;

    return "AudiodQueue::Unknown Error";
}

void MusicEngineAudioQueue::handleAudioQueueWarnings(OSStatus result, int line)
{
    if (result == 0)
        return;

#if _DEBUG
    String body;
    static int messageCount = 0;
    if (messageCount >= 10)
        return;
    ++messageCount;

    auto errorString = audioQueueErrorString(result);
    
    auto filename  = currentTrack_ ? apl::extractFileName(currentTrack_->filename) : "";

    MDebugPrint("AudioQueue::%d:%s:%d: %s", line,  filename, int(result),  errorString);

    if (messageCount == 10)
        MDebugPrint("AudioQueue: Too many errors, supressing...");
#endif
}




static void aqBufferCallback(void* data, AudioQueueRef queue, AudioQueueBufferRef buffer) {
    if (!data)
        return;
    static_cast<MusicEngineAudioQueue *>(data)->onAudioQueueOutputBuffer(queue, buffer);
}



void MusicEngineAudioQueue::shutdownContext()
{
    if (isQueueRunning())
        aqPause();
}

void MusicEngineAudioQueue::restoreContext()
{
}


void MusicEngineAudioQueue::shutdown()
{
    if (isQueueRunning())
        aqStop();
}

bool MusicEngineAudioQueue::isQueueRunning()
{
	if (!aqState_.queue)
		return false;
    
    return aqState_.isRunning;
}

void MusicEngineAudioQueue::aqDisposeQueue(OSStatus status)
{
	// release previously allocated queue and buffers
    if (aqState_.queue) {
        if (kAudioQueueErr_QueueInvalidated != status) {
            for (auto& b: aqState_.buffers) {
                if (b) {
                    AudioQueueFreeBuffer(aqState_.queue, b);
                    b = nullptr;
                }
            }
            
            UInt32 isRunning;
            UInt32 propertySize = sizeof(isRunning);

            OSStatus status = AudioQueueGetProperty(aqState_.queue, kAudioQueueProperty_IsRunning, &isRunning, &propertySize);

            // Trying to find crashes
            if (status == 0) {
                if (isRunning) {
                    AudioQueueStop(aqState_.queue, true);
                    AudioQueueFlush(aqState_.queue);
                }
                
                AudioQueueDispose(aqState_.queue, false);
            }
        }
		aqState_.queue = nullptr;
        
		aqState_.stopped = true;
        aqState_.isRunning = false;

		MDebugFunc();
	}
}

void MusicEngineAudioQueue::aqCloseFile()
{
	// close the audio file
	if(aqState_.fileHandle) {
		AudioFileClose(aqState_.fileHandle);
		aqState_.fileHandle = 0;
	}
}




void MusicEngineAudioQueue::deriveAudioQueueBufferSize(AudioStreamBasicDescription &asbd, UInt32 maxPacketSize, Float64 seconds)
{
	constexpr UInt32 maxBufferSize = 0x50000;
	constexpr UInt32 minBufferSize = 0x4000;

	if (asbd.mFramesPerPacket != 0) {
		Float64 packetCount = asbd.mSampleRate / asbd.mFramesPerPacket * seconds;
        aqState_.bufferSize = (packetCount + 1) * maxPacketSize;
	} else {
        aqState_.bufferSize = maxBufferSize > maxPacketSize ? maxBufferSize : maxPacketSize;
	}

	if (aqState_.bufferSize > maxBufferSize && aqState_.bufferSize > maxPacketSize)
        aqState_.bufferSize = maxBufferSize;

	if (aqState_.bufferSize < minBufferSize)
        aqState_.bufferSize = minBufferSize;

    aqState_.packetsReadCount = aqState_.bufferSize / maxPacketSize;
}


void MusicEngineAudioQueue::setVolume(Real volume, Real ramp)
{
    auto gain = volumeToGain(volume);

    MDebugFunc("%g/%g %g", volume, gain, ramp);

    aqSetVolumeGain(gain, ramp);
}



//bool MusicEngineAudioQueue::playing() {
//	if (!aqState_.isRunning)
//		return false;
//	return isQueueRunning();
//}

void MusicEngineAudioQueue::play(MusicTrackContextP track, Timestamp delay, Timestamp ramp, Real volume)
{
    paused_ = false;
    stopped_ = false;
    
	// Check if the audio queue is still busy
	// we just schedule the track
	if (isQueueRunning()) {
        // delay the play
        delay = std::max(delay, 0.5);
	}

    if (delay != 0) {
        triggerPlay_.schedule(delay);
        triggerPlay_.track = track;
        triggerPlay_.ramp = ramp;
        triggerPlay_.volume = volume;
        return;
    }

    
	aqPlayTrack(track, ramp, volume);
}

void MusicEngineAudioQueue::playCrossfade(MusicTrackContextP track, Timestamp delay, Timestamp ramp, Timestamp gap, Real volume)
{
    // Check if anything playing now.
    // If it's not then crossfade just bevomes play
    if (!aqState_.queue) {
        play(track, delay, ramp, volume);
        return;
    }
    
    if (ramp) {
        triggerStopRamp_.schedule(delay);
        triggerStopRamp_.ramp = ramp;
        delay += ramp;
    }
    
    triggerStop_.schedule(delay);
    
    delay += gap;

    triggerPlay_.schedule(delay);
    triggerPlay_.track = track;
    triggerPlay_.ramp = ramp;
    triggerPlay_.volume = volume;
}



void MusicEngineAudioQueue::stop(Timestamp delay, Timestamp ramp)
{
    MDebugFunc();
    
    // Check if we stopped already
    if (!aqState_.queue)
        return;
    
    
    triggerStop_.scheduled = false;
    triggerStopRamp_.scheduled = false;
    
    triggerPlayVolume_.scheduled = false;
    triggerPlay_.scheduled = false;

    
    // Check if we need to stop now or schedule it
    if (delay == 0 && ramp == 0) {
        aqStop();
        return;
    }

    
    if (ramp != 0) {
        triggerStopRamp_.schedule(delay);
        triggerStopRamp_.ramp = ramp;
    }

    // Schedule stop
    triggerStop_.schedule(delay + ramp);
}


void MusicEngineAudioQueue::pause()
{
    if (!isQueueRunning())
        return;

    // Remove all the further play commands
    triggerPlay_.scheduled = false;
    triggerPlayVolume_.scheduled = false;
    
    triggerPause_.schedule(0);
//    aqPause();
}

void MusicEngineAudioQueue::aqPause() {
    auto result = AudioQueuePause(aqState_.queue);
    
    if (!success(result)) {
        // If error happened then just dispose the queue
        aqDisposeQueue();
        aqCloseFile();
        
        if (!stopReported_) {
            Music::processTrackFinish(currentTrack_->trackId);
            stopReported_ = true;
        }
        
        return;
    }
//    assertResult(result, "AudioQueuePause");

    paused_ = true;
   
    Music::processTrackPause(currentTrack_->trackId);
}

void MusicEngineAudioQueue::resume() {
	if (!paused_ || !aqState_.queue)
		return;
    
    // Remove the stop commands
    triggerStop_.scheduled = false;
    triggerStopRamp_.scheduled = false;
    

//    aqResume();
    triggerResume_.schedule(0);
}

void MusicEngineAudioQueue::aqResume() {
    
    paused_ = false;
    
    auto result = AudioQueueStart(aqState_.queue, 0);
    assertResult(result, "AudioQueueStart");
    
    Music::processTrackResume(currentTrack_->trackId);
}


void MusicEngineAudioQueue::aqStop()
{
    if (!aqState_.queue)
        return;
    
    paused_ = false;
    stopped_ = true;
    aqState_.isRunning = false;


    // Stop the queue
    //AudioQueueFlush(aqData_->queue);
    AudioQueueReset(aqState_.queue);
    //AudioQueueStop(aqData_->queue, false);
    AudioQueueStop(aqState_.queue, true);
    
    aqDisposeQueue();
    aqCloseFile();
    
    if (!stopReported_) {
        Music::processTrackStop(currentTrack_->trackId);
        stopReported_ = true;
    }
}




void MusicEngineAudioQueue::aqPlayTrack(MusicTrackContextP track, Timestamp ramp, Real volume)
{
	currentTrack_ = track;

    OSStatus result{};
    
	// Just in case
	aqDisposeQueue();
	aqCloseFile();
    
    MDebugFunc("Playing: (ramp: %g, volume: %g) %s", ramp, volume, apl::extractFileName(track->filename));
    
    MDebugFunc("Audio: AudioQueue::Play::Thread Id: %ul", M::debugGetCurrentThreadId());


	try {
		CFURLRef url = CFURLCreateFromFileSystemRepresentation(kCFAllocatorDefault, 
                                                               (UInt8 *)track->filename.data(),
                                                               (CFIndex)track->filename.size(),
                                                               false);
        result = AudioFileOpenURL(url, kAudioFileReadPermission, 0, &aqState_.fileHandle);

        assertResult(result, "Cannot Opeen Music Track: %s", track->filename);
        
        CFRelease(url);

		AudioStreamBasicDescription *af = &aqState_.audioFormat;
		UInt32 propSize = sizeof(AudioStreamBasicDescription);

		result = AudioFileGetProperty(aqState_.fileHandle, kAudioFilePropertyDataFormat, &propSize, af);
		assertResult(result, "Can't read file format");


		// Create playback audio queue
		CFRunLoopRef runLoop = CFRunLoopGetCurrent();
        // CFRunLoopRef runLoop = CFRunLoopGetMain();
        // kCFRunLoopCommonModes
		result = AudioQueueNewOutput(&aqState_.audioFormat,
                                     aqBufferCallback,
                                     this,  // User data
                                     runLoop,  // callback run loop
                                     0,  // callback run loop mode
                                     0,  // flags (0)
                                     &aqState_.queue);
		assertResult(result, "CreateNewOutput");


        UInt32 maxPacketSize{};
		propSize = sizeof(maxPacketSize);

		result = AudioFileGetProperty(aqState_.fileHandle, kAudioFilePropertyPacketSizeUpperBound, &propSize, &maxPacketSize);
		assertResult(result, "GetUpperBound");

		Float64 bufferLengthSec = 2.;
		deriveAudioQueueBufferSize(aqState_.audioFormat, maxPacketSize, bufferLengthSec);

		MDebugFunc("Buffer Size: %d", aqState_.bufferSize);

		// allocate memory for the buffers
		bool isFormatVBR = (af->mBytesPerPacket == 0 || af->mFramesPerPacket == 0);

		if (isFormatVBR) {
            aqState_.packetDescBuffer.alloc(aqState_.packetsReadCount);
            aqState_.packetDesc = aqState_.packetDescBuffer.get();
		} else {
            aqState_.packetDesc = nullptr;
		}

		// Try to get total duration
		Float64 durationSecs = 0;
		propSize = sizeof(durationSecs);
        result = AudioFileGetProperty(aqState_.fileHandle, kAudioFilePropertyEstimatedDuration, &propSize, &durationSecs);
		if (success(result) && durationSecs) {
			currentTrack_->duration = durationSecs;
		}

		MDebugFunc("Music Start: %s : %0.2fs", apl::extractFileName(track->filename), track->duration);


		//
		// Set "magic cookie"
		//
		propSize = sizeof(UInt32) ;

        result = AudioFileGetPropertyInfo(aqState_.fileHandle, kAudioFilePropertyMagicCookieData, &propSize, 0);

		if (!result && propSize) {
			apl::ByteBuffer magicCookie(propSize) ;
            AudioFileGetProperty(aqState_.fileHandle, kAudioFilePropertyMagicCookieData, &propSize, magicCookie.get());
            AudioQueueSetProperty(aqState_.queue, kAudioQueueProperty_MagicCookie, magicCookie.get(), propSize);
		}

        aqState_.isRunning = true;


		//
		// Allocate and prime audio queue buffers
		//
        aqState_.currentPacket = 0;
        
        for (auto& b: aqState_.buffers) {
            b = nullptr;
        }

        for (auto& b: aqState_.buffers) {
            result = AudioQueueAllocateBuffer(aqState_.queue, aqState_.bufferSize, &b);
            assertResult(result, "AudioQueueAllocateBuffer");
		}

        for (auto& b: aqState_.buffers) {
            onAudioQueueOutputBuffer(aqState_.queue, b);
        }


		//
		// Set volume (gain)
		//
		if (ramp == 0) {  // if no ramp just set volume immediately
			aqSetVolume(volume, ramp);
		} else {  // if there is ramp first set volume to zero
			// Start with the zero
            aqSetVolumeGain(0, 0);

			// And schedule volume
            triggerPlayVolume_.schedule(0.1);
            triggerPlayVolume_.volume = volume;
            triggerPlayVolume_.ramp = ramp;
		}
        
		//
		// Start audio queue
		//
        result = AudioQueueStart(aqState_.queue, 0);
		assertResult(result, "Start");
        
        Music::processTrackStart(track->trackId);
        stopReported_ = false;
	}
	catch(const std::exception &e) {
		MDebugFunc("%s", e.what());
        MAnalyticsException(e.what());
	}
	catch(...) {
		MDebugFunc("Exception!");
        MAnalyticsException("AudioQueue::aqPlayTrack::Exception!");
	}
    
    if (result != 0) {
        aqDisposeQueue();
    }

}

void MusicEngineAudioQueue::aqSetVolume(Real volume, Timestamp ramp) {
    aqSetVolumeGain(volumeToGain(volume), ramp);
}


void MusicEngineAudioQueue::aqSetVolumeGain(Real gain, Timestamp ramp) {
    if (!aqState_.queue)
        return;
    
    MDebugFunc("Gain: %g, Ramp: %g", gain, ramp);
    
    OSStatus status{};
    
    status = AudioQueueSetParameter(aqState_.queue,
                                         kAudioQueueParam_VolumeRampTime,
                                         (AudioQueueParameterValue)ramp);
    handleResult(status);

    status = AudioQueueSetParameter(aqState_.queue,
                                    kAudioQueueParam_Volume,
                                    (AudioQueueParameterValue)gain);
    handleResult(status);
}


void MusicEngineAudioQueue::onAudioQueueOutputBuffer(AudioQueueRef aq, AudioQueueBufferRef buffer)
{
//    MDebugFunc("Audio: AudioQueue::Callback::Thread Id: %ul", M::debugGetCurrentThreadId());
    if (!aqState_.isRunning)
        return;

    aqState_.runningCheck = true;

    OSStatus result = 0;

    UInt32 bytesRead = aqState_.bufferSize;
    UInt32 packetsRead = aqState_.packetsReadCount;
    
    if (aqState_.fileHandle) {
        // Sometimes it crashes here
        result = AudioFileReadPacketData(aqState_.fileHandle,
                                         false,
                                         &bytesRead,
                                         aqState_.packetDesc,
                                         aqState_.currentPacket,
                                         &packetsRead,
                                         buffer->mAudioData);
        handleResult(result);
        
        if (result != 0)
            packetsRead = 0;
    } else {
        packetsRead = 0;
    }
    
    /*  loop the sound track automatically
     if ( packetsRead == 0 && state->repeat ) {
     state->currentPacket = 0 ;
     packetsRead = state->packetsReadCount ;
     
     result = AudioFileReadPacketData ( state->fileHandle, false, &bytesRead, state->packetDesc, state->currentPacket, &packetsRead, buffer->mAudioData ) ;
     mAssert(!result,"AudioFileReadPacketData") ;
     }
     */
    
    if (packetsRead > 0) {
        buffer->mAudioDataByteSize = bytesRead;
        
        if (aqState_.queue) {
            result = AudioQueueEnqueueBuffer(aqState_.queue,
                                             buffer,
                                             aqState_.packetDesc ? packetsRead : 0,
                                             aqState_.packetDesc);
            handleResult(result);
        }
        
        aqState_.currentPacket += packetsRead;
        return;
    }
    
    // No more packets
    MDebugFunc("AudioQueue buffers ended.");

    // Stop the queue, but don't destroy it yet - it still might be playing something
    if (aqState_.queue) {
        AudioQueueStop(aqState_.queue, false);
        triggerQueueDispose_.schedule(0.5);
        // From here we will be checking every tiny bit if the queue has stopped
        //                queueDisposeScheduled_ = 0.5;
    }
    
    aqCloseFile();
    
    aqState_.isRunning = false;
    
    if (!stopReported_) {
        Music::processTrackFinish(currentTrack_->trackId);
        stopReported_ = true;
    }

}



void MusicEngineAudioQueue::processTimer(Timestamp timestamp)
{
    currentTimestamp_ = timestamp;
    
    if (triggerQueueDispose_.check()) {
        
        if (isQueueRunning()) {
            // Wait a little longer
            triggerQueueDispose_.schedule(0.3);
            return;
        }
        
        aqDisposeQueue();
        aqCloseFile();
        if (!stopReported_) {
            Music::processTrackFinish(currentTrack_->trackId);
            stopReported_ = true;
        }

        return;
    }
        
    
    if (triggerStopRamp_.check()) {
        aqSetVolumeGain(0.001, triggerStopRamp_.ramp);
        //aqSetVolume(0.001, scheduleStopRampAction_.ramp);
        return;
    }

    if (triggerStop_.check()) {
        aqStop();
        return;
    }
    
    if (triggerPlay_.check()) {
        aqPlayTrack(triggerPlay_.track, triggerPlay_.ramp, triggerPlay_.volume);
        return;
    }
    
    if (triggerPlayVolume_.check()) {
        auto gain = volumeToGain(triggerPlayVolume_.volume);
        aqSetVolumeGain(gain, triggerPlayVolume_.ramp);
    }
    
    if (triggerPause_.check()) {
        aqPause();
    }

    if (triggerResume_.check()) {
        aqPause();
    }

}

}  // namespace mangl
#endif
