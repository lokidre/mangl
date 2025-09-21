/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "texmake.h"

#include <apeal/fs.hpp>

void TexMake::buildTextureHeader() {

    apl::FileWriteFormat f{params.outputHeaderFile};

    f.writeln("// Automatically Generated. DO NOT EDIT");
    f.writeln("#pragma once");
    f.nl();

    f.writeln("#include <mangl/texture.h>");
    f.nl();



    if (!clsNamespace.empty()) {
        f.writeln("namespace %s {", clsNamespace);
        f.nl();
    }

    apl::verify(!clsName.empty(), "class: name is missing");

    f.writeln("class %s final: public mangl::TextureAtlas {", clsName);
    f.writeln("    using Base = mangl::TextureAtlas;");
    f.nl();
    f.writeln("public:");
    f.writeln("    using Texture = mangl::Texture;");
    f.writeln("    using Font = mangl::Font;");
    f.writeln("    using StrView = mangl::StrView;");
    f.nl();

    String lastVar{};
    for (auto& tex: textures_) {
        if (tex.variableName == lastVar)
            continue;
        f.writeln("    Texture %s{};", tex.variableName);
        lastVar = tex.variableName;
    }

    for (auto& font: fonts_) {
        f.writeln("    Font %s{};", font.name);
    }

    f.nl();

    //f.writeln("public:");
    f.nl();
    f.writeln("    void load(StrView filename) override {");
    f.writeln("        Base::load(filename);");
    f.nl();

    lastVar.clear();
    for (auto& tex: textures_) {
        if (tex.variableName == lastVar)
            continue;
        f.writeln("        %s = get(\"%s\");", tex.variableName, tex.variableName);
        lastVar = tex.variableName;
    }

    for (auto& font: fonts_) {
        f.writeln("        %s = getFont(\"%s\");", font.name, font.name);
    }

    f.nl();
    f.writeln("    }");
    f.nl();

    f.writeln("};");
    f.nl();


    if (!clsNamespace.empty()) {
        f.writeln("}  // namespace %s", clsNamespace);
        f.nl();
    }


    f.commit();
}

