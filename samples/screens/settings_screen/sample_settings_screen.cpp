/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl_test.h>


class SettingsScreenAudioTexture: public SettingsScreenBase<Screen> {
public:
    SettingsScreenAudioTexture() {
        setup({
            .items = {
                {
                    .type = GameSettingsItemType::Sounds,
                    .checkboxTexture = texCommon.settingsSoundsCheckbox,
                    .sliderTexture = texCommon.settingsSlider,
                },

                {
                    .type = GameSettingsItemType::Music,
                    .checkboxTexture = texCommon.settingsMusicCheckbox,
                    .sliderTexture = texCommon.settingsSlider,
                },
            },

        });
    }
};

class SettingsScreenAudioDefault: public SettingsScreenBase<Screen> {
public:
    SettingsScreenAudioDefault() {
        setup({
            .items = {
                {
                    .type = GameSettingsItemType::Sounds,
                },

                {
                    .type = GameSettingsItemType::Music,
                },
            },

        });
    }
};




class TestScreen: public Screen {
    using Self = TestScreen;


protected:
    void onStart() override;
};

void TestScreen::onStart()
{
    buildSampleBackground();

    gridAddScreenButton({
        .text = "Audio Textures",
        .onPush = App::pushScreenCreate<Self, SettingsScreenAudioTexture>,
    });

    gridAddScreenButton({
        .text = "Audio Default",
        .onPush = App::pushScreenCreate<Self, SettingsScreenAudioDefault>,
    });

}

void App::onStart()
{
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

