/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

/*
	Widget declarations

*/

namespace mangl {

struct Font;
using FontP = Font*;

struct Widget;
using WidgetP = Widget*;


struct BackgroundWidget;
using BackgroundWidgetP = BackgroundWidget*;


class Button;
using ButtonP = Button*;

class ButtonContentWidget;
using ButtonContentWidgetP = ButtonContentWidget*;


class Checkbox;
using CheckboxP = Checkbox*;


struct Dialog;
using DialogP = Dialog*;


class GridLayout;
using GridLayoutP = GridLayout*;


class Label;
using LabelP = Label*;

class ScrollWidget;
using ScrollWidgetP = ScrollWidget*;

class Segmented;
using SegmentedP = Segmented*;

class SegmentWidget;
using SegmentWidgetP = SegmentWidget*;

class Slider;
using SliderP = Slider*;

class StackWidget;
using StackWidgetP = StackWidget*;

class TableWidget;
using TableWidgetP = TableWidget*;



//
// Effects
//
struct ScaleEffect;
using ScaleEffectP = ScaleEffect*;


}  // namespace mangl

