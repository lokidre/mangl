/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "select_widget.h"

#include "animator.h"
#include "main.h"
#include "main_window.h"
#include "app.h"
#include "animation_screen.h"
#include "storage.h"

#include <mangl/storage.h>


#pragma warning(push, 0)
#include <QFileDialog>
#include <QLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QProcess>
#pragma warning(pop)

#include <apeal/file.hpp>

namespace animator {

SelectWidget::SelectWidget(QWidget* parent): QWidget(parent)
{
    animator_ = Animator::instance();

    auto layout = new QVBoxLayout;

    tabbar_ = new QTabBar;
    //tabbar_->addTab("Background");
    tabbar_->addTab("Background");
    tabbar_->addTab("Blueprint");
    tabbar_->addTab("Animation");
    tabbar_->addTab("Model");
    connect(tabbar_, &QTabBar::currentChanged, this, &Self::onTabbarCurrentChanged);

    tabbar_->setCurrentIndex(settings->currentTabIndex());
    layout->addWidget(tabbar_);


    listWidget_ = new SelectListWidget;
    dataModel_ = new SelectListDataModel(this);
    listWidget_->setModel(dataModel_);
    listWidget_->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(listWidget_, &SelectListWidget::clicked, this, &Self::onAnimationClicked);
    connect(listWidget_, &SelectListWidget::doubleClicked, this, &Self::onAnimationDoubleClicked);
    layout->addWidget(listWidget_);


    auto buttonsLayout = new QHBoxLayout;

    addButton_ = new QPushButton("Add");
    connect(addButton_, &QPushButton::clicked, this, &Self::onAddButton);
    buttonsLayout->addWidget(addButton_);

    removeButton_ = new QPushButton("Remove");
    connect(removeButton_, &QPushButton::clicked, this, &Self::onRemoveButton);
    buttonsLayout->addWidget(removeButton_);

    buttonsLayout->addStretch();

    clearButton_ = new QPushButton("Edit");
    connect(clearButton_, &QPushButton::clicked, this, &Self::onEditButton);
    buttonsLayout->addWidget(clearButton_);

    layout->addLayout(buttonsLayout);

    setLayout(layout);
}


void SelectWidget::onAddButton()
{
    QFileDialog fd;

    fd.restoreState(toQByteArray(settings->lastOpenFileDialogState()));

    fd.setDirectory(toQt(settings->lastOpenDir()));

    fd.setNameFilter(currentTab() == TabIndexModel ? "Model (*.obj)" : "Schema (*.ini; *.yaml)");
    fd.setFileMode(QFileDialog::ExistingFiles);

    if (fd.exec()) {
        auto selectedFiles = fd.selectedFiles();

        dataModel_->beginUpdates();

        for (auto& fn: selectedFiles) {
            animator_->appendFile(currentTab(), fromQt(fn));
        }

        animator_->saveState();
        dataModel_->endUpdates();

        settings->lastOpenFileDialogState = fromQByteArray(fd.saveState());
        settings->lastOpenDir = fromQt(fd.directory().canonicalPath());
    }

}


void SelectWidget::onRemoveButton()
{
    auto sm = listWidget_->selectionModel();
    auto il = sm->selectedRows();
    if (il.empty())
        return;

    dataModel_->beginUpdates();

    QListIterator<QModelIndex> it(il);
    it.toBack();
    int tab = currentTab();
    while (it.hasPrevious()) {
        auto& idx = it.previous();
        if (!idx.isValid())
            continue;
        animator_->unload(tab, idx.row());
    }

    animator_->saveState();

    dataModel_->endUpdates();
}

void SelectWidget::onAnimationClicked(const QModelIndex& index)
{
    try {
        //auto filename = getItems()[index.row()].file;
        //animator_->start(currentTab(), filename);
        animator_->start(currentTab(), index.row());
        MainWindow::instance()->animatorWidget->onUpdateContents();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error!", e.what(), QMessageBox::Ok);
    }
    catch (...) {
        QMessageBox::critical(this, "Error!", "Unhandled Exception!", QMessageBox::Ok);
    }
}


void SelectWidget::onAnimationDoubleClicked(const QModelIndex& index)
{
    auto editorCommandline = toQt(settings->editorBinPath());
    auto editFile = toQt(getItems()[index.row()].file);

    QStringList args(editFile);
    QProcess::startDetached(editorCommandline, args);
}

void SelectWidget::onEditButton()
{
    auto sm = listWidget_->selectionModel();
    auto il = sm->selectedRows();
    if (il.empty())
        return;

    auto editorCommandline = toQt(settings->editorBinPath());

    for (auto& idx: il) {
        if (!idx.isValid())
            continue;
        auto editFile = toQt(getItems()[idx.row()].file);

        QStringList args(editFile);
        QProcess::startDetached(editorCommandline, args);
    }
}


void SelectWidget::onTabbarCurrentChanged(int currentIndex)
{
    if (dataModel_) {
        dataModel_->beginUpdates();
        dataModel_->endUpdates();
    }

    settings->currentTabIndex = currentIndex;

    Animator::instance()->onTabChanged(currentIndex);
}

}  // namespace animator
