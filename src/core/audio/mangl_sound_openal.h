/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_sound.h"


#if MANGL_AUDIO_OPENAL

#if APEAL_OS_MAC

#include <OpenAL/OpenAL.h>

#elif APEAL_OS_ANDROID

using ALuint = int;
struct ALCcontext{};
struct ALCdevice{};

#elif APEAL_OS_WINDOWS

#include <alc.h>
#include <al.h>
#include <al/alut.h>

#elif APEAL_OS_UNIX

#include <AL/al.h>
#include <AL/alc.h>

#if !defined(__EMSCRIPTEN__)
#include <AL/alut.h>
#endif

#else
#error OpenAL Unsupported!!!
#endif

namespace mangl {


struct OpenALContext {
    ALuint sourceId;
    ALuint bufferId;
    ALfloat volume, gain;
} ;

class OpenALEngine {
    using Ectx = OpenALContext;
    using EctxP = OpenALContext*;
    using Ctx = SfxContext;
    using CtxP = SfxContextPtr;

    ALCcontext *context_{};
    ALCdevice *device_{};

    //bool initialized_{};
    void initialize();
    void destroy();


    Real volume_{} ;

    void updateVolume(EctxP ectx, Real volume);


public:
    //void init() ;
    void shutdownContext();
    void restoreContext();
    void clearErrors() {  alGetError(); }

    void release(CtxP &ctx)
    {
        auto ectx = getEngineContext(ctx);

        alSourceUnqueueBuffers(ectx->sourceId, 1, &ectx->bufferId);
        alSourcei(ectx->sourceId, AL_BUFFER, 0);
        alDeleteBuffers(1, &ectx->bufferId);
    }

    void release()
    {
//#if APEAL_OS_MAC
        if (!device_)
            return;
        alcMakeContextCurrent(0);
        alcDestroyContext(context_);
        alcCloseDevice(device_);
        device_ = 0;
//#endif
    }

    bool load(CtxP &ctx, StrView filename);
    bool play(CtxP &ctx, Real volume);

    void setVolume(CtxP &ctx, Real volume) {
        alSourcef(getEngineContext(ctx)->sourceId, AL_GAIN, (ALfloat)volume);
    }

private:
    EctxP getEngineContext(CtxP &ctx) {
        return static_cast<EctxP>(ctx->engineContext.get());
    }

    EctxP makeEngineContext(CtxP &ctx) {
        ctx->engineContext = std::make_unique<Ectx>();
        return getEngineContext(ctx);
    }
};

}  // namespace mangl

#endif

