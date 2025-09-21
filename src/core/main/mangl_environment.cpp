/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_environment.h"

namespace mangl {


#if MANGL_GAMECENTER
bool Env::gamecenterEnabled{true};
bool Env::gamecenterAuthenticated{true};
#endif

ScreenOrientation Env::orientation{ScreenOrientation::Unknown}; // device orientation at startup

#if APEAL_OS_DESKTOP
DeviceType Env::device{DeviceType::Desktop};
#else
DeviceType Env::device{DeviceType::Phone};
#endif

InterfaceIdiom Env::interfaceIdiom{InterfaceIdiom::Phone};

Size Env::physScreenSize{};  // Physical screen size
Real Env::statusBarHeight{};

Rect Env::applicationRect{InvalidRect};
Padding Env::screenPadding{};

//Size Env::screenSize{};
Size Env::logicalScreenSize{};

Size Env::layoutSize{}; // Application screenLayout in logical units (applicationFrame.w == layoutWidth * stretchValue)
Padding Env::layoutPadding{};  // Padding adjusted to selected screenLayout units

Size Env::screenshotSize{};


Cord Env::pixelDensity{1};
//Cord Env::upscale{1};
Cord Env::screenScale{1};

Cord Env::conversionK_{1};
Cord Env::conversionFlipHeight_{1};

bool Env::fullScreenEnabled_{true};



String Env::docDir;  // application documents
String Env::homeDir;  // application home dir
String Env::bundleDir{"."};  // Bundle dir
String Env::moduleDir;   // Location of the executable
String Env::libDir;  // Equivalent of /var/lib
String Env::cacheDir;
String Env::tempDir;


String Env::appTitle;
String Env::appName, Env::appVersion;

String Env::settingsFile, Env::statsFile;


String Env::windowTitle, Env::windowIcon;

String Env::musicExt, Env::soundExt, Env::textureExt;

String Env::uuid;

int Env::osVersionHi{}, Env::osVersionLo{}, Env::osVersionPatch{};
int Env::osVersionCode{};

}  // namespace mangl
