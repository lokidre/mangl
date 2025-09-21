/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <iostream>
#include <string>
#include <vector>

#include <apeal/buffer.hpp>
#include <apeal/cmdline.hpp>
#include <apeal/endian.hpp>
#include <apeal/image.hpp>
#include <apeal/file.hpp>
#include <apeal/parse.hpp>

#include <apeal/image/bmp.hpp>
#include <apeal/image/ico.hpp>
#include <apeal/image/libpng.hpp>


using apl::String;
using apl::ByteBuffer;

using std::cout;
using std::cerr;
using std::endl;

// Test run
// mangl-make-ico -o test.ico test_256.png test_48.png test_32.png test_24.png test_16.png

constexpr static auto helpHeader = std::string_view{R"(
Windows ICO file generator.
Copyright (C) 2013-2022 Andrei Mirzoyan.

Usage: mangl-make-ico <options> [images...]

The images should come in the following resolutions:
    256x256, 48x48, 32x32, 24x24, 16x16

Options:
)"}.substr(1);


//typedef std::string String;


apl::CommandLine::StringList optInputFiles_;

//String optImage256_, optImage64_, optImage48_, optImage32_, optImage24_, optImage16_;
String optOutputFileName_;


struct ImageInfo {
    String source_file;
    apl::Image origImage, image;

    ByteBuffer output_data;
    apl::Index data_offset{};
};


int process()
{
    using namespace apl;
    //using namespace std;

    String s;


    //
    // Prepare the structures
    //
    std::multimap<Index, ImageInfo, std::greater<Index>> images;

    //
    // Load all the images
    //
    for (const auto& imageFile: optInputFiles_) {
        ImageInfo im;
        im.source_file = imageFile;
        im.origImage = loadImage(imageFile);

        verify(im.origImage.size.w <= 256 && im.origImage.size.h <= 256,
            "Image cannot be larger than 256x256: %s", imageFile);

        im.image = im.origImage.clone(ColorModel::BGRA);
        images.emplace(im.origImage.size.h, im);
    }

    //for (auto im = images.begin(); im != images.end(); ++im) {
    //    //LibPng libpng;
    //    //int width, height;

    //    verify(!im->source_file.empty(), "Image is not provided for %dx%d resolution", im->width, im->height);

    //    //auto imageContainer = libpng.loadImage(im->source_file);
    //    im->origImage = loadImage(im->source_file);
    //    //auto& ii = imageContainer.front();
    //    //libpng.load(im->source_file, width, height, im->image_data);

    //    verify(im->origImage.size.w == im->width && im->origImage.size.h == im->height, "Image dimensions %dx%d doesn't match for file: %s", im->width, im->height, im->source_file);

    //    im->image = im->origImage.clone(ColorModel::RGBA);

    //    //im->width = width, im->height = height;
    //}


    //
    // Build each image in the dictionary
    //
    //for (auto imIt = images.begin(); im != images.end(); ++im) {
    for (auto& imIt: images) {
        auto& im = imIt.second;
        auto width = im.origImage.size.w;
        auto height = im.origImage.size.h;

        Index outputDataSize = 0;

        auto alphaChannelRowSize = (width + 7) / 8;
        alphaChannelRowSize = (alphaChannelRowSize + 3) / 4 * 4;
        auto alphaChannelSize = alphaChannelRowSize * height;
        outputDataSize = sizeof(BmpInfoHeader) + im.image.dataSize() + alphaChannelSize;

        im.output_data.resize(outputDataSize);

        auto bih = (BmpInfoHeader*)im.output_data.get();
        memset(bih, 0, sizeof(BmpInfoHeader));

        bih->bih_size = to_le_32(sizeof(BmpInfoHeader));
        bih->width = to_le_32(width);
        bih->height = to_le_32(height * 2);  // bmp must come with doubled height
        bih->planes = to_le_16(1);
        bih->bits_per_pixel = to_le_16(32);
        bih->image_size = to_le_32(width * 4 * height);
        bih->x_pels_per_meter = to_le_32(2834);
        bih->y_pels_per_meter = to_le_32(2834);


        auto rowDataSize = width * 4;
        auto outputDataPtr = im.output_data.get() + sizeof(BmpInfoHeader);
        auto inputDataPtr = im.image.getData() + (height -1) * rowDataSize;

        for (Index j = 0; j < height; ++j) {

            //auto dest = outputDataPtr;
            //auto src = inputDataPtr;
            //for (Index i = 0; i < width; ++i) {
            //    dest[0] = src[2];
            //    dest[1] = src[1];
            //    dest[2] = src[0];
            //    dest[3] = src[3];

            //    dest += 4, src += 4;
            //}

            memcpy(outputDataPtr, inputDataPtr, rowDataSize);

            outputDataPtr += rowDataSize;
            inputDataPtr -= rowDataSize;
        }

        // Alpha channel
        auto alpha = im.output_data.get() + sizeof(BmpInfoHeader) + 3;
        int bit = 0;

        auto outAlphaPtr = im.output_data.get() + sizeof(BmpInfoHeader) + bih->image_size;
        memset(outAlphaPtr, 0, alphaChannelSize);

        for (Index y = 0; y < height; ++y) {

            auto a = outAlphaPtr + y*alphaChannelRowSize;

            for (Index x = 0; x < width; ++x) {
                if (*alpha == 0) {
                    (*a) |= 1 << (7 - bit);
                }

                ++bit;

                if (bit == 8) {
                    ++a;
                    bit = 0;
                }

                alpha += 4;
            }
        }
    }

    //
    // Process for output
    //
    auto totalFileSize = sizeof(IcoFileHeader);
    auto totalDictHeaderSize = images.size() * sizeof(IcoDirEntry);
    totalFileSize += totalDictHeaderSize;

    for (auto& imIt: images) {
        auto& im = imIt.second;
    //  for (auto im = images.begin(); im != images.end(); ++im) {
        im.data_offset = totalFileSize;
        totalFileSize += im.output_data.size();
    }

    //
    // Build the final image
    //
    File output;

    output.create(optOutputFileName_, File::CreateMode::CreateAlways, File::AccessMode::Write);

    // Write ico header
    IcoFileHeader ifh{};

    ifh.image_type = to_le_16(1);
    ifh.images_count = to_le_16(images.size());

    output.write(&ifh, sizeof(ifh));


    // Write directory entries
    for (auto& imIt: images) {
        auto& im = imIt.second;
        auto width = im.origImage.size.w;
        auto height = im.origImage.size.h;

        IcoDirEntry ide{};

        ide.width = width == 256 ? 0 : uint8_t(width);
        ide.height = height == 256 ? 0 : uint8_t(height);
        ide.planes = to_le_16(1);
        ide.bits_per_pixel = to_le_16(32);

        ide.data_size = to_le_32(im.output_data.size());
        ide.data_offset = to_le_32(im.data_offset);

        output.write(&ide, sizeof(ide));
    }

    // Write the actual images data
    for (auto& imIt: images) {
        auto& im = imIt.second;
        output.write(im.output_data.get(), im.output_data.size());
    }

    return 0;
}




int main(int argc, char* argv[])
{
    using namespace apl;

    int retCode = 1;

    try {
        CommandLine cmdline;

        cmdline.addFlag({
            .name = "help",
            .action = [&]{
                cmdline.printout(helpHeader).stop();
            },
            .desc = "Print this message",
        });
        cmdline.dup("h", "help");


        cmdline.add({.name = "output", .sbind = &optOutputFileName_, .desc = "Output .ICO file name"});
        cmdline.dup("o", "output");

        cmdline.bindArguments(&optInputFiles_, 1);  // Minimum 1 argument is required

        cmdline.process(argc, argv);

        if (cmdline.stopped())
            return retCode;

        if (optOutputFileName_.empty())
            throw std::runtime_error("output file is missing");

        retCode = process();
    }
    catch (std::exception& e) {
        cerr << e.what() << endl;
    }
    catch (...) {
        cerr << "Exception!" << endl;
    }

    return retCode;
}
