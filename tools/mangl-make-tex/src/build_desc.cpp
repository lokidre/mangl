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
#include <apeal/parse/yaml.hpp>

#include <variant>

void TexMake::buildTextureDesc() {
    using namespace mangl;
    using namespace apl;

    if (params.outputDescFile.empty())
        return;

    YamlBuilder yaml;

    auto numTextures = textures_.size();
    for (auto& font: fonts_)
        for (auto& glyph: font.glyphs)
            if (glyph.sourceRect.w)
                ++numTextures;

    yaml.obj("textures");

    yaml.seq("file", extractFileName(params.outputImageFile));
    yaml.flowSeq("size", outputImage_.size.w, outputImage_.size.h);
    yaml.field("image_format", outputImage_.colorModelTag());
    yaml.field("density", params.density);
    yaml.field("total_images", numTextures);
    // yaml.field("fonts", fonts_.size());

    if (!texByVar_.empty()) {
        yaml.obj("images");
    }

    String lastVar;
    for (auto& texByVar: texByVar_) {

        auto index = texByVar.second;
        auto& tex = textures_[index];

        if (lastVar != tex.variableName) {
            if (!lastVar.empty()) {
                yaml.pop(); // coords
                yaml.pop(); // -tex:
            }

            // output the variable name
            yaml.seq("name", tex.variableName);
            yaml.field("hash", tex.hash);
            yaml.obj("coords");
        }

        yaml.seqBlockVec("rect", tex.targetRect.shrank(tex.margin * 2).toVector());

        lastVar = tex.variableName;
    }
    if (!lastVar.empty()) {
        yaml.pop(); // coords
        yaml.pop(); // -tex:
    }


    if (!texByVar_.empty()) {
        yaml.pop();
    }

    // yaml.pop(); //images


    /*
    S lastVar;
    for (auto& texByVar : texByVar_) {

        auto index = texByVar.second;
        auto& tex = textures_[index];

        if (lastVar != tex.variableName) {
            //if (!lastVar.empty())
                //file.nl();

            // output the variable name
            yaml.field("tex", tex.variableName);
        }
        yaml.field("",fmt("%d,%d,%,%d",tex.targetRect.x,tex.targetRect.y,tex.targetRect.w,tex.targetRect.h));

        lastVar = tex.variableName;
    }
    */


    if (!fonts_.empty()) {
        yaml.obj("fonts");
    }

    // yaml.obj("fonts");

    auto buildFontParam = [&yaml](FontParamsPair& param) {
        auto& name = param.first;
        auto& val = param.second;

        if (auto pval = std::get_if<FontParamsValue>(&val); pval) {
            if (auto ival = std::get_if<int>(pval); ival) {
                yaml.field(name, *ival);
            } else if (auto sval = std::get_if<String>(pval); sval) {
                yaml.field(name, *sval);
            } else if (auto vval = std::get_if<std::vector<int>>(pval); vval) {
                yaml.flowSeqVec(name, *vval);
            }
            return;
        }

        if (auto mval = std::get_if<FontParamMapValue>(&val); mval) {
            yaml.obj(name);

            for (auto& field: *mval) {
                auto& fname = field.first;
                auto& fval = field.second;
                if (auto ival = std::get_if<int>(&fval); ival) {
                    yaml.field(fname, *ival);
                } else if (auto sval = std::get_if<String>(&fval); sval) {
                    yaml.field(fname, *sval);
                } else if (auto vval = std::get_if<std::vector<int>>(&fval); vval) {
                    yaml.flowSeqVec(fname, *vval);
                }
            }
            yaml.pop();
        }
    };

    // Render font (if any)
    for (auto& font: fonts_) {
        yaml.seq("name", font.name);

        for (auto& param: font.params) {
            buildFontParam(param);
        }


        // Every glyph
        yaml.obj("glyphs");
        for (auto& glyph: font.glyphs) {
            yaml.seq("charcode", M::fontCharCodeFormat(glyph.charcode));

            if (!glyph.phonyImage) {
                yaml.flowSeq("tex", glyph.targetRect.shrank(glyph.margin * 2).toVector());
                // yaml.flowSeq("tex", glyph.targetRect.toVector());
            }

            for (auto& param: glyph.params) {
                buildFontParam(param);
            }
            yaml.pop();
        }

        yaml.pop(); // glyphs
        yaml.pop(); // font
    }

    yaml.pop();

    yaml.pop(); // texture
    if (!fonts_.empty()) {
        yaml.pop();
    }
    // yaml.pop(); // textures

    yaml.end();

    auto yamlStr = yaml.str();
    saveFile(params.outputDescFile, yamlStr);
}
