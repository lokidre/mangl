/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../core/mangl_setup.h"

#if MANGL_GTK
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#endif

#if MANGL_SDL
#if MANGL_SDL3
#include <SDL3/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#endif

#include "../core/mangl_environment.h"

#include "../core/mangl_storage.h"
#include "../core/mangl_error.h"

#include <apeal/file.hpp>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

namespace mangl {

Env::Env() {
    using namespace apl;

    FileSystem fs;


    // TODO: implement in apeal::fs class (fs.homedir())
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    homeDir = homedir;

    String name;

#ifdef MANGL_APP_TAG
    name = MANGL_APP_TAG;
#else

    constexpr std::size_t bufferSize = PATH_MAX + 1;
    char buf[bufferSize + 1];
    int res = readlink("/proc/self/exe", buf, bufferSize);
    buf[res] = 0;
    massert(res > 0, "Cannot read /proc/self/exe");
    name = extractFileName(buf);

#endif


    appName = name;

#ifdef MANGL_APP_TITLE
    appTitle = MANGL_APP_TITLE;
#else
    appTitle = appName;
#endif

#ifdef MANGL_APP_VERSION
    appVersion = MANGL_APP_VERSION;
#else
    appVersion = "0.0.1";
#endif


    tempDir = FS::tmpdir();

    //String varDir = "/var";
    String varDir = makeFilePath(homeDir, "var");

    cacheDir = makeFilePath(varDir, "cache", name);
    libDir = makeFilePath(varDir, "lib", name);

    auto homeDocumentsDir = makeFilePath(homedir, "Documents");

    if (fileExists(homeDocumentsDir))
        docDir = makeFilePath(homeDocumentsDir, name);
    else
        docDir = makeFilePath(varDir, "doc", name);

    //bundleDir = makeFilePath(varDir, "bundle", name);
    bundleDir = "./bundle";

    mkdirIfNotExistsParents(cacheDir);

    mkdirIfNotExistsParents(libDir);
    //mkdirIfNotExistsParents(docDir);

    settingsFile = makeFilePath(libDir, "mangl_settings.sqlite");
    statsFile = makeFilePath(libDir, "mangl_stats.sqlite");


    SettingsStorage::instance()->setLocation(settingsFile);
    StatsStorage::instance()->setLocation(statsFile);

#if MANGL_GTK
    GdkRectangle workarea{};
    auto display = gdk_display_get_default();
    auto monitors = gdk_display_get_monitors(display);


    auto numMonitors = g_list_model_get_n_items(monitors);

    auto monitor = (GdkMonitor *)g_list_model_get_item(monitors, 0);
    gdk_monitor_get_geometry(monitor, &workarea);

    physScreenSize = Size::make(workarea.width, workarea.height);
#endif

#if MANGL_SDL
#if MANGL_SDL3
    SDL_DisplayMode dm{};
    int displaysCount = 0;
    auto displays = SDL_GetDisplays(&displaysCount);
    for (auto i = 0; i < displaysCount; ++i) {
        auto pdm = SDL_GetCurrentDisplayMode(displays[i]);
        // TODO: determine which is current display
        if (pdm)
            dm = *pdm;
    }
    //auto displayId = SDL_GetCurrentDisplay();
    //auto& dm = *pdm;
#else
    SDL_DisplayMode dm{};
    SDL_GetCurrentDisplayMode(0, &dm);
#endif

    physScreenSize = Size::make(dm.w, dm.h);
#endif


    logicalScreenSize = physScreenSize;

    interfaceIdiom = InterfaceIdiom::Desktop;

    auto contents = loadFile("/etc/machine-id");

    auto comps = parseStdVectorTrim<String>(contents, '\n');
    uuid = comps[0];
}

//String Env::deviceUserName() { return "Unknown"; }

String Env::deviceUserName() { return getlogin(); }

String Env::getStoreAppPageLink() noexcept { return "https://appstore.com/linux_stub"; }

} // namespace mangl
