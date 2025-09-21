/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "gui.h"

APEAL_QT_HEADERS_BEGIN
#include <QFileSystemWatcher>
APEAL_QT_HEADERS_END

#include "app.h"

namespace mangl{
class ManglMainWindow;
}

namespace animator {

class AnimatorScreen;

class AnimatorScreenWidget: public QFrame {
    using Base = QFrame;
    using Self = AnimatorScreenWidget;

public:
    AnimatorScreenWidget(QWidget* parent = 0);

    ManglMainWindow* manglWindow_{};

    Size screenSize;
    int frameLineWidth = 2;

    void onResolutionChanged();
    void createScreen();

protected:
    virtual QSize sizeHint() const override {
        return QSize{(int)screenSize.w + frameLineWidth*2, (int)screenSize.h + frameLineWidth*2};
    }

};



class AnimatorWidget: public QFrame {

    using Self = AnimatorWidget;

public:
    AnimatorWidget(QWidget* parent = 0);

    AnimatorScreenWidget* screenWidget_;

    App* app{App::instance()};

    AnimatorScreen* screen_{};

    void onResolutionChanged();
    void onUpdateContents();

private:
    QPushButtonP playButton_{}, stopButton_{}, clearButton_{};

    QCheckBoxP repeatCheckbox_{};
    QSliderP progressSlider_{};
    QVBoxLayoutP mainLayout_{};

    QCheckBoxP wireframeCheckbox_{}, verticesCheckbox_{}, axesCheckbox_{};
    QCheckBoxP boundsCheck_{}, labelsCheck_{};

    QGroupBoxP lightingGroup_;
    QSpinBoxP lightPosition_[3];

    QGroupBoxP layoutGroup_{};
    QLineEditP customLayoutWidth_{}, customLayoutHeight_{};

    // Textures
    QPushButtonP textureDirButton_{};
    QLabelP textureDirLabel_{};


private:
    void onTimer();

};

}  // namespace animator
