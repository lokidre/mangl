/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "animation_info_widget.h"

APEAL_QT_HEADERS_BEGIN
#include <QFormLayout>
APEAL_QT_HEADERS_END

namespace animator {

AnimationInfoWidget::AnimationInfoWidget(QWidget* parent): QWidget(parent)
{
    auto layout = new QFormLayout();

    totalTimeEdit_ = new QLineEdit();
    totalTimeEdit_->setReadOnly(true);

    layout->addRow("Total Time:", totalTimeEdit_);

    layout->setLabelAlignment(Qt::AlignRight);

    setLayout(layout);

    updateContents();
}


void AnimationInfoWidget::updateContents()
{
}

} // namespace animator
