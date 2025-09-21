#include "../core/mangl_core.h"
#include "../core/mangl_environment.h"
#include "../core/mangl_storage.h"

#include <apeal/file.hpp>
#include <apeal/parse.hpp>

namespace mangl {

Env::Env()
{
    using namespace apl;

    FileSystem fs;

    // collect all the directories
    auto paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    docDir = toString(paths[0]);

    homeDir = toString(NSHomeDirectory());
    tempDir = toString(NSTemporaryDirectory());

    paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    if (paths && paths.count > 0)
        cacheDir = toString(paths[0]);
    else
        cacheDir = docDir;


    auto bundle = NSBundle.mainBundle;

    bundleDir = toString(bundle.resourcePath);

    appName = toString([bundle.infoDictionary objectForKey:(NSString *)kCFBundleNameKey]);
    appVersion = toString([bundle.infoDictionary objectForKey:(NSString *)kCFBundleVersionKey]);
#ifdef MANGL_APP_TITLE
    appTitle = MANGL_APP_TITLE;
#else
    appTitle = appName;
#endif

    // create the documents dir (sometimes it doesn't exist for an app)
    fs.mkdirIfNotExists(docDir);

    settingsFile = makeFilePath(docDir, "mangl_settings.sqlite");
    statsFile = makeFilePath(docDir, "mangl_stats.sqlite");
    
    SettingsStorage::instance()->setLocation(settingsFile);
    StatsStorage::instance()->setLocation(statsFile);


    auto currentDevice = UIDevice.currentDevice;

    [currentDevice beginGeneratingDeviceOrientationNotifications];

    auto versionString = currentDevice.systemVersion;
    auto v = parseStdVectorTrim<StrView>(versionString.UTF8String, '.');

    osVersionHi = v.size() >= 1 ? parseInt(v[0]) : 0;
    osVersionLo = v.size() >= 2 ? parseInt(v[1]) : 0;
    osVersionPatch = v.size() >= 3 ? parseInt(v[2]) : 0;

    MDebugPrint("OS Version: %d.%d.%d", osVersionHi, osVersionLo, osVersionPatch);

    osVersionCode = osVersionHi * 10000 + osVersionLo * 100 + osVersionPatch;


    auto o = currentDevice.orientation;

    switch (o) {
    case UIDeviceOrientationUnknown:            orientation = ScreenOrientation::Unknown; break;
    case UIDeviceOrientationPortrait:           orientation = ScreenOrientation::Portrait; break;
    case UIDeviceOrientationPortraitUpsideDown: orientation = ScreenOrientation::PortraitUpside; break;
    case UIDeviceOrientationLandscapeLeft:      orientation = ScreenOrientation::Landscape; break;
    case UIDeviceOrientationLandscapeRight:     orientation = ScreenOrientation::LandscapeRight; break;
    case UIDeviceOrientationFaceUp:             orientation = ScreenOrientation::FaceUp; break;
    case UIDeviceOrientationFaceDown:           orientation = ScreenOrientation::FaceDown; break;
    }


    if (UIUserInterfaceIdiomPad == currentDevice.userInterfaceIdiom)
        interfaceIdiom = InterfaceIdiom::Pad;


    //
    // Screen resolution
    // TODO: move to screen initialization
    //
    auto mainScreen = UIScreen.mainScreen;
    
    auto nativeBounds = mainScreen.nativeBounds.size;
    auto nativeScale = mainScreen.nativeScale;
    
    auto logicalScreen = mainScreen.bounds.size;
    auto logicalScale = mainScreen.scale;

    physScreenSize = {nativeBounds.width, nativeBounds.height};
    //UIScreen.mainScreen.displa

    //screenSize = {physScreen.width * physScale, physScreen.height * physScale};
    //screenSize = physScreenSize;
    //applicationFrame = {.w = logicalScreen.width, .h = logicalScreen.height};
    applicationRect = {.w = physScreenSize.w, .h = physScreenSize.h};
    layoutSize = {logicalScreen.width, logicalScreen.height};

//    pixelDensity = logicalScale;
    pixelDensity = nativeScale;
    logicalScreenSize = {.w = logicalScreen.width, .h = logicalScreen.height};

    MDebugPrint("Screen: phys: %g, logical: %g", physScreenSize, logicalScreenSize);
}

String Env::deviceUserName()
{
    return toString(UIDevice.currentDevice.name);
}

String Env::getStoreAppPageLink() noexcept
{
    String appId;

#ifdef MANGL_APP_ID
    appId = MANGL_APP_ID;
#endif
    
    MDebugTrapAssert(appId);
    
    auto urlTemplate = "itms-apps://itunes.apple.com/app/id%s";
    
    return apl::fmt(urlTemplate, appId);
}


}   // namespace mangl
