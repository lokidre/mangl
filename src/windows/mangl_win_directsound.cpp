/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_win_directsound.h"

#include "../core/audio/mangl_audio_file.h"


#if MANGL_AUDIO_DIRECTSOUND

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")


#include "mangl_win_main_window.h"

namespace mangl {

void SfxEngineDirectSound::initialize()
{
    HRESULT result;

    result = DirectSoundCreate(0, &directSoundApi_, 0);

    if (FAILED(result))
        return;

#if MANGL_WINMAIN

    result = directSoundApi_->SetCooperativeLevel(ManglMainWindow::instance()->hwnd(), DSSCL_PRIORITY);

    if (FAILED(result))
        return;
#endif

    DSBUFFERDESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.dwSize = sizeof(DSBUFFERDESC);
    bd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
    bd.guid3DAlgorithm = GUID_NULL;

    // Get control of the primary sound buffer on the default sound device.
    result = directSoundApi_->CreateSoundBuffer(&bd, &primaryBufferApi_, NULL);
    if (FAILED(result))
        return;

    WAVEFORMATEX wf{};

    // Setup the format of the primary sound bufffer.
    // In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nSamplesPerSec = 48000; // 44100;
    wf.wBitsPerSample = 16;
    wf.nChannels = 1; //2;
    wf.nBlockAlign = (wf.wBitsPerSample / 8) * wf.nChannels;
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
    wf.cbSize = 0;

    // Set the primary buffer to be the wave format specified.
    result = primaryBufferApi_->SetFormat(&wf);
    if (FAILED(result))
        return;

    initialized_ = true;
}

void SfxEngineDirectSound::release()
{
    if (primaryBufferApi_) {
        primaryBufferApi_->Release();
        primaryBufferApi_ = 0;
    }

    if (directSoundApi_) {
        directSoundApi_->Release();
        directSoundApi_ = 0;
    }
}

void SfxEngineDirectSound::release(Ctxp &ctx)
{
    auto ectx = getEngineContext(ctx);

    if (ectx->bufferApi) {
        ectx->bufferApi->Release();
        ectx->bufferApi = 0;
    }

    ctx->engineContext.reset();
}


bool SfxEngineDirectSound::load(Ctxp &ctx, StrView filename)
{
    if (!initialized_)
        initialize();

    auto ectx = makeEngineContext(ctx);

    // If no DirectSound engine found just imitate normal loading
    if (!directSoundApi_)
        return true;


    HRESULT result;
    AudioFileLoader afl;

    afl.load(filename);

//    apl::File file;

    //WavHeader wh;

    //file.open(filename, apl::FileAccess::Read);
    //file.read(&wh, sizeof(wh));

    ////
    //// Analyze the header
    ////
    //if (memcmp(wh.chunk_header.chunkId, "RIFF", 4) != 0)
    //    mthrowError("File format not recognized: %s", filename);


    WAVEFORMATEX wf{};


    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nSamplesPerSec = (DWORD)afl.sampleRate;
    wf.wBitsPerSample = (WORD)afl.bitsPerSample;
    wf.nChannels = (WORD)afl.numChannels;
    wf.nBlockAlign = (wf.wBitsPerSample / 8) * wf.nChannels;
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;


    //apl::ByteBuffer buffer;

    //// read and analize data chunks
    //ChunkHeader ch;
    //int64_t headerCount, chunkCount;

    //for (;;) {

    //    if ((headerCount = file.read(&ch, sizeof(ch))) == 0)
    //        break;

    //    if (memcmp(ch.chunkId, "data", 4) == 0) {
    //        buffer.resize(ch.chunkSize);
    //        chunkCount = file.read(buffer.get(), ch.chunkSize);
    //    } else
    //        if (memcmp(ch.chunkId, "fmt ", 4) == 0) {
    //            FmtChunk fmt;
    //            chunkCount = file.read(&fmt, sizeof(fmt));

    //            wf.wFormatTag = WAVE_FORMAT_PCM;
    //            wf.nSamplesPerSec = fmt.sampleRate;
    //            wf.wBitsPerSample = fmt.bitsPerSample;
    //            wf.nChannels = fmt.numChannels;
    //            wf.nBlockAlign = (wf.wBitsPerSample / 8) * wf.nChannels;
    //            wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
    //        } else {
    //            chunkCount = 0;
    //        }

    //    // read the rest of the chunk
    //    if (chunkCount < (int)ch.chunkSize)
    //        file.seek(ch.chunkSize - chunkCount, apl::FileOrigin::Current);
    //}



    DSBUFFERDESC bd{};

    bd.dwSize = sizeof(DSBUFFERDESC);
    bd.dwFlags = DSBCAPS_CTRLVOLUME;
    bd.dwBufferBytes = (DWORD)afl.buffer.size();
    bd.lpwfxFormat = &wf;
    bd.guid3DAlgorithm = GUID_NULL;

    IDirectSoundBuffer* tempBufferApi{};

    result = directSoundApi_->CreateSoundBuffer(&bd, &tempBufferApi, NULL);
    apl::verify(SUCCEEDED(result), "Cannot create sound buffer for: %s", filename);

    // Test the buffer format against the direct sound 8 interface and create the secondary buffer.
    result = tempBufferApi->QueryInterface(IID_IDirectSoundBuffer, (void **)&(ectx->bufferApi));

    tempBufferApi->Release();
    tempBufferApi = 0;

    if (FAILED(result)) {
        MDebugPrint("Failed to query DirectSoundBuffer interface");
        return false;
    }

    //file.close();

    // Lock the secondary buffer to write wave data into it.
    DWORD bufferSize = (DWORD)afl.buffer.size();
    void *bufferPtr = 0;
    result = ectx->bufferApi->Lock(0, bufferSize, &bufferPtr, &bufferSize, NULL, 0, 0);
    if (FAILED(result)) {
        MDebugPrint("Failed to lock secondary buffer");
        return false;
    }

    memcpy(bufferPtr, afl.buffer.get(), afl.buffer.size());

    result = ectx->bufferApi->Unlock(bufferPtr, bufferSize, NULL, 0);
    if (FAILED(result)) {
        MDebugPrint("Failed to unlock secondary buffer");
        return false;
    }

    return true;
}

bool SfxEngineDirectSound::play(Ctxp &ctx, Real volume)
{
    auto ectx = getEngineContext(ctx);

    if (!ectx->bufferApi)
        return false;

    HRESULT result;

    // Set position at the beginning of the sound buffer.
    result = ectx->bufferApi->SetCurrentPosition(0);
    if (FAILED(result))
        return false;

    // Set volume of the buffer to 100%.
    // TODO: better ramp
    Real v = apl::lerp(Real(DSBVOLUME_MIN), Real(DSBVOLUME_MAX), volume);

    result = ectx->bufferApi->SetVolume(LONG(v));
    if (FAILED(result))
        return false;

    // Play the contents of the secondary sound buffer.
    result = ectx->bufferApi->Play(0, 0, 0);

    if (FAILED(result))
        return false;

    return true;
}

void SfxEngineDirectSound::setVolume(Ctxp &ctx, Real volume)
{
    auto ectx = getEngineContext(ctx);

    if (!ectx || !ectx->bufferApi)
        return;

    Real v = apl::lerp(Real(DSBVOLUME_MIN), Real(DSBVOLUME_MAX), volume);

    ectx->bufferApi->SetVolume(LONG(v));
}

}  // namespace mangl


#endif
