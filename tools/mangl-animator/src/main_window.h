/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "animator_types.h"

APEAL_QT_HEADERS_BEGIN
#include <QMainWindow>
#include <QListView>
#include <QStatusBar>
#include <QMenu>
#include <QFileSystemWatcher>
#include <QSplitter>
APEAL_QT_HEADERS_END

#include "animation_info_widget.h"
#include "animation_widget.h"
#include "select_widget.h"
#include "storage.h"


namespace animator {

class MainWindow: public QMainWindow {
Q_OBJECT
    using Self = MainWindow;
    using Base = QMainWindow;

public:
    MainWindow(QWidget* parent = {}, Qt::WindowFlags flags = {});

    static MainWindow* instance();

    void show();

public:
    SelectWidget* selectWidget;
    AnimationInfoWidget* infoWidget;
    AnimatorWidget* animatorWidget;
    String setupFilename, workingDirectory;
    int selectedAnimation = -1;

    QFileSystemWatcher* fsWatcher_;

    Settings *settings{Settings::instance()};

    //void onUpdate();

private:
    QStatusBar* statusBar_;
    QSplitter* splitter_;


    void createActions();
    void createMenus();

    void load(StrView filename);


protected:  // events
    void closeEvent(QCloseEvent* e);


private:
    QAction* fileOpenAction_, * exitAction_;
    QAction* editorAction_;
    QAction* orientationPortrait_, * orientationLandscape_;


    void onAbout();
    void onFileOpen();
    void onExit();

    void onEditorAction();
    void onFileChanged();
    void onLayoutAction();


Q_SIGNALS:
    void errorMessage(String);
    void errorMessageQ(QString);


protected Q_SLOTS:
    void onErrorMessage(String);
    void onErrorMessageQ(QString);
};

//extern MainWindow* mainWindow_;

}  // namespace animator
