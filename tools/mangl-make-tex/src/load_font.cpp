/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "texmake.h"

#include <apeal/file.hpp>
#include <apeal/hash.hpp>
#include <apeal/str/case.hpp>


void TexMake::loadFontSource(FontSource& font) {
    using namespace apl;

    font.params.clear();

    // auto fontFile = processInputFileName(font.filename, "yaml");

    // auto yaml = loadYaml(fontFile);

    auto yaml = loadYaml(font.filename);

    for (auto& docNode: yaml.front()) {
        if ("font" == docNode.key) {
            loadFontNode(font, docNode);
        }
    }
}

void TexMake::loadFontNode(FontSource& font, const apl::YamlNode& fontNode) {
    using namespace apl;

    String typeString;

    for (auto& node: fontNode) {
        switch (pshHash(node.key)) {
        case "type"_psh: node.get(&typeString); break;
        default:
            break;
        }
    }

    typeString = str_lower(typeString);
    if (typeString == "ttf" || typeString == "otf" || typeString.empty()) {
        font.type = FontSourceType::Ttf;
    } else if (typeString == "img" || typeString == "image") {
        font.type = FontSourceType::Image;
    } else {
        throwError("Invalid font.type: %s", typeString);
    }

    if (font.type == FontSourceType::Ttf) {
        loadFontNodeTtf(font, fontNode);
    } else if (font.type == FontSourceType::Image) {
        loadFontNodeImage(font, fontNode);
    }

}


void TexMake::loadFontNodeTtf(FontSource& font, const apl::YamlNode& fontNode) {
    using namespace apl;

    using std::ranges::for_each;
    using namespace mangl;

    auto dirname = extractDirName(font.filename);

    std::vector<int> p;

    for (auto& node: fontNode) {
        switch (pshHash(node.key)) {
        case "file"_psh: break;
        case "div"_psh:
        case "height"_psh:
            font.insertParams(node.key, node.getInt());
            break;
        case "face"_psh: {
            FontParamMapValue val;

            for (auto& faceNode: node) {
                switch (pshHash(faceNode.key)) {
                case "family"_psh:
                case "style"_psh:
                    insertParamMapValue(faceNode.key, faceNode.getString(), &val);
                    break;

                case "bbox"_psh:
                    p.clear();
                    for_each(faceNode.nodes, [&p](auto& n) { p.push_back(n.getInt()); });
                    insertParamMapValue(faceNode.key, p, &val);
                    break;

                case "units_per_em"_psh:
                case "ascent"_psh:
                case "descent"_psh:
                case "height"_psh:
                case "max_advance"_psh:
                case "underline_position"_psh:
                case "underline_thickness"_psh:
                    insertParamMapValue(faceNode.key, faceNode.getInt(), &val);
                    break;

                default: break;
                }
            }

            // font.params.insert({node.key, val});
            font.insertParams(node.key, val);
            break;
        }
        case "metrics"_psh: {
            FontParamMapValue val;

            for (auto& metricsNode: node) {
                switch (pshHash(metricsNode.key)) {
                case "ppem"_psh:
                case "scale"_psh:
                    p.clear();
                    for_each(metricsNode.nodes, [&p](auto& n) { p.push_back(n.getInt()); });
                    insertParamMapValue(metricsNode.key, p, &val);
                    break;
                case "ascent"_psh:
                case "descent"_psh:
                case "height"_psh:
                case "max_advance"_psh:
                    // val.insert({metricsNode.key, metricsNode.getInt()});
                    insertParamMapValue(metricsNode.key, metricsNode.getInt(), &val);
                    break;

                default: break;
                }

            } // for

            font.insertParams(node.key, val);
            break;
        }
        case "glyphs"_psh: {

            for (auto& glyphsNodes: node) {
                auto& glyph = font.glyphs.emplace_back();
                glyph.margin = params.margin;

                for (auto& glyphNode: glyphsNodes) {
                    switch (pshHash(glyphNode.key)) {
                    case "charcode"_psh:
                        glyph.charcode = M::fontCharCodeParse(glyphNode.getString());
                        break;

                    case "bitmap_size"_psh:
                        p.clear();
                        for_each(glyphNode.nodes, [&p](auto& n) { p.push_back(n.getInt()); });

                        // glyph.sourceRect = {0, 0, p[0], p[1]};
                        // if (glyph.sourceRect.w == 0) {
                        //     glyph.phonyImage = true;
                        // } else {
                        //     glyph.sourceRect.expand(glyph.bleeding * 2);
                        // }
                        //
                        // glyph.targetRect = glyph.sourceRect;

                        glyph.insertParams(glyphNode.key, p);
                        break;
                    case "bitmap"_psh:
                        glyph.filename = makeFilePath(dirname, glyphNode.getString());
                        break;

                    case "bitmap_rect"_psh:
                        p.clear();
                        for_each(glyphNode.nodes, [&p](auto& n) { p.push_back(n.getInt()); } );
                        glyph.sourceRect = {p[0], p[1], p[2], p[3]};

                        if (glyph.sourceRect.w == 0) {
                            glyph.phonyImage = true;
                        } else {
                            glyph.sourceRect.expand(glyph.margin * 2);
                            glyph.targetRect = glyph.sourceRect;
                        }

                        glyph.insertParams(glyphNode.key, p);
                        break;

                    case "bitmap_margin"_psh:
                    case "glyph_size"_psh:
                    case "horz_bearing"_psh:
                    case "vert_bearing"_psh:
                    case "advance"_psh:
                        p.clear();
                        for_each(glyphNode.nodes, [&p](auto& n) { p.push_back(n.getInt()); });
                        glyph.insertParams(glyphNode.key, p);
                        break;

                    default: break;
                    }
                }
            } // for

            break;
        }
        default: break;
        }
    }
}

void TexMake::loadFontNodeImage(FontSource& font, const apl::YamlNode& fontNode) {
    using namespace apl;
    using namespace mangl;

    for (auto& node: fontNode) {
        switch (pshHash(node.key)) {
        case "glyphs"_psh: {

            for (auto& glyphsNodes: node) {
                unsigned glyphChar{};
                std::vector<String> imageFiles;

                for (auto& glyphNode: glyphsNodes) {
                    switch (pshHash(glyphNode.key)) {
                    //case "char"_psh: glyphNode.get(&glyphChar); break;
                    case "char"_psh:
                        glyphChar = glyphNode.value[0];
                        break;

                    default:
                        break;
                    }
                }

                for (auto& glyphNode: glyphsNodes) {
                    switch (pshHash(glyphNode.key)) {
                    case "file"_psh:
                    case "files"_psh:
                        for (auto& glyphFile: glyphNode) {
                            if (glyphFile.isScalar()) {
                                imageFiles.push_back(glyphFile.getString());
                            } else {
                                for (auto& fileNode: glyphFile) {
                                    imageFiles.push_back(fileNode.getString());
                                }
                            }
                        }
                        break;

                    default:
                        break;
                    }
                }

                for (auto& imageFile: imageFiles) {
                    auto range = processRange(imageFile);

                    for (auto& fn: range) {
                        auto& glyph = font.glyphs.emplace_back();
                        glyph.charcode = glyphChar;
                        glyph.filename = processInputFileName(fn, "png");

                        glyphChar++;
                    }
                }
            }

            break;
        }

        default:
            break;
        }
    }

}
