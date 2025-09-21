/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_audio_file.h"

#include <apeal/fs.hpp>


#define MANGL_SUPPORT_WAV 1
#define MANGL_SUPPORT_FLAC 1


#include "../3rdparty/flac/FLAC++/flac_all.h"


namespace mangl {


#pragma pack(1)
struct WavChunkHeader {
    uint8_t  chunkId[4];
    uint32_t chunkSize;
};

struct WavFmtChunk {
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t bytesPerSecond;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
};

struct WavHeader {
    WavChunkHeader chunkHeader;
    uint8_t        format[4];
};
#pragma pack()


void AudioFileLoader::load(StrView filename) 
{
    auto ext = apl::extractFileExt(filename);

    if ("wav" == ext) {
        loadWav(filename);
        return;
    }

    if ("flac" == ext) {
        loadFlac(filename);
        return;
    }
}



void AudioFileLoader::loadWav(StrView filename) 
{
    apl::File file;

    WavHeader wh;

    file.open(filename, apl::FileAccess::Read);
    file.read(&wh, sizeof(wh));

    //
    // Analyze the header
    //
    if (memcmp(wh.chunkHeader.chunkId, "RIFF", 4) != 0)
        mthrowError("File format not recognized: %s", filename);


    // read and analize data chunks
    WavChunkHeader ch;
    int64_t headerCount, chunkCount;

    for (;;) {

        if ((headerCount = file.read(&ch, sizeof(ch))) == 0)
            break;

        if (memcmp(ch.chunkId, "data", 4) == 0) {
            buffer.resize(ch.chunkSize);
            chunkCount = file.read(buffer.get(), ch.chunkSize);
        } else
            if (memcmp(ch.chunkId, "fmt ", 4) == 0) {
                WavFmtChunk fmt;
                chunkCount = file.read(&fmt, sizeof(fmt));

                sampleRate = fmt.sampleRate;
                bitsPerSample = fmt.bitsPerSample;
                numChannels = fmt.numChannels;
            } else {
                chunkCount = 0;
            }

        // read the rest of the chunk
        if (chunkCount < (int)ch.chunkSize)
            file.seek(ch.chunkSize - chunkCount, apl::FileOrigin::Current);
    }


    file.close();

}



//
// Flac
//

namespace mangl_impl_ {
class FlacLoader: public FLAC::Decoder::Stream {
public:
    FlacLoader(AudioFileLoader& afl, StrView filename): afl_{afl} {
        apl::loadFile(filename, &sourceData_);
        sourceOffset_ = 0;
    }

private:
    AudioFileLoader& afl_;
    size_t destOffset_ {};
    bool aflInitialized_{};

    // Source
    apl::ByteBuffer sourceData_;
    size_t sourceOffset_{};




protected:
    void metadata_callback(const ::FLAC__StreamMetadata* metadata) override {
        afl_.bitsPerSample = metadata->data.stream_info.bits_per_sample;
        afl_.sampleRate = metadata->data.stream_info.sample_rate;
        afl_.numChannels = metadata->data.stream_info.channels;

        afl_.totalSamples = (Index)metadata->data.stream_info.total_samples;

        auto totalBytes = afl_.totalSamples * afl_.numChannels * afl_.bitsPerSample / 8;

        afl_.buffer.alloc(totalBytes);

        aflInitialized_ = true;

        apl::verify(afl_.bitsPerSample == 16, "FLAC:Only 16 bits per sample supported at this time");

    }

    ::FLAC__StreamDecoderSeekStatus seek_callback(FLAC__uint64 absolute_byte_offset) override {
        sourceOffset_ = absolute_byte_offset;
        return FLAC__STREAM_DECODER_SEEK_STATUS_OK;
    }

	::FLAC__StreamDecoderTellStatus tell_callback(FLAC__uint64* absolute_byte_offset) override {
        *absolute_byte_offset = sourceOffset_;
        return FLAC__STREAM_DECODER_TELL_STATUS_OK;
    }

	::FLAC__StreamDecoderLengthStatus length_callback(FLAC__uint64* stream_length) override {
        *stream_length = sourceData_.size();
        return FLAC__STREAM_DECODER_LENGTH_STATUS_OK;
    }

    	bool eof_callback() override {
        auto remainingBytes = sourceData_.size() - sourceOffset_;
        return (remainingBytes == 0);
    }



	::FLAC__StreamDecoderReadStatus read_callback(FLAC__byte buffer[], size_t *bytes) override {
        auto remainingBytes = sourceData_.size() - sourceOffset_;
        if (remainingBytes == 0)
            return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
        auto bytesToRead = std::min(remainingBytes, *bytes);
        std::memcpy(buffer, sourceData_.data() + sourceOffset_, bytesToRead);
        sourceOffset_ += bytesToRead;
        *bytes = bytesToRead;
        return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
    }



	::FLAC__StreamDecoderWriteStatus write_callback(const ::FLAC__Frame *frame, const FLAC__int32 * const buffer[]) override {

        if (!aflInitialized_) {
            aflInitialized_ = true;
            afl_.bitsPerSample = frame->header.bits_per_sample;
            afl_.numChannels = frame->header.channels;
            afl_.sampleRate = frame->header.sample_rate;

            apl::verify(afl_.bitsPerSample == 16, "FLAC:Only 16 bits per sample supported at this time");
        }

        auto blockBytes = frame->header.blocksize * afl_.numChannels * frame->header.bits_per_sample / 8;

        if (destOffset_ + blockBytes > afl_.buffer.size())
            afl_.buffer.resize(destOffset_ + blockBytes);

        auto destPtr = afl_.buffer.data() + destOffset_;

        // For 16 bits per sample
        	for (size_t i = 0; i < frame->header.blocksize; i++) {
            for (int iChannel = 0; iChannel < afl_.numChannels; ++iChannel) {
                // Convert from big endian
                auto sample = static_cast<FLAC__uint16>(buffer[iChannel][i]);
                *destPtr++ = static_cast<uint8_t>(sample);  // hsb
                *destPtr++ = static_cast<uint8_t>(sample >> 8);  // lsb
            }
        }

        destOffset_ += destPtr - (afl_.buffer.data() + destOffset_);

        return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
    }

	void error_callback(::FLAC__StreamDecoderErrorStatus status) override {
        mthrowError("FLAC decoder error: %d - %s", (int)status, FLAC__StreamDecoderErrorStatusString[status]);
    }

};

}  // namespace mangl_impl_;


void AudioFileLoader::loadFlac(StrView filename) 
{
    mangl_impl_::FlacLoader loader{*this, filename};

    loader.init();
    loader.process_until_end_of_stream();
    loader.finish();

}



}  // namespace mangl

