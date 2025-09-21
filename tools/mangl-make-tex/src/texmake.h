/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <apeal/buffer.hpp>
#include <apeal/parse/yaml.hpp>


#include "types.h"
#include "params.h"


#include <map>
#include <unordered_map>
#include <variant>
#include <set>
#include <unordered_set>



struct ImageSource {

    String filename;
    Image* image{};

    RectI sourceRect{};
    RectI targetRect{};
    bool phonyImage{};
    String hash;
    int margin{};

    bool placed{};
};

using ImageSourceCR = const ImageSource&;


// Texture source image data
struct TexSource: ImageSource {
    String variableName;

    bool isArray{};
    Index arrayIndex{}, arraySize{};
};

//using FontParamsValue = std::variant<int, String, std::vector<std::variant>>;
using FontParamsValue = std::variant<int, String, std::vector<int>>;

//using FontParams = std::map<String, std::vector<std::variant<int, String, FontParams>>>;
//using FontParamMapValue = std::unordered_map<String, FontParamsValue>;
using FontParamMapValue = std::vector<std::pair<String, FontParamsValue>>;

inline void insertParamMapValue(const String& key, const FontParamsValue& val, FontParamMapValue *cont) {
    //cont->insert({key, val});
    cont->emplace_back(key, val);
}

using FontParamsVarValue = std::variant<FontParamsValue, FontParamMapValue>;
using FontParamsPair = std::pair<const String, FontParamsVarValue>;
//using FontParams = std::unordered_map<String, FontParamsVarValue>;
using FontParams = std::vector<FontParamsPair>;


struct GlyphSource: public ImageSource {
    FontChar charcode;

    FontParams params;

    void insertParams(const String& key, const FontParamsVarValue& val) {
        params.emplace_back(key, val);
    }
};

enum class FontSourceType {
    Ttf,
    Image,
};

struct FontSource: ImageSource {
    String name;
    String filename;
    bool nosuffix{};

    RectI sourceRect{}, targetRect{};

    String hash;

    FontSourceType type{FontSourceType::Ttf};
    FontParams params;
    std::vector<GlyphSource> glyphs;

    // Constructor to initialize FontSource specific attributes if needed
    FontSource() : ImageSource() {
        placed = false;  // Inherits from ImageSource, but sets default value
    }

    void insertParams(const String& key, const FontParamsVarValue& val) {
        params.emplace_back(key, val);
    }
};


class TexMake {
public:
    TexMakeParams params;

private:
    apl::Image outputImage_;

    std::vector<TexSource> textures_;
    std::multimap<String, int> texByVar_;

    std::vector<FontSource> fonts_;

    std::unordered_map<String, String> defines_;

    // Dependencies
    std::set<String> deps_;


    String clsNamespace, clsName;


public:
    int process();

private:
    // Loading
    void loadDescFile();

    // Building
    void buildDeps();

    void markImagePlaced(ImageSource& src);

    bool buildPlacementsBruteForce();
    bool placeImageSourceBruteForce(ImageSource& src, PointI* current);

    void buildPlacementsRectPack2D();

    void initializeDFS();
    bool buildPlacementsDFS();

    apl::Buffer<char> imageMask_; // buffering


    void buildTextureDesc();
    void buildTextureImage();
    void buildTextureHeader();

    //apl::Image inputImage_;
    apl::Image tempImage_;  // just buffering

    void placeTextureImageSource(ImageSourceCR source, ImageCR inputImage);

    bool canPlaceTexture(const ImageSource& src, const PointI& pos);
    void initializePlacements();


private:
    StringList suffixes_;

    String processResourceFilename(StrView input);
    String processInputFileName(StrView input, StrView defExt, bool nosuffix = false);
    String processOutputFilename(StrView input);
    String preprocessVariable(StrView varName, StrView varValue);
    bool preprocessDefines(String& variable);


    void loadSourceImageInfo(ImageSource& src, StrView filename, int margin);
    Image loadImage(StrView filename);
    void loadImage(StrView filename, apl::ImageContainer* ic);
    void loadSourceImage(ImageSourceCR src, apl::ImageContainer* ic);
    static void scaleImage(const Image& src, Image* dst);
    void bleedImage(Image& image, RectI rect);

    StringList processRange(StrView value);
    void loadFontSource(FontSource& font);
    void loadFontNode(FontSource& font, const apl::YamlNode& fontNode);
    void loadFontNodeTtf(FontSource& font, const apl::YamlNode& fontNode);
    void loadFontNodeImage(FontSource& font, const apl::YamlNode& fontNode);


    bool dfsGuillotine(ImageSource &tex, const PointI &topLeft, const std::pair<int, int> &regionSize);
    static void splitRegion(const PointI &topLeft, const std::pair<int, int> &regionSize, const ImageSource &tex,
                     std::pair<PointI, std::pair<int, int>> &newRegion1,
                     std::pair<PointI, std::pair<int, int>> &newRegion2);




    void splitRegionForFont(const PointI &topLeft, const std::pair<int, int> &regionSize, const ImageSource &font,
                            std::pair<PointI, std::pair<int, int>> &newRegion1,
                            std::pair<PointI, std::pair<int, int>> &newRegion2);



    bool dfsGuillotineForFonts(FontSource &font, const PointI &topLeft, const std::pair<int, int> &regionSize);

    void markFontPosition(const FontSource &font, const PointI &pos);

    bool canPlaceFont(const FontSource &font, const PointI &pos);

    bool canPlaceImageSource(const ImageSource &source, const PointI &pos);
};

