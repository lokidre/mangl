#include "../core/mangl_environment.h"
#include "../core/mangl_common.h"
#include "../core/mangl_storage.h"

#include "../apple/mangl_imacos_util.h"

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <CoreServices/CoreServices.h>


#include <apeal/file.hpp>
#include <apeal/parse.hpp>

namespace mangl {

Env::Env()
{
    using namespace apl;

    FileSystem fs;

    // collect all the directories
    auto docPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    docDir = toString(docPaths[0]);

    auto libPaths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    libDir = toString(libPaths[0]);

    auto cachesPaths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    cacheDir = toString(cachesPaths[0]);


    homeDir = toString(NSHomeDirectory());
    tempDir = toString(NSTemporaryDirectory());

    auto bundle = NSBundle.mainBundle;

    bundleDir  = toString(bundle.resourcePath);

    appName = toString(bundle.infoDictionary[(NSString *)kCFBundleNameKey]);

#ifdef MANGL_APP_TAG
    if (appName.empty())
        appName = MANGL_APP_TAG;
#endif
	
    appVersion = toString(bundle.infoDictionary[@"CFBundleShortVersionString"]);
    if (appVersion.empty())
        appVersion = "0.99.99";

#ifdef MANGL_APP_TITLE
    appTitle = MANGL_APP_TITLE;
#else
    appTitle = appName;
#endif

    // Available since Yosemite
    auto osv = NSProcessInfo.processInfo.operatingSystemVersion;
    osVersionHi = (int)osv.majorVersion;
    osVersionLo = (int)osv.minorVersion;
    osVersionPatch = (int)osv.patchVersion;

    MDebugPrint("OS Version: %d.%d.%d", osVersionHi, osVersionLo, osVersionPatch);

    osVersionCode = osVersionHi * 1000000 + osVersionLo * 10000 + osVersionPatch;
	

//	// create the documents dir (sometimes it doesn't exist for an app)
//	if (!apl::fileExists(docDir))
//		fs.mkdir(docDir) ;
//
//	if ( !fs.exist(libDir) )
//		fs.mkdir(libDir) ;
//
//	auto appLibraryDir = libDir + "/" + appName ;
//
//	if ( !fs.exist(appLibraryDir) )
//		fs.mkdir(appLibraryDir) ;
//
//	if ( !fs.exist(cacheDir) )
//		fs.mkdir(cacheDir) ;
//
//	auto appCachesDir = cacheDir + "/" + appName ;
//	if ( !fs.exist(appCachesDir) )
//		fs.mkdir(appCachesDir) ;
//
//
//	fs.make_file_path ( appLibraryDir, MANGL_APP_TAG "-settings.db", settingsFile ) ;
//	fs.make_file_path ( appLibraryDir, MANGL_APP_TAG "-leaderboards.db", leaderboardsFile ) ;
//	fs.make_file_path ( appLibraryDir, MANGL_APP_TAG "-game-state.db", stateFile ) ;
//	fs.make_file_path ( appLibraryDir, MANGL_APP_TAG "-stats.xml", statsFile ) ;
	
    settingsFile = makeFilePath(libDir, "stats.sqlite");
    statsFile = makeFilePath(libDir, "stats.sqlite");

    SettingsStorage::instance()->setLocation(settingsFile);
    StatsStorage::instance()->setLocation(statsFile);



    interfaceIdiom = InterfaceIdiom::Desktop;

    auto mainScreen = NSScreen.mainScreen;
    auto nativeBounds = mainScreen.frame.size;
    auto logicalScreen = mainScreen.frame.size;
    auto logicalScale = mainScreen.backingScaleFactor;

    physScreenSize = {nativeBounds.width, nativeBounds.height};

    //screenSize = physScreenSize;
    applicationRect = {.w = logicalScreen.width, .h = logicalScreen.height};
    layoutSize = {logicalScreen.width, logicalScreen.height};
    logicalScreenSize = {logicalScreen.width, logicalScreen.height};

    pixelDensity = logicalScale;



    //
    // Screen resolution
    //
    //scale = 1.0f ;
    //highres = false ;

    //pixelDensity = screen.backingScaleFactor;
    //	if (pixelDensity > 1)
    //		highres = true;

    //scaleFlag = highres ;

    //screenSize = {screen.frame.size.width, screen.frame.size.height};

    auto vf = mainScreen.visibleFrame;
    visibleFrame = {vf.origin.x, vf.origin.y, vf.size.width, vf.size.height};
	
	
    //layoutSize = screenSize;
}

String Env::deviceUserName() {
    String name;
    if (auto user = getenv("USER"); user)
        name = user;
    if (name.empty())
        name = "UNKNOWN";
    return name;
}

String Env::getStoreAppPageLink() noexcept {
    return "https://appstore.com/windows_stub";
}


}  // namespace mangl


