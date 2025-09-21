/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../core/audio/mangl_sound.h"

#if MANGL_AUDIO_DIRECTSOUND

// WAVEFORMATEX 
#include <Mmreg.h>

#include <dsound.h>
#include <mmsystem.h>

namespace mangl {

struct SfxEngineDirectSoundContext {
    IDirectSoundBuffer *bufferApi{};
};


class SfxEngineDirectSound {
    using Ectx = SfxEngineDirectSoundContext;
    using Ectxp = SfxEngineDirectSoundContext *;
    using Ctx = SfxContext;
    using Ctxp = SfxContextPtr;

private:
    LPDIRECTSOUND directSoundApi_{};
    IDirectSoundBuffer* primaryBufferApi_{};

    bool initialized_ = false;
    void initialize();


//#pragma pack(1)
//    struct ChunkHeader {
//        uint8_t chunkId[4];
//        uint32_t chunkSize;
//    };
//
//    struct FmtChunk {
//        uint16_t audioFormat;
//        uint16_t numChannels;
//        uint32_t sampleRate;
//        uint32_t bytesPerSecond;
//        uint16_t blockAlign;
//        uint16_t bitsPerSample;
//    };
//
//    struct WavHeader {
//        ChunkHeader chunk_header;
//        uint8_t  format[4];
//    };
//#pragma pack()

public:
    void release(Ctxp &ctx);
    void release();

    void shutdownContext() {}
    void restoreContext() {}

    void clearErrors() {}

    bool load(Ctxp &ctx, StrView filename);
    bool play(Ctxp &ctx, Real volume);

    void setVolume(Ctxp &ctx, Real volume);

private:
    Ectx* getEngineContext(Ctxp &ctx) {
        return static_cast<Ectx *>(ctx->engineContext.get());
    }

    Ectx* makeEngineContext(Ctxp &ctx) {
        ctx->engineContext = std::make_unique<Ectx>();
        return getEngineContext(ctx);
    }
};

}  // namespace mangl

#endif
