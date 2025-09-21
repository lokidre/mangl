/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once


#include "mangl_types.h"
#include <limits>

#include <apeal/singleton.hpp>

namespace mangl {


class Env: public apl::Singleton<Env, false> {

    using Self = Env;

public:
    static String docDir;    // application documents
    static String homeDir;   // application home dir
    static String bundleDir; // Bundle dir
    static String moduleDir; // Location of the executable
    static String libDir, cacheDir, tempDir;

    static String appTitle, appName, appVersion;

    static String uuid;

    // Device name entered by the user
    static String deviceUserName();

    static String musicExt, soundExt, textureExt;

    static int osVersionHi, osVersionLo, osVersionPatch;
    static int osVersionCode;

    static auto getOsVersionCode() noexcept { return osVersionCode; }

#if MANGL_GAMECENTER
    static bool gamecenterEnabled, gamecenterAuthenticated;
#else
    constexpr static bool gamecenterEnabled{}, gamecenterAuthenticated{};
#endif



    static ScreenOrientation orientation; // device orientation at startup

    static DeviceType device;

    constexpr static Size DefaultAppSize{320, 568};
    constexpr static Rect InvalidRect{M::InvalidCord, M::InvalidCord, M::InvalidCord, M::InvalidCord};

    //
    // Various screen sizes and resolutions
    //
    constexpr static Size AppStorePhoneScreenshot69{1320, 2868};
    constexpr static Size AppStorePhoneScreenshot65{1284, 2778};

    constexpr static Size AppStorePadScreenshot13{2064, 2752};

    constexpr static Size PlayStorePhoneScreenshot{1152, 2048};



    // Dimensions
    static Size physScreenSize;   // Physical screen size in pixels
    static Rect applicationRect;  // Application rectangle on the screen in device units (pixels)
    static Real statusBarHeight;
    static Padding screenPadding; // iPhone X or Android P+ safe areas
    // static Size screenSize;  // Logical screen size in device units (pixels)
    Rect visibleFrame{};

    static Size logicalScreenSize; // Screen size in logical units. Usually physSize / pixelDensity

    static Size layoutSize;        // Application layout in logical units set by user
    static Padding layoutPadding;  // Padding adjusted to selected layout units


    static Size screenshotSize;
    static void setScreenshotSize(SizeA ss) {
        screenshotSize = ss;
    }


    // should not be called from the phone
    // on desktop it sets the application contentsFrame on the screen
    // of position coordinates are negative the position of the contentsFrame will be managed
    // automatically
    static void setApplicationRect(RectA rect, PaddingA padding = {}) noexcept {
        applicationRect = rect;
        screenPadding = padding;
    }

    static void setApplicationSize(Size size, PaddingA padding = {}) noexcept {
        applicationRect.resize(size);
        screenPadding = padding;
    }


    static Rect fullLayout() noexcept { return {0, 0, layoutSize.w, layoutSize.h}; }
    static Rect getFullArea() noexcept { return fullLayout(); }

    static Rect paddedLayout() noexcept {
        return {
            layoutPadding.left(), layoutPadding.bottom(),
            layoutSize.w - layoutPadding.horz2(),
            layoutSize.h - layoutPadding.vert2()};
    }

    static auto getSafePadding() noexcept { return layoutPadding; }
    static auto safeLayout() noexcept { return paddedLayout(); }
    static auto getSafeArea() noexcept { return paddedLayout(); }

    static void setLayout(Size layout) noexcept {
        layoutSize = layout;
        screenScale = applicationRect.w / layoutSize.w;
        layoutPadding = screenPadding / screenScale;
        updateConversionData();
    }

    static void setLayoutWidth(Cord width) noexcept {
        Size size{width, width * applicationRect.h / applicationRect.w};
        // MDebugPrint("setLayoutWidth: %f", width);
        setLayout(size);
    }

    static bool inRange(Cord u, Cord v) noexcept { return std::fabs(u - v) < static_cast<Cord>(0.01); }
    // pad: 768x1024 (1.333333)
    // legacy: 320x480 (1.5)
    // normal: 320x568 (1.775) (iPhone 5)
    // padded tall: 320x632 (1.975) (iPhone X padded
    // tall: 375x812 (2.1653) (iPhone X)
    static auto screenRatio() noexcept { return applicationRect.h / applicationRect.w; }
    static bool isPaddedLayoutLegacy() noexcept { return inRange(screenRatio(), 1.5); }
    static bool isPaddedLayoutPad() noexcept { return inRange(screenRatio(), 1.333333); }


    static InterfaceIdiom interfaceIdiom;
    static InterfaceIdiom setInterfaceIdiom(InterfaceIdiom ii) {
        auto curr = interfaceIdiom;
        interfaceIdiom = ii;
        return curr;
    }
    static bool isDesktop() noexcept { return InterfaceIdiom::Desktop == interfaceIdiom; }
    static bool isPad() noexcept { return InterfaceIdiom::Tablet == interfaceIdiom; }
    static bool isTablet() noexcept { return InterfaceIdiom::Tablet == interfaceIdiom; }
    static bool isPhone() noexcept { return InterfaceIdiom::Phone == interfaceIdiom; }


#if APEAL_OS_ANDROID || APEAL_OS_IPHONE
    constexpr static bool isDeviceDesktop() noexcept { return false; }
    constexpr static bool isDevicePhone() noexcept { return true; }
    constexpr static bool isDevicePad() noexcept { return true; }
    constexpr static bool isDeviceTablet() noexcept { return true; }
#else
    // #if APEAL_OS_WINDOWS || APEAL_OS_MAC_DESKTOP || APEAL_OS_LINUX
    constexpr static bool isDeviceDesktop() noexcept { return true; }
    constexpr static bool isDevicePhone() noexcept { return false; }
    constexpr static bool isDevicePad() noexcept { return false; }
    constexpr static bool isDeviceTablet() noexcept { return false; }
// #else
#endif

    static Cord pixelDensity; // 2+ for retina display
    static Cord screenScale;  // Managed internally. Usually: screenScale * upscale * layoutSize ==
                              // screenSize


    // Conversion between screen coordinates and layout units
    static Cord conversionK_;
    static Cord conversionFlipHeight_;

    static void updateConversionData() noexcept {
        // conversionK_ = layoutSize.w / applicationRect.w;
        conversionK_ = layoutSize.w / logicalScreenSize.w;
        // conversionFlipHeight_ = screenSize.h / pixelDensity;
        conversionFlipHeight_ = logicalScreenSize.h;
    }

    //static void updateLogicalConversionData() noexcept {
    //    conversionK_ = layoutSize.w / applicationRect.w;
    //    // conversionFlipHeight_ = screenSize.h / pixelDensity;
    //    conversionFlipHeight_ = applicationRect.h;
    //}


    static Cord toScreenCoord(Cord c) noexcept { return c / conversionK_; }

private:  // Shorthand
    static Cord tosc(Cord c) noexcept { return toScreenCoord(c); }
public:

    static Cord toScreenFlipPointY(Cord c) noexcept {
        if (c == 0)
            return conversionFlipHeight_ - 0.0000001;
        return conversionFlipHeight_ - tosc(c);
    }

    static Cord toScreenFlipRectY(Cord c, Cord h) noexcept {
        return conversionFlipHeight_ - tosc(h + c);
    }

    static Cord fromScreenCoord(Cord c) noexcept { return c * conversionK_; }

private:  // Shorthand
    static Cord frsc(Cord c) noexcept { return fromScreenCoord(c); }
public:

    static Cord fromScreenFlipPointY(Cord c) noexcept {
        if (c == 0)
            return conversionFlipHeight_ - 0.0000001;
        return frsc(conversionFlipHeight_ - c);
    }
    static Cord fromScreenFlipRectY(Cord c, Cord h) noexcept {
        return frsc(conversionFlipHeight_ - h - c);
    }


    static Cord toScreenPointY(Cord c) noexcept {
#if defined(APEAL_OS_MAC_DESKTOP)
        return tosc(c);
#else
        return toScreenFlipPointY(c);
#endif
    }

    static Cord fromScreenPointY(Cord c) noexcept {
#if defined(APEAL_OS_MAC_DESKTOP)
        return fromScreenCoord(c);
#else
        return fromScreenFlipPointY(c);
#endif
    }

    static Cord toScreenRectY(Cord c, Cord h) noexcept {
#if defined(APEAL_OS_MAC_DESKTOP)
        return toScreenCoord(c);
#else
        return toScreenFlipRectY(c, h);
#endif
    }

    static Cord fromScreenRectY(Cord c, Cord h) noexcept {
#if defined(APEAL_OS_MAC_DESKTOP)
        return fromScreenCoord(c);
#else
        return fromScreenFlipRectY(c, h);
#endif
    }


    static Point toScreen(Point p) noexcept { return {tosc(p.x), toScreenPointY(p.y)}; }
    static Point fromScreen(Point p) noexcept { return {frsc(p.x), fromScreenPointY(p.y)}; }

    static Size toScreen(Size s) noexcept { return {tosc(s.w), tosc(s.h)}; }
    static Size fromScreen(Size s) noexcept { return {frsc(s.w), frsc(s.h)}; }

    static Rect toScreen(RectA r) noexcept {
        return { tosc(r.x), toScreenRectY(r.y, r.h), tosc(r.w), tosc(r.h) };
    }
    static Rect fromScreen(RectA r) noexcept {
        return { frsc(r.x), toScreenRectY(r.y, r.h), frsc(r.w), frsc(r.h) };
    }


    //
    // FullScreen support
    //
    static bool fullScreenEnabled_;
    static bool isFullScreenEnabled() { return fullScreenEnabled_; }


    //
    // App store
    //
    static String getStoreAppPageLink() noexcept;



    static String settingsFile, statsFile;


    //
    // Desktop stuff
    //

    static String windowTitle;
    static String windowIcon;

    static void setWindowTitle(StrView text){ windowTitle = text; }
    static void setWindowIcon(StrView text){ windowIcon = text; }
    static void setWindowIcon(int resourceId){ windowIcon = apl::fmt("#%d", resourceId); }



    //
    // Initializations
    //
    Env();
    static void init();
    static void processScreenLayout();
    static void updateScreenChanges();
};

} // namespace mangl
