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

#include <apeal/singleton.hpp>

#if QT_GUI_LIB
class QWidget;
#endif

namespace mangl {

class ManglMainWindow: public apl::Singleton<ManglMainWindow> {
public :
    void destroy() {}
    bool exitOnClose = false;
	
#if QT_GUI_LIB
    void createQt(QWidget *parent, RectI frame, int flags);
#endif
} ;

} // namespace mangl

#ifdef __OBJC__

#import <Cocoa/Cocoa.h>
#import "mangl_main_view.h"

@interface ManglMainWindowObjC: NSWindow<NSWindowDelegate>


-(void)onLaunch;
-(void)onTerminate;

@end

#endif  // if Objective C

