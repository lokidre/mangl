/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_common.h"

#include "mangl_and_jni_utils.h"
#include "mangl_and_main_engine.h"

#include "../core/mangl_environment.h"
#include "../core/mangl_setup.h"
#include "../core/mangl_storage.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <apeal/file.hpp>

#include <game-activity/GameActivity.h>


namespace mangl {

Env::Env() = default;

void Env::init()
{
    using apl::makeFilePath;

    auto jenv = Jni::getJniEnv();
    auto javaMainActivity = Jni::getJavaActivity();
    auto jclass = Jni::getJavaActivityClass();

    jmethodID jmethod{};
//    jfloatArray jfloatsResult{};
//    float* floatsArray{};
//    jboolean isCopy{};

    auto jFile = Jni::findClass("java/io/File");

    auto getAbsolutePath = Jni::getMethodId(jFile, "getAbsolutePath", "()Ljava/lang/String;");
    auto getCacheDir = Jni::getActivityMethodId("getCacheDir", "()Ljava/io/File;");

    auto cacheFile = jenv->CallObjectMethod(javaMainActivity, getCacheDir);

    cacheDir = Jni::j2string((jstring)jenv->CallObjectMethod(cacheFile, getAbsolutePath));
    tempDir = cacheDir;  // in Android the temp dir is alco cache dir

    auto getFilesDir = jenv->GetMethodID(jclass, "getFilesDir", "()Ljava/io/File;");
    auto docFile = jenv->CallObjectMethod(javaMainActivity, getFilesDir);
    docDir = Jni::j2string(jenv->CallObjectMethod(docFile, getAbsolutePath));

    libDir = makeFilePath(docDir, ".mangl");

    apl::FileSystem::mkdirIfNotExistsParents(libDir);

    MDebugPrint("LibDir: %s", libDir);


    bundleDir = ":assets";

    settingsFile = makeFilePath(libDir, "settings.sqlite");
    statsFile = makeFilePath(libDir, "stats.sqlite");

    SettingsStorage::instance()->setLocation(settingsFile);
    StatsStorage::instance()->setLocation(statsFile);


    //MDebugPrint("Settings file: %s", settingsFile);




    auto getPackageManager = jenv->GetMethodID(jclass, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    auto packageManager = jenv->CallObjectMethod(javaMainActivity, getPackageManager);
    auto packageManagerClass = jenv->GetObjectClass(packageManager);

    auto getPackageName = jenv->GetMethodID(jclass, "getPackageName", "()Ljava/lang/String;");
    auto packageName = jenv->CallObjectMethod(javaMainActivity, getPackageName);

    auto getPackageInfo = jenv->GetMethodID(packageManagerClass, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    auto packageInfo = jenv->CallObjectMethod(packageManager, getPackageInfo, packageName, 0);
    auto packageInfoClass = jenv->GetObjectClass(packageInfo);

    auto versionNameField = jenv->GetFieldID(packageInfoClass, "versionName", "Ljava/lang/String;");
    appVersion = Jni::j2string(jenv->GetObjectField(packageInfo, versionNameField));

    MDebugPrint("Package Version: %s", appVersion);


    // App Bundle Id
    //appName = "app";
    jmethod = jenv->GetMethodID(jclass, "getPackageName", "()Ljava/lang/String;");
    appName = Jni::j2string(jenv->CallObjectMethod(javaMainActivity, jmethod));

#ifdef MANGL_APP_TITLE
    appTitle = MANGL_APP_TITLE;
#else
    appTitle = appName;
#endif

    MDebugPrint("App Name: %s", appName);


    // create the documents dir (sometimes it doesn't exist for an app)
    //if (!fs.exist(doc_dir))
    //	fs.mkdir(doc_dir);

//        auto getAssetManager = jenv->GetMethodID(jclass, "getAssets", "()Landroid/content/res/AssetManager;");
//        auto jAssetManager = jenv->CallObjectMethod(javaMainActivity, getAssetManager);
//        assetManager_ = AAssetManager_fromJava(jenv, jAssetManager);


    /*
    // List all assets
    auto assetDir = AAssetManager_openDir ( assetManager_, "" ) ;
    for(;;) {
        auto fn = AAssetDir_getNextFileName(assetDir) ;
        if ( fn == 0 )
            break ;
        debug_printf ( "%s", fn ) ;
    }
    AAssetDir_close(assetDir) ;
    */

    auto isTablet = Jni::isTablet();
    interfaceIdiom = isTablet ? InterfaceIdiom::Tablet : InterfaceIdiom::Phone;

    orientation = ScreenOrientation::Portrait;
    pixelDensity = Jni::getPixelDensity();
    MDebugPrint("Pixel density: %g", pixelDensity);

    //
    // Get insets and cutouts
    //
    ARect insets;
    GameActivity_getWindowInsets(Jni::getActivity(), GAMECOMMON_INSETS_TYPE_DISPLAY_CUTOUT, &insets);
    MDebugPrint("Activity insets: %g, %g, %g, %g", insets.top, insets.right, insets.bottom, insets.left);

    screenPadding = Jni::getDisplayInsets();
    MDebugPrint("Display insets: %g, %g, %g, %g", screenPadding.top(), screenPadding.right(), screenPadding.bottom(), screenPadding.left());

    physScreenSize = Jni::getDisplaySize();

    applicationRect = {.w = physScreenSize.w, .h = physScreenSize.h};
    logicalScreenSize = physScreenSize / pixelDensity;
    MDebugPrint("Screen phys: %.0z, logical: %.0z", physScreenSize, logicalScreenSize);

    // TODO: generate UUID and store it in the local database
    uuid = "1234567890";
}

String Env::getStoreAppPageLink() noexcept {
    return {};
}

} // namespace mangl
