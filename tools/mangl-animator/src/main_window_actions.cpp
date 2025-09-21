/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "main_window.h"

#pragma warning(push, 0)
#include <QAction>
#include <QActionGroup>
#include <QDir>
#include <QInputDialog>
#include <QSettings>
#pragma warning(pop)


#include "main.h"
#include "app.h"
#include "animation_screen.h"
#include "animator_mode.h"
#include "storage.h"


#include <apeal/assert.hpp>



namespace animator {

void MainWindow::createActions()
{
    using namespace apl;

    fileOpenAction_ = new QAction("O&pen...", this);
    connect(fileOpenAction_, &QAction::triggered, this, &Self::onFileOpen);

    exitAction_ = new QAction("E&xit", this);
    connect(exitAction_, &QAction::triggered, this, &Self::onExit);

    editorAction_ = new QAction("Editor...", this);
    connect(editorAction_, &QAction::triggered, this, &Self::onEditorAction);


    auto mode = AnimatorMode::instance();


    //
    // Zoom section
    //
    auto zoom = settings->zoom();
    auto group = new QActionGroup(this);

    bool wasChecked = false;
    for (auto& zi: mode->zoomInfos) {
        zi.action = new QAction(toQt(fmt("%d%%", zi.zoom)), this);
        zi.action->setCheckable(true);
        if (zi.zoom == zoom) {
            zi.action->setChecked(true);
            wasChecked = true;
        }
        group->addAction(zi.action);
        connect(zi.action, &QAction::triggered, this, &Self::onLayoutAction);
    }

    if (!wasChecked) {
        for (auto& zi: mode->zoomInfos) {
            if (zi.zoom == 100) {
                zi.action->setChecked(true);
                settings->zoom = zi.zoom;
                break;
            }
        }
    }
    group->setExclusive(true);


    //
    // Screen size section
    //
    auto screenSizeTag = settings->screenSizeTag();
    group = new QActionGroup(this);

    wasChecked = false;
    QAction* defaultAction{};
    for (auto& r: mode->resolutionInfos) {
        r.action = new QAction(toQt(r.title), this);
        r.action->setCheckable(true);
        if (r.title == screenSizeTag) {
            r.action->setChecked(true);
            wasChecked = true;
        }

        if (r.title == mode->defaultResolution) {
            defaultAction = r.action;
        }


        group->addAction(r.action);
        connect(r.action, &QAction::triggered, this, &Self::onLayoutAction);
    }
    if (!wasChecked) {
        defaultAction->setChecked(true);
    }
    group->setExclusive(true);


    //
    // Orientation setting
    //
    auto isLandscape = settings->orientationLandscape();
    group = new QActionGroup(this);

    orientationPortrait_ = new QAction("Portrait", this);
    orientationPortrait_->setCheckable(true);
    connect(orientationPortrait_, &QAction::triggered, this, &Self::onLayoutAction);
    group->addAction(orientationPortrait_);

    orientationLandscape_ = new QAction("Landscape", this);
    orientationLandscape_->setCheckable(true);
    connect(orientationLandscape_, &QAction::triggered, this, &Self::onLayoutAction);
    group->addAction(orientationLandscape_);

    if (isLandscape)
        orientationLandscape_->setChecked(true);
    else
        orientationPortrait_->setChecked(true);

    group->setExclusive(true);
}

void MainWindow::onEditorAction()
{
    bool ok{};
    auto stored = settings->editorBinPath();
    auto exec = QInputDialog::getText(this, "External Editor", "Executable:", QLineEdit::Normal, toQt(stored), &ok);

    if (ok && !exec.isEmpty()) {
        settings->editorBinPath = fromQt(exec);
    }
}

void MainWindow::onLayoutAction()
{
    auto actionSender = sender();
    auto mode = AnimatorMode::instance();

    // Maybe handle the action sender
    // so if the user chooses the menu it overrides the custom layout
    // think later

    for (auto& r: mode->resolutionInfos) {
        if (actionSender == r.action) {
            settings->screenSizeTag = r.title;
            break;
        }
    }

    if (actionSender == orientationPortrait_)
        settings->orientationLandscape = false;
    else if (actionSender == orientationLandscape_)
        settings->orientationLandscape = true;

    for (auto& zi: mode->zoomInfos) {
        if (actionSender == zi.action) {
            settings->zoom = zi.zoom;
            break;
        }
    }

    mode->onUpdate();
    animatorWidget->onResolutionChanged();
}

}  // namespace animator

