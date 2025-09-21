/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "main_window.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>

#include "animator_mode.h"


namespace animator {

void MainWindow::createMenus()
{
    using namespace apl;

    QMenuBar* mb = menuBar();
    QMenu* m;
    QAction* a;

    //ManglEnvironment *env = ManglEnvironment::instance() ;

    //
    // Standard File Menu
    //
    m = mb->addMenu("&File");
    m->addAction(fileOpenAction_);
    m->addSeparator();
    m->addAction(exitAction_);


    // 
    // Tools/Options
    //
    m = mb->addMenu("Tools");
    m->addAction(editorAction_);


    //
    // View
    //
    auto mode = AnimatorMode::instance();

    m = mb->addMenu("View");
    auto lastFamily = mode->resolutionFamilies.begin()->first;
    auto rm = m->addMenu(toQt(mode->resolutionFamilies.begin()->second));
    for (auto& r : mode->resolutionInfos) {
        if (r.family != lastFamily) {
            rm = m->addMenu(toQt(mode->resolutionFamilies[r.family]));
            lastFamily = r.family;
        }
        rm->addAction(r.action);
    }


    m->addSeparator();

    m->addAction(orientationPortrait_);
    m->addAction(orientationLandscape_);


    m->addSeparator();

    auto zm = m->addMenu("Zoom");
    for (auto& z: mode->zoomInfos)
        zm->addAction(z.action);

    //
    // Help
    //
    m = mb->addMenu(tr("&Help"));
    a = new QAction(tr("About"), this);
    connect(a, &QAction::triggered, this, &MainWindow::onAbout);
    m->addAction(a);
}

}  // namespace animator
