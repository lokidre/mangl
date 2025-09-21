/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_model.h"

#include "../mangl_common.h"

#include <apeal/file.hpp>
#include <apeal/parse.hpp>

namespace mangl {


Model &Model::loadObjFile(StrView filename) {
    auto fileContents = apl::loadFile(filename);

    return loadObjData(fileContents, filename);
}

Model& Model::loadObjData(StrView fileContents, StrView filename) {

    using namespace apl;

    clear();

    auto c = fileContents.data(), end = c + fileContents.size();
    int currentLineIndex = 0;
    //StringVector components, vertexComponents;
    StrViewVector components, vertexComponents;

    bool textureFlag{}, normalFlag{};

    auto raiseError = [&](StrView message) {
        mthrowError("%s:%d:%s", filename, currentLineIndex, message);
    };

    auto skipWhitespace = [](auto cc, auto end) {
        while (cc != end && (' ' == *cc || '\t' == *cc || '\r' == *cc))
            ++cc;
        return cc;
    };

    auto readLine = [&skipWhitespace](const char *&c, const char *end) {

        //StrView line;

        // determine the line length
        c = skipWhitespace(c, end);

        auto begin = c;
        std::size_t length{};
        for (auto i = c; i != end; ++i, ++length)
            if (*i == '\n' || *i == '#')
                break;


        // Read the line
        while (c != end && *c != '\n')
            ++c;

        // Skip the newline character
        if (c != end)
            ++c;

        // Trim the found line from the back
        while (length > 0) {
            auto&& ch = begin[length-1];
            auto&& ws = (ch == ' ' || ch == '\r' || ch == '\t');
            if (!ws)
                break;
            --length;
        }

        return StrView{begin, length};
        //return c;
    };


    auto parseVertex = [&, this](StrView *&cc, StrView *&cend, FaceVertex &v) -> bool {
        if (cc == cend)
            return false;

        parse_vector(*cc, '/', vertexComponents);

        v.p = parseInt(vertexComponents[0]);

        verify(v.p != 0, "Invalid vertex value: %s", *cc);

        if (v.p < 0)
            v.p += apl::count(vertexPositionsOrig);
        else
            --v.p;

        if (vertexComponents.size() >= 2 && !vertexComponents[1].empty()) {
            v.t = parseInt(vertexComponents[1]);

            apl::verify(v.t != 0, "Texture index can't be zero.");

            if (v.t < 0)
                v.t += apl::count(vertexTexturesOrig);
            else
                --v.t;
            textureFlag = true;
        }

        if (vertexComponents.size() >= 3 && !vertexComponents[2].empty()) {
            parse_int(vertexComponents[2], v.n);
            if (v.n == 0)
                raiseError("Invalid Normal Value");
            else if (v.n < 0)
                v.n += (int)vertexPositionsOrig.size();
            else
                --v.n;
            normalFlag = true;
        }


        ++cc;
        return true;
    };



    vertexPositionsOrig.reserve(20000);
    vertexNormalsOrig.reserve(20000);
    vertexTexturesOrig.reserve(20000);

    for (;;) {
        if (c == end)
            break;
        auto line = readLine(c, end);
        if (line.empty())
            continue;
        ++currentLineIndex;

        if (groups.empty()) {
            groups.emplace_back().faces.reserve(20000);
        }
        auto& currentGroup = groups.back();

        {
            components.clear();
            auto cc = line.data(), cend = cc + line.size();
            cc = skipWhitespace(cc, cend);
            //while (cc != cend && *cc == ' ')
            //    ++cc;
            while (cc != cend) {
                auto begin = cc;
                while (cc != cend && *cc != ' ')
                    ++cc;
                components.push_back({begin, (std::size_t)std::distance(begin, cc)});
                cc = skipWhitespace(cc, cend);
            }
        }

        //str_parse(line,' ',components) ;
        auto cc = components.begin(), cend = components.end();
        ++cc;
        using namespace apl::literals;
        switch (apl::pshHash(components[0])) {

        case "v"_psh: {  // Vertex
            auto &v = vertexPositionsOrig.emplace_back();
            for (int i = 0; i < 3 && cc != cend; ++i)
                v[i] = M::parseRealGL(*cc++);
            break;
        }

        case "vt"_psh: {  // Vertex texture
            auto& vt = vertexTexturesOrig.emplace_back();
            if (cc != cend)
                vt.x = M::parseRealGL(*cc++);

            if (cc != cend)
                vt.y = 1 - M::parseRealGL(*cc++);
            break;
        }

        case "vn"_psh: {  // Vertex normal
            auto& vn = vertexNormalsOrig.emplace_back();
            for (int i = 0; i < 3 && cc != cend; ++i)
                vn[i] = M::parseRealGL(*cc++);
            break;
        }


        //
        // Elements
        //
        case "f"_psh: {  // Face
            textureFlag = normalFlag = false;
            FaceVertex fv{};

            auto& face = currentGroup.faces.emplace_back();
            face.ft = face.fn = true;

            for (;;) {
                if (!parseVertex(cc, cend, fv))
                    break;

                face.v.push_back(fv);

                face.ft &= textureFlag;
                face.fn &= normalFlag;
            }

            break;
        }
        default:
            break;
        }
    }

    vertexPositionsOrig.shrink_to_fit();
    vertexNormalsOrig.shrink_to_fit();
    vertexTexturesOrig.shrink_to_fit();

    vertexPositions = vertexPositionsOrig;
    vertexNormals = vertexNormalsOrig;
    vertexTextures = vertexTexturesOrig;

    normalsGenerated_ = !vertexNormals.empty();

    // check if the normals are provided, but the faces don't have the normal index
    if (vertexPositions.size() == vertexNormals.size()) {
        for (auto &g: groups) {
            for (auto &face: g.faces) {
                if (!face.fn) {
                    for (auto& v: face.v)
                        v.n = v.p;
                    face.fn = true;
                }
            }  // faces
        }  // groups
    }

    for (auto &g: groups)
        g.faces.shrink_to_fit();

    return *this;
}

}  // using namespace mangl

