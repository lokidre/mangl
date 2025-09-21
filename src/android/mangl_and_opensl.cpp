/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_opensl.h"


#include <apeal/debug.hpp>

#if MANGL_AUDIO_OPENSL

#include <stdexcept>

#include <unistd.h>

#if _DEBUG
#define ASSERT_RESULT(__result) \
    if ((__result) != SL_RESULT_SUCCESS) { \
        MDebugPrint("OpenSL FAIL:%d:code %d", __LINE__, (__result));  \
        throw std::runtime_error("Bailing Out") ;\
    }

#else
#define ASSERT_RESULT(__result)
#endif

#define QUEUE_BUFFER_COUNT 1
#define QUEUE_BUFFER_CONTEXT_COUNT 8

#define SBC_AUDIO_OUT_BUFFER_SIZE (4096*4)
#define SBC_AUDIO_OUT_CHANNELS 8

namespace mangl {

namespace android {

struct BufferQueueContext {

	SLObjectItf slMixerObject;
	SLObjectItf slPlayerObject;
	SLPlayItf slPlay;
	SLAndroidSimpleBufferQueueItf slBufferQueue;
	SLVolumeItf slVolume;
	SLint32 volumeMin, volumeMax;
	SLboolean isMillibel;

	bool busy;
	uint8_t *dataPtr;
	int dataPosition, dataSize;

	uint8_t *buffer;
	int bufferSize;

	long long startTimestamp;

};

BufferQueueContext bufferContexts_[QUEUE_BUFFER_CONTEXT_COUNT];

static void bufferQueueCallback(SLAndroidSimpleBufferQueueItf queue, void *context) {
	auto ctx = static_cast<BufferQueueContext *>(context);

	if (!ctx->busy)
		return;

	if (ctx->dataPosition >= ctx->dataSize) {
		ctx->busy = false;
		return;
	}

	if (ctx->dataPtr == nullptr) {
		ctx->busy = false;
		return;
	}

	int enqueueSize = std::min(ctx->dataSize - ctx->dataPosition, ctx->bufferSize);

	memcpy(ctx->buffer, ctx->dataPtr + ctx->dataPosition, enqueueSize);

	ctx->dataPosition += enqueueSize;

	SLresult result = (*queue)->Enqueue(queue, ctx->buffer, enqueueSize);

	if (result != SL_RESULT_SUCCESS) {
		MDebugPrint("Enqueue FAIL:%d:code %d", __LINE__, result);
	}

}

} // namespace android




void SfxEngineOpenSL::initialize() {
}

void SfxEngineOpenSL::initEngine(const AudioFileLoader&) {
    using namespace android;

    if (slObject_)
        return;


    MDebugFunc();

    //debug_printf ( "SoundEngine::init()" ) ;

    SLresult result;

    try {
        SLEngineOption engineOption[] = {(SLuint32)SL_ENGINEOPTION_THREADSAFE, (SLuint32)SL_BOOLEAN_TRUE};

        result = slCreateEngine(&slObject_, 1, engineOption, 0, nullptr, nullptr);
        ASSERT_RESULT(result);

        result = (*slObject_)->Realize(slObject_, SL_BOOLEAN_FALSE);
        ASSERT_RESULT(result);

        // get engine interface
        result = (*slObject_)->GetInterface(slObject_, SL_IID_ENGINE, &slEngine_);
        ASSERT_RESULT(result);

        auto slEng = *slEngine_;

        for (auto ctx = bufferContexts_, ctxEnd = bufferContexts_ + QUEUE_BUFFER_CONTEXT_COUNT;
             ctx != ctxEnd; ++ctx) {
            //debug_printf ( "Initializing buffer context" ) ;

            ctx->busy = false;

            //
            // Mixing output
            //

            // mixed output
            const SLuint32 outputMixIIDCount = 0;
            const SLInterfaceID outputMixIIDs[] = {SL_IID_VOLUME};
            const SLboolean outputMixReqs[] = {SL_BOOLEAN_TRUE};

            // create output
            result = slEng->CreateOutputMix(slEngine_, &ctx->slMixerObject, outputMixIIDCount, outputMixIIDs, outputMixReqs);
            ASSERT_RESULT(result);

            result = (*ctx->slMixerObject)->Realize(ctx->slMixerObject, SL_BOOLEAN_FALSE);
            ASSERT_RESULT(result);


            // INPUT
            // audio source with maximum of two buffers in the queue
            // where are data
            SLDataLocator_AndroidSimpleBufferQueue dataLocatorInput{};
            dataLocatorInput.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
            dataLocatorInput.numBuffers = QUEUE_BUFFER_COUNT;


            // format of data
            SLDataFormat_PCM pcm{};
            pcm.formatType = SL_DATAFORMAT_PCM;
            pcm.numChannels = 1; // Mono sound.
                                 //pcm.samplesPerSec = SL_SAMPLINGRATE_11_025;
            //pcm.samplesPerSec = SL_SAMPLINGRATE_22_05;
            //pcm.samplesPerSec = SL_SAMPLINGRATE_44_1 ;
            pcm.samplesPerSec = SL_SAMPLINGRATE_48;
            pcm.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
            pcm.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;
            pcm.channelMask = SL_SPEAKER_FRONT_CENTER /*|  SL_SPEAKER_FRONT_LEFT |  SL_SPEAKER_FRONT_RIGHT */;
            pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;


            // combine location and format into source
            SLDataSource dataSource{};
            dataSource.pLocator = &dataLocatorInput;
            dataSource.pFormat = &pcm;


            // OUTPUT
            SLDataLocator_OutputMix dataLocatorOut{};
            dataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
            dataLocatorOut.outputMix = ctx->slMixerObject;

            SLDataSink dataSink{};
            dataSink.pLocator = &dataLocatorOut;
            dataSink.pFormat = nullptr;


            // create sound player
            const SLuint32 soundPlayerIIDCount = 3;
            const SLInterfaceID soundPlayerIIDs[soundPlayerIIDCount] = {SL_IID_PLAY, SL_IID_BUFFERQUEUE, SL_IID_VOLUME};
            const SLboolean soundPlayerReqs[soundPlayerIIDCount] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

            result = slEng->CreateAudioPlayer(slEngine_, &ctx->slPlayerObject, &dataSource, &dataSink, soundPlayerIIDCount, soundPlayerIIDs, soundPlayerReqs);
            ASSERT_RESULT(result);

            auto slPlayer = *ctx->slPlayerObject;

            result = slPlayer->Realize(ctx->slPlayerObject, SL_BOOLEAN_FALSE);
            ASSERT_RESULT(result);


            // get interfaces
            result = slPlayer->GetInterface(ctx->slPlayerObject, SL_IID_PLAY,  &ctx->slPlay);
            ASSERT_RESULT(result);

            result = slPlayer->GetInterface(ctx->slPlayerObject, SL_IID_BUFFERQUEUE, &ctx->slBufferQueue);
            ASSERT_RESULT(result);
            result = (*ctx->slBufferQueue)->RegisterCallback(ctx->slBufferQueue, bufferQueueCallback, ctx);
            ASSERT_RESULT(result);

            result = slPlayer->GetInterface(ctx->slPlayerObject, SL_IID_VOLUME, &ctx->slVolume);
            ASSERT_RESULT(result);

            ctx->volumeMin = SL_MILLIBEL_MIN;
            SLmillibel maxValue{};
            result = (*ctx->slVolume)->GetMaxVolumeLevel(ctx->slVolume, &maxValue);
            ctx->volumeMax = maxValue;
            ctx->volumeMin = maxValue - 2000;

            // allocate buffer
            ctx->bufferSize = SBC_AUDIO_OUT_BUFFER_SIZE;
            ctx->buffer = new uint8_t[ctx->bufferSize + 4];
        }
    }
    catch (std::exception &e) {
        MDebugPrint("OpenSL init failed: %s", e.what());
    }
    catch (...) {
        MDebugPrint("OpenSL init failed: Unhandled exception!");
    }

    MDebugFunc("Done.");

}


bool SfxEngineOpenSL::load(CtxP &ctx, StrView filename) {

    auto ectx = makeEngineContext(ctx);

	try {
        AudioFileLoader afl;
        afl.load(filename);

        ectx->pcm = std::move(afl.buffer);

		// Check the audio info for: 44.1/16/1
		if (afl.numChannels != 1 || afl.bitsPerSample != 16 || afl.sampleRate != 48000)
			throw std::runtime_error(apl::sformat("SoundEngine:Invalid flac %s instead(48000:16:1) have:%d:%d:%d", filename, afl.sampleRate, afl.bitsPerSample, afl.numChannels));

        initEngine(afl);

		//MDebugPrint("loaded %s:%d", filename, ectx->pcm.size());
	}
	catch (std::exception &e) {
		MDebugPrint("%s:%d:%s", __FILE__, __LINE__, e.what());
		return false;
	}
	catch (...) {
		MDebugPrint("%s:%d:Unhandled Exception", __FILE__, __LINE__);
		return false;
	}

	return true;
}

void SfxEngineOpenSL::release(CtxP &ctx) {

	using namespace android;

	auto ectx = getEngineContext(ctx);

	// check if any of the current contexts contain the sound
	for (auto bctx = bufferContexts_, ctxEnd = bufferContexts_ + QUEUE_BUFFER_CONTEXT_COUNT;
		 bctx != ctxEnd; ++bctx) {
		if (bctx->dataPtr == ectx->pcm.get()) {
			if (bctx->busy) {
				(*bctx->slPlay)->SetPlayState(bctx->slPlay, SL_PLAYSTATE_STOPPED);
				bctx->busy = false;
				bctx->dataPtr = nullptr;
				bctx->dataPosition = 0;
				bctx->dataSize = 0;
			}
		}
	}

	ectx->pcm.clear();
}

void SfxEngineOpenSL::release() {
}

void SfxEngineOpenSL::setVolume(CtxP &ctx, Real volume) {
}

bool SfxEngineOpenSL::play(CtxP &ctx, Real volume) {
	using namespace android;

	auto ectx = getEngineContext(ctx);

	SLresult result{};

	try {
		// find free sound slot
		long long oldestTimestamp = 0;
		BufferQueueContext* oldestChannel{};
		BufferQueueContext* bctx{};
		for (auto ch = bufferContexts_, chEnd = bufferContexts_ + QUEUE_BUFFER_CONTEXT_COUNT;
			 ch != chEnd; ++ch) {
			if (!ch->busy) {
				bctx = ch;
				break;
			}

			if (oldestChannel == nullptr || ch->startTimestamp < oldestTimestamp) {
				oldestTimestamp = ch->startTimestamp;
				oldestChannel = ch;
			}
		}

		if (nullptr == bctx)
			bctx = oldestChannel;
		if (nullptr == bctx) {
#if _DEBUG
            throw std::runtime_error("Something went wrong");
#else
            return false;
#endif
        }


		// load sound info into free slot
		bctx->busy = true;
		bctx->dataPtr = ectx->pcm.get();
		bctx->dataPosition = 0;
		bctx->dataSize = ectx->pcm.size();

		//debug_printf ( "playing: size: %d", ctx->dataSize ) ;

		struct timespec ticks{};
		clock_gettime(CLOCK_MONOTONIC, &ticks);
		bctx->startTimestamp = ticks.tv_sec * 1000000000LL + ticks.tv_nsec;

		auto volumeMB = SLmillibel(Real(bctx->volumeMax - bctx->volumeMin) * volume + Real(bctx->volumeMin));

		result = (*bctx->slVolume)->SetVolumeLevel(bctx->slVolume, volumeMB);
		ASSERT_RESULT(result);

		int enqueueSize = std::min(bctx->dataSize, bctx->bufferSize);

		memcpy(bctx->buffer, bctx->dataPtr, enqueueSize);
		bctx->dataPosition += enqueueSize;

		result = (*bctx->slBufferQueue)->Enqueue(bctx->slBufferQueue, bctx->buffer, enqueueSize);
		ASSERT_RESULT(result);

		result = (*bctx->slPlay)->SetPlayState(bctx->slPlay, SL_PLAYSTATE_PLAYING);
		ASSERT_RESULT(result);

        MDebugFunc("Playing...");
	}
	catch (std::exception &e) {
		MDebugPrint("%s:%d:%s", __FILE__, __LINE__, e.what());
		return false;
	}
	catch (...) {
		MDebugPrint("%s:%d:Unhandled Exception", __FILE__, __LINE__);
		return false;
	}

	return true;
}

} //namespace mangl

#endif
