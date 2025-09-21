/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "texmake.h"

#include <apeal/parse.hpp>
#include <apeal/parse/ini.hpp>

#include <apeal/parse/yaml.hpp>

void TexMake::loadDescFile() {
    using namespace apl;
    int currentLine = 0;

    SizeI outputSize{};

    try {
        auto yaml = loadYaml(params.inputFile);

        for (auto& node: yaml.front()) {
            switch (pshHash(node.key)) {
            case "defines"_psh:
                for (auto& defMapNode: node) {
                    String defineName, defineValue;

                    for (auto& defNode: defMapNode) {
                        switch (pshHash(defNode.key)) {
                        case "name"_psh: defNode.get(&defineName); break;
                        case "value"_psh: defNode.get(&defineValue); break;
                        default: throwError("Invalid defines.key: %s", defNode.key);
                        }
                    }

                    defines_[defineName] = preprocessVariable(defineName, defineValue);
                }
                break;

            case "class"_psh:
                for (auto& clsNode: node) {
                    switch (pshHash(clsNode.key)) {
                    case "name"_psh: clsNode.get(&clsName); break;
                    case "namespace"_psh: clsNode.get(&clsNamespace); break;
                    default: throwError("Invalid class.key: %s", clsNode.key);
                    }
                }
                break;

            case "fonts"_psh:
                for (auto& fontMapNode: node) {
                    auto& font = fonts_.emplace_back();

                    for (auto& fontNode: fontMapNode) {
                        switch (pshHash(fontNode.key)) {
                        case "name"_psh: fontNode.get(&font.name); break;
                        case "file"_psh: fontNode.get(&font.filename); break;
                        case "nosuffix"_psh: fontNode.get(&font.nosuffix); break;
                        default: throwError("Invalid font.key: %s", fontNode.key);
                        }
                    }

                    verify(!font.name.empty(), "Missing 'fonts.name'");
                    verify(!font.filename.empty(), "Missing 'fonts.file'");
                }
                break;

            case "images"_psh:
                for (auto& imMapNode: node) {
                    String imageName;
                    
                    std::vector<String> imageFiles;

                    for (auto& imNode: imMapNode) {
                        switch (pshHash(imNode.key)) {
                        case "name"_psh: imNode.get(&imageName); break;
                        case "file"_psh:
                        case "files"_psh:
                            if (imNode.isScalar()) {
                                imageFiles.push_back(imNode.getString());
                            } else {
                                for (auto& fileNode: imNode) {
                                    imageFiles.push_back(fileNode.getString());
                                }
                            }
                            break;
                        default: throwError("Invalid image.key: %s", imNode.key);
                        }
                    }

                    verify(!imageName.empty(), "Missing 'images.name'");
                    verify(!imageFiles.empty(), "Missing 'images.file'");

                    // Check for dumplicates
                    for (auto& tex: textures_) {
                        if (tex.variableName == imageName)
                            throwError("Duplicate texture: %s", imageName);
                    }

                    for (auto& imageFile: imageFiles) {

                        auto range = processRange(imageFile);

                        for (auto& fn: range) {
                            TexSource s;
                            s.filename = fn;
                            s.variableName = imageName;
                            textures_.emplace_back(s);
                        }
                    }
                }

                break;

            case "output"_psh:
                for (auto& outNode: node) {
                    switch (pshHash(outNode.key)) {
                    case "size"_psh:
                        if (outNode.isScalar()) {
                            outNode.get(&outputSize.w);
                            if (outputSize.w == 0)
                                throwError("Invalid output size: %s", outNode.value);
                            outputSize.h = outputSize.w;
                        } else {
                            if (outNode.nodes.size() < 2)
                                throwError("size must be given either as array [<width>, <heigth>] or one number");
                            outNode.nodes[0].get(&outputSize.w);
                            outNode.nodes[1].get(&outputSize.h);
                        }
                        break;
                    case "file"_psh:
                        break;
                    default: throwError("Invalid output.key: %s", outNode.key);
                    }
                }
                break;


            default: throwError("Invalid key: %s", node.key);
            }
        }

    } catch (const std::exception& e) {
        throwError("%s:%d:%s", params.inputFile, currentLine, e.what());
    } catch (...) {
        throwError("%s:%d:Unhandled Exception!", params.inputFile, currentLine);
    }


    verify(outputSize, "Missing output.size or zero");

    // Check the data
    // verify(outputSize, "Output image size is missing: output=WxH");


    //
    // Prepare suffixes
    //
    suffixes_.clear();
    if (!params.suffix.empty())
        suffixes_.push_back(params.suffix);

    if (!params.fallbackSuffix.empty())
        suffixes_.push_back(params.fallbackSuffix);

    if (params.scaleMissing)
        suffixes_.emplace_back("");


    //
    // Now adjust the input resource names
    //
    for (auto& tex: textures_) {
        tex.filename = processInputFileName(tex.filename, "png");
    }

    for (auto& font: fonts_) {
        font.filename = processInputFileName(font.filename, "yaml", font.nosuffix);
    }


    // Load fonts if any
    for (auto& font: fonts_) {
        if (!font.filename.empty())
            loadFontSource(font);
        else {
            auto errorMessage = std::string("Error: " + font.name + " font file is missing ");
            throw std::runtime_error(errorMessage);
        }
    }


    outputSize *= params.scale;

    // Prepare output file
    // maybe in the future add file format (RGBA vs RGB)
    outputImage_.create({.cm = ColorModel::RGBA, .size = outputSize});
}
