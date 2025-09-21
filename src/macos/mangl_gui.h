/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../core/mangl_types.h"
#include "../core/mangl_color.h"

#include <Foundation/Foundation.h>


@interface ManglGuiHelper: NSObject

+(instancetype)instance;

-(void)clearWidgets ;
-(mangl::WidgetId)createInput:(NSString *)text frame:(mangl::Rect)frame flags:(mangl::Flags)flags;
-(mangl::WidgetId)createLabel:(NSString *)text frame:(mangl::Rect)frame flags:(mangl::Flags)flags;

-(void)setInputPlaceholder:(mangl::WidgetId)widgetId text:(NSString *)text;
-(void)setText:(mangl::WidgetId)widgetId text:(NSString *)text;
-(NSString *)getText:(mangl::WidgetId)widgetId;
-(void)setVisible:(mangl::WidgetId)widgetId visible:(bool)visible;
-(void)moveWidget:(mangl::WidgetId)widgetId point:(mangl::Point)point;
-(void)moveWidget:(mangl::WidgetId)widgetId frame:(mangl::Rect)frame;
-(void)setTextColor:(mangl::WidgetId)widgetId color:(mangl::Color)color;
-(void)setBackgroundColor:(mangl::WidgetId)widgetId color:(mangl::Color)color;
-(void)setFont:(mangl::WidgetId)widgetId size:(mangl::Cord)size fontName:(NSString *)fontName;

// Utilities
//+(mangl::Cord)nativeGuiCoordsK;

@end

