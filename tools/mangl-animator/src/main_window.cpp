/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "main_window.h"

APEAL_QT_HEADERS_BEGIN
#include <QFileDialog>
#include <QIcon>
#include <QLayout>
#include <QMenu>
#include <QMessageBox>
#include <QSplitter>
#include <QStatusBar>
#include <QTimer>
APEAL_QT_HEADERS_END


#include "animator.h"
#include "main.h"
#include "app.h"
#include "animation_screen.h"
#include "storage.h"


#include <apeal/file.hpp>
#include <cognit/func.hpp>

namespace animator {

static MainWindow* mainWindow_{};

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags): QMainWindow(parent, flags)
{
    mainWindow_ = this;

    App::instance();
    Env::instance();

    setWindowIcon(QIcon(":icon"));
    setWindowTitle(PackageTitle);

    statusBar_ = statusBar();
    statusBar_->setSizeGripEnabled(true);


    createActions();
    createMenus();

    splitter_ = new QSplitter;
    {
        auto handle = splitter_->handle(1);

        auto layout = new QVBoxLayout(handle);
        layout->setSpacing(0);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addStretch();

        auto line = new QFrame(handle);
        line->setFrameShape(QFrame::StyledPanel);
        line->setFrameShadow(QFrame::Sunken);
        layout->addWidget(line);
    }


    auto levelsLayout = new QVBoxLayout;

    selectWidget = new SelectWidget;
    levelsLayout->addWidget(selectWidget);

    infoWidget = new AnimationInfoWidget;
    levelsLayout->addWidget(infoWidget);

    auto layoutWidget = new QWidget;
    layoutWidget->setLayout(levelsLayout);

    splitter_->addWidget(layoutWidget);

    animatorWidget = new AnimatorWidget;
    splitter_->addWidget(animatorWidget);

    splitter_->setStretchFactor(0, 1);
    splitter_->setStretchFactor(1, 2);

    setCentralWidget(splitter_);

    statusBar_->showMessage("Ready.");

    Animator::instance()->setMainWindow(this);

    fsWatcher_ = new QFileSystemWatcher;
    connect(fsWatcher_, &QFileSystemWatcher::fileChanged, this, &Self::onFileChanged);


    connect(this, &Self::errorMessage, this, &Self::onErrorMessage);
    connect(this, &Self::errorMessageQ, this, &Self::onErrorMessageQ);
}

MainWindow* MainWindow::instance()
{
    return mainWindow_;
}

void MainWindow::show()
{
    selectedAnimation = settings->selectedAnimation();

    // Restore settings
    restoreState(toQByteArray(settings->mainWindowState()));
    restoreGeometry(toQByteArray(settings->mainWindowGeometry()));

    setupFilename = settings->currentFile();
    workingDirectory = settings->workingDirectory();

    splitter_->restoreState(toQByteArray(settings->mainSplitterState()));

    QMainWindow::show();
}


void MainWindow::load(StrView filename)
{
    using namespace apl;

    try {
        setupFilename = filename;
        workingDirectory = extractDirName(setupFilename);

        FileSystem::cd(workingDirectory);
        //changeDirectory(workingDirectory);

        //Animator::instance()->load(selectWidget->currentTab(), filename);
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error!", e.what());
    }
    catch (...) {
        QMessageBox::critical(this, "Error!", "Unhandled Exception!");
    }
}


void MainWindow::closeEvent(QCloseEvent*)
{
    // Windows geometry and state
    settings->mainWindowState = fromQByteArray(this->saveState());
    settings->mainWindowGeometry = fromQByteArray(this->saveGeometry());
    settings->mainSplitterState = fromQByteArray(splitter_->saveState());


    // Various states
    settings->currentFile = setupFilename;
    settings->workingDirectory = workingDirectory;
    settings->selectedAnimation = selectedAnimation;
}

void MainWindow::onExit()
{
    close();
}


void MainWindow::onFileOpen()
{
    auto filter = "Schema Files (*.ini; *.yaml)";
    auto file = QFileDialog::getOpenFileName(this, "Open File", toQt(workingDirectory), filter);

    if (!file.isEmpty()) {
        load(fromQt(file));
        settings->workingDirectory = workingDirectory;
    }
}


void MainWindow::onFileChanged()
{
    //apl::sleep(300);

    // Give some time while file is loading
    QTimer::singleShot(400, [&] {
        try {
            App::instance()->animatorScreen->reload();
        }
        catch (const std::exception& e) {
            Q_EMIT errorMessage(e.what());
        }
        catch(...) {
            Q_EMIT errorMessage("Unhandled Exception");
        }

    });

    /*
    try {
        App::instance()->animatorScreen->reload();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error!", e.what(), QMessageBox::Ok);
    }
    catch (...) {
        QMessageBox::critical(this, "Error!", "Unhandled Exception!", QMessageBox::Ok);
    }
    */
}

void MainWindow::onErrorMessage(String text)
{
    QMessageBox::critical(this, "Error!", apl::toQt(text), QMessageBox::Ok);
}


void MainWindow::onErrorMessageQ(QString m)
{
    QMessageBox::critical(this, "Error!", m, QMessageBox::Ok);
}


void MainWindow::onAbout()
{
    using namespace apl;

    auto html = R"(
<p>
<font size='+2'><i>%s</i></font>
</p>

<p>
</p>
)";

    auto text = fmt(html, PackageTitle);

    QMessageBox::about(this, "About", toQt(text));
}


} // namespace animator
