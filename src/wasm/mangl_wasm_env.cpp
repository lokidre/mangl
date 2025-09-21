/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../core/mangl_setup.h"

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

    apl::FileSystem fs;


    // TODO: implement in apeal::fs class (fs.homedir())
    // struct passwd *pw = getpwuid(getuid());
    // const char *homedir = pw->pw_dir;
    // homeDir = homedir;

    homeDir = "/";

    String name;

#ifdef MANGL_APP_TAG
    name = MANGL_APP_TAG;
#else

    // const int bufferSize = PATH_MAX + 1;
    // char buf[bufferSize + 1];
    // int res = readlink("/proc/self/exe", buf, bufferSize);
    // buf[res] = 0;
    // massert(res > 0, "Cannot read /proc/self/exe");
    // name = extractFileName(buf);

    name = "test";

#endif


    appName = name;

#ifdef MANGL_APP_VERSION
    appVersion = MANGL_APP_VERSION;
#else
    appVersion = "0.0.1";
#endif


    tempDir = fs.tmpdir();

    //String varDir = "/var";
    String varDir = makeFilePath(homeDir, "var");

    cacheDir = makeFilePath(varDir, "cache", name);
    libDir = makeFilePath(varDir, "lib", name);
    docDir = makeFilePath(varDir, "doc", name);
    bundleDir = makeFilePath(varDir, "bundle", name);

    settingsFile = makeFilePath(libDir, "stats.sqlite");
    statsFile = makeFilePath(libDir, "stats.sqlite");


    SettingsStorage::instance()->setLocation(settingsFile);
    StatsStorage::instance()->setLocation(statsFile);

    // TODO: move / detect
    physScreenSize.w = 414;
    physScreenSize.h = 896;

    applicationRect = {0, 0,physScreenSize.w, physScreenSize.h};

    logicalScreenSize = physScreenSize;

    interfaceIdiom = InterfaceIdiom::Desktop;

    uuid = "12345";


    // auto contents = loadFile("/etc/machine-id");

    // auto comps = parseStdVectorTrim<String>(contents, '\n');
    // uuid = comps[0];
}

String Env::deviceUserName() { return "Unknown"; }

String Env::getStoreAppPageLink() noexcept { return "https://appstore.com/linux_stub"; }

} // namespace mangl
