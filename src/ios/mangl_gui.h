/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#import "mangl_main_vc.h"

#include "../core/mangl_color.h"
#include "../core/native/mangl_native_gui.h"

using namespace mangl;

@interface ManglGuiHelper: NSObject <UITextFieldDelegate>


+ (instancetype)instance;

- (void)alert:(NSString *)message withTitle:(NSString *)title;
- (void)clearWidgets;
- (WidgetId)inputDialog:(NativeGui::InputDialogParam)params;
- (WidgetId)createInput:(NSString *)text frame:(RectA)frame flags:(Flags)flags;
- (WidgetId)createLabel:(NSString *)text frame:(RectA)frame flags:(Flags)flags;

- (void)setInputPlaceholder:(WidgetId)widgetId text:(NSString *)text;
- (void)setText:(WidgetId)widgetId text:(NSString *)text;
- (NSString *)getText:(WidgetId)widgetId;
- (void)setVisible:(WidgetId)widgetId visible:(bool)visible;
- (void)moveWidget:(WidgetId)widgetId point:(mangl::Point)point;
- (void)moveWidget:(WidgetId)widgetId frame:(RectA)frame;
- (void)setTextColor:(WidgetId)widgetId color:(ColorA)color;
- (void)setBackgroundColor:(WidgetId)widgetId color:(Color)color;
- (void)setFont:(WidgetId)widgetId size:(Cord)size fontName:(NSString *)fontName;


@end

