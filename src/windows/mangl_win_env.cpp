/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../core/mangl_environment.h"

#include "mangl_win_util.h"
#include "mangl_win_main_window.h"

#include "../core/mangl_storage.h"

#include <lmcons.h>
#include <shlobj.h>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <apeal/file.hpp>

namespace mangl {

Env::Env()
{
    init();
}

void Env::init() {
    using namespace apl;

    FS fs;

    //
    // Get information about all the directories
    //
    homeDir = ".";
    docDir = ".";
    libDir = ".";
    cacheDir = ".";
    bundleDir = ".";
    moduleDir = "." ;



    tempDir = fs.tmpdir();

    WCHAR winPath[MAX_PATH + 1];

    if (SUCCEEDED(SHGetFolderPathW(0, CSIDL_PERSONAL, 0, 0, winPath))) {
        docDir = apiFromWin(winPath);
    }


    String modulePath, moduleName;

    if (auto moduleHandle = GetModuleHandleW(0)) {
        GetModuleFileNameW(moduleHandle, winPath, MAX_PATH);
        modulePath = apiFromWin(winPath);
        auto [dir, name, ext] = parseFilePath(modulePath);
        moduleDir = dir;
        moduleName = name;
    }


#ifdef MANGL_APP_TAG
    appName = MANGL_APP_TAG;
#else
    appName = moduleName;
#endif
    appVersion = "0.0.1";

#ifdef MANGL_APP_TITLE
    appTitle = MANGL_APP_TITLE;
#else
    appTitle = appName;
#endif


    String varDir = ".";

//#if _DEBUG
    //varDir = "/var";
//#else
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, winPath))) {
        auto appDataDir = apiFromWin(winPath);
        varDir = makeFilePath(appDataDir, "mangl", appName);
    }

    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, winPath))) {
        homeDir = apiFromWin(winPath);
    }


//#endif


    cacheDir = makeFilePath(varDir, "cache");
    libDir = makeFilePath(varDir, "lib");

    //docDir = makeFilePath(varDir, "doc");
    docDir += "\\" + appTitle;

    mkdirIfNotExistsParents(cacheDir);
    mkdirIfNotExistsParents(libDir);
    mkdirIfNotExistsParents(docDir);



    settingsFile = makeFilePath(libDir, "mangl_settings.sqlite");
    statsFile = makeFilePath(libDir, "mangl_stats.sqlite");


    SettingsStorage::instance()->setLocation(settingsFile);
    StatsStorage::instance()->setLocation(statsFile);


    musicExt = "mp3";
    soundExt = "flac"; //"wav";
    textureExt = "png";

    orientation = ScreenOrientation::Portrait;

#if MANGL_ORIENTATION_PORTRAIT
    orientation = Orientation::Portrait;
#elif MANGL_ORIENTATION_LANDSCAPE
    orientation = Orientation::Landscape;
#endif

    processScreenLayout();

    device = DeviceType::Desktop;
    interfaceIdiom = InterfaceIdiom::Desktop;

#if _DEBUG
#if MANGL_DEBUG_IMITATE_PHONE
    device = DeviceType::Phone;
    interfaceIdiom = InterfaceIdiom::Phone;
#elif MANGL_DEBUG_IMITATE_TABLET
    device = DeviceType::Tablet;
    interfaceIdiom = InterfaceIdiom::Tablet;
#endif
#endif  // Debug

    //bundleDir = ".";

    // create the documents dir (sometimes it doesn't exist for an app)
    //fs.mkdirIfNotExists(docDir) ;

    uuid = "1234567890";
}

void Env::processScreenLayout() {
#if !defined(MANGL_NO_GUI) && !defined(MANGL_CONSOLE)
    HWND hwnd = NULL;

#if MANGL_WINMAIN
    auto mainWindow = ManglMainWindow::instance();
    hwnd = mainWindow->hwnd();
#endif

    if (hwnd) {
        auto windowDpi = GetDpiForWindow(hwnd); 

        auto hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi{};
        mi.cbSize = sizeof(mi);

        GetMonitorInfoA(hmon, &mi);

        auto screenWidth = mi.rcMonitor.right - mi.rcMonitor.left;
        auto screenHeight = mi.rcMonitor.bottom - mi.rcMonitor.top;

        pixelDensity = Real(windowDpi) / M::winDefaultDpi;
        physScreenSize = Size::make(screenWidth, screenHeight);

    } else {
        auto hdc = GetDC(0);

        auto systemDpi = GetDpiForSystem();
        auto screenHorzRes = GetDeviceCaps(hdc, HORZRES);
        auto screenVertRes = GetDeviceCaps(hdc, VERTRES);

        pixelDensity = Real(systemDpi) / M::winDefaultDpi;
        physScreenSize = Size::make(screenHorzRes, screenVertRes);

        ReleaseDC(0, hdc);
    }

    logicalScreenSize = physScreenSize / pixelDensity;


    MDebugPrint("Screen: phys: %.0z, logical: %.0z", physScreenSize, logicalScreenSize);
#endif
}


String Env::deviceUserName()
{
    DWORD bufferSize = UNLEN;
    WCHAR buffer[UNLEN + 1];

    if (GetUserNameW(buffer, &bufferSize)) {
        return apl::apiFromWin(buffer);
    }

    return "Unknown";
}

String Env::getStoreAppPageLink() noexcept {  return "https://appstore.com/windows_stub";  }

}  // namespace mangl
