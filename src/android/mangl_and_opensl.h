/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../core/mangl_setup.h"
#include "../core/audio/mangl_sound.h"

#if MANGL_AUDIO_OPENSL

#include <map>

#include <apeal/types.hpp>
#include <apeal/buffer.hpp>

// for native audio
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "../core/audio/mangl_audio_file.h"

namespace mangl {

struct SfxEngineOpenSLContext {
    int hashIndex{};
    apl::ByteBuffer pcm;
};

class SfxEngineOpenSL {
    using Ctx = SfxContext;
    using CtxP = SfxContextPtr;
    using Ectx = SfxEngineOpenSLContext;
    using EctxP = SfxEngineOpenSLContext*;

    SLObjectItf slObject_{};
    SLEngineItf slEngine_{};

    void initialize();
    void destroy();

    EctxP getEngineContext(CtxP &ctx) {
        return static_cast<EctxP>(ctx->engineContext.get());
    }

    EctxP makeEngineContext(CtxP &ctx) {
        ctx->engineContext = std::make_unique<Ectx>();
        return getEngineContext(ctx);
    }

public:
    //void init();
    void shutdownContext() {}
    void restoreContext() {}
    void clearErrors() {}

    void release(CtxP &ctx);
    void release();

    bool load(CtxP &ctx, StrView filename);
    bool play(CtxP &ctx, Real volume);

    void setVolume(CtxP &ctx, Real volume);

public:
    void sendSoundBuffer();
    void prepareSoundBuffer();
    void swapSoundBuffers();

private:
    void initEngine(const AudioFileLoader& afl);
};

} // namespace mangl

#endif
