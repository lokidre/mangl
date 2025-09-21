/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
﻿#pragma once

#include "animator_types.h"

APEAL_QT_HEADERS_BEGIN
#include <QCheckBox>
#include <QFrame>
#include <QGroupBox>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>
APEAL_QT_HEADERS_END


namespace animator {

using QGroupBoxP = QGroupBox*;
using QPushButtonP = QPushButton*;
using QCheckBoxP = QCheckBox*;
using QLabelP = QLabel*;
using QCheckBoxP = QCheckBox*;
using QSliderP = QSlider*;
using QLineEditP = QLineEdit*;
using QVBoxLayoutP = QVBoxLayout*;
using QSpinBoxP = QSpinBox*;

}  // namespace animator

