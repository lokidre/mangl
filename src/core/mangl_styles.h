/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "styles/mangl_styles_palette.h"
#include "styles/mangl_styles_button.h"
#include "styles/mangl_styles_text.h"

#include "mangl_texture.h"

#include "widgets/mangl_background_widget_params.h"
#include "mangl_widgets_decl.h"

#include "widgets/mangl_label_styles.h"
#include "widgets/mangl_table_styles.h"


#include "game/mangl_game_screen_style.h"


namespace mangl {

struct BodyTheme {
    Color backgroundColor{ThemeColors::BackgroundPaper};
};


struct GridTheme {
    Size spacing{4, 4};
};

struct SliderStyles {
    Texture textures{};
};

struct SegmentedTheme {
    Cord textSize{14};

    Color textColor{ThemeColors::PrimaryContrastText};
    Color pressedTextColor{ThemeColors::PrimaryLight};
    Color disabledTextColor{0x9e9e9e};
    Color selectedTextColor{};

    Color backgroundColor{ThemeColors::PrimaryMain};
    Color pressedBackgroundColor{ThemeColors::PrimaryDark};
    Color disabledBackgroundColor;
    Color selectedBackgroundColor;

    ThemeBorder border{.radius = 4};

    Texture segmentTex{};
    Texture segmentFirstTex{};
    Texture segmentLastTex{};
};


struct CheckboxTheme {
    Cord textSize{16};

    Color textColor{ThemeColors::PrimaryContrastText};
    Color pressedTextColor{ThemeColors::PrimaryLight};
    Color disabledTextColor{0x9e9e9e};

    Color backgroundColor{ThemeColors::PrimaryMain};
    Color pressedBackgroundColor{ThemeColors::PrimaryDark};
    Color disabledBackgroundColor;

    //ThemeBorder border{4};

    Cord borderRadius{4};
    Color borderColor{ThemeColors::PrimaryMain};
    Cord borderWidth{1};

    Texture texture{};
};

struct DialogTheme {
    //
    // Background and decorations
    //
    Color backgroundColor{ThemeColors::Grey800};
    BackgroundWidgetParams backgroundWidgetParams{};
    ThemeBorder border{.radius = 4};
    std::function<WidgetP(DialogP)> decorCreateCallback{};

    Margins margins{};
    Padding padding{.a = 12};
    Padding contentsPadding{};
    Size spacing{12, 12};  // Elements spacing inside the dialog box

    Size smallSize{280, 160};  // Dialog sizes

    TextStyle titleText{.color = ThemeColors::TextPrimary, .size = 18};

    TextStyle contentsText{.color = ThemeColors::Grey300, .size = 14};

    ButtonStyle button{.size = {100, 34}};

    void clear() {
        DialogTheme th{};
        std::swap(*this, th);
    }
};



namespace M {

struct Styles {
    static ThemePalette palette;
    static BodyTheme body;
    static LabelTheme label;

    static ButtonStyle button;
    static CheckboxTheme checkbox;
    static SliderStyles slider;
    static SegmentedTheme segmented;
    static TableStyles table;
    static GridTheme grid;

    static DialogTheme dialog;
    static GameScreenStyles screen;
};

void loadDefaultStyles();

}  // namespace M

}  // namespace mangl
