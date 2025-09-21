/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl_test.h>

#include <tex_tutorial.hpp>

class TutorialScreenImage1: public TutorialScreenBase<Screen> {
    using Base = TutorialScreenBase<Screen>;

    TutorialTexture tex_;

public:
    TutorialScreenImage1() {
        Base::loadTempTexture(getTutorialTexBundlePath(), &tex_);

        //setup({
        //    .pageImage = tex_.tutorialImage1,
        //});

        setup(tex_.tutorialImage1);
    }
};

class TutorialScreenImage2: public TutorialScreenBase<Screen> {
    using Base = TutorialScreenBase<Screen>;
    TutorialTexture tex_;

public:
    TutorialScreenImage2() {
        Base::loadTempTexture(getTutorialTexBundlePath(), &tex_);

        setup({
            .titleText = "TUTORIAL IMG-2",
            .pageImages = {tex_.tutorialImage1, tex_.tutorialImage2},
        });
    }
};


class TutorialScreenImageN: public TutorialScreenBase<Screen> {
    using Base = TutorialScreenBase<Screen>;
    TutorialTexture tex_;

public:
    TutorialScreenImageN() {
        Base::loadTempTexture(getTutorialTexBundlePath(), &tex_);

        setup({
            .titleText = "TUTORIAL IMG-N",
            //.closeButton = true,
            //.backButton = true,
            .pages = {
                {
                    .image = tex_.tutorialImage1,
                },

                {
                    .image = tex_.tutorialImage2,
                },
            },
        });
    }
};


class TutorialScreenAnimation: public TutorialScreenBase<Screen> {
public:
    TutorialScreenAnimation() {
        setup({
            .titleText = "TUTORIAL ANI",
            .closeButton = true,
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

    //gridStart({
    //    .cols = 1,
    //});

    gridAddScreenButton({
        .text = "Tutorial Image 1",
        .onPush = App::pushScreenCreate<Self, TutorialScreenImage1>,
    });

    gridAddScreenButton({
        .text = "Tutorial Image 2",
        .onPush = App::pushScreenCreate<Self, TutorialScreenImage2>,
    });


    gridAddScreenButton({
        .text = "Tutorial Images N",
        .onPush = App::pushScreenCreate<Self, TutorialScreenImageN>,
    });

    gridAddScreenButton({
        .text = "Tutorial Animation",
        .onPush = App::pushScreenCreate<Self, TutorialScreenImageN>,
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

