// @ts-check

import { generateHeader } from "./generate_header.mjs";
import { params_, project_, vars_ } from "./gengradle_generate.mjs";
import { processValueBody } from "./gengradle_vars.mjs";
import { createAndWriteFile, linkFile } from "./lib_fs.mjs";
import { content_, nl, outclear, outn } from "./lib_io.mjs";


export function createAndroidNetworkSecurityConfig() {

    if (!project_.ads && !project_.multiplayer)
        return;

    outclear(generateHeader("xml"));

    outn(`
<network-security-config>
    <base-config cleartextTrafficPermitted="true">
        <trust-anchors>
            <certificates src="system" />
        </trust-anchors>
    </base-config>
    <domain-config cleartextTrafficPermitted="true">
        <domain includeSubdomains="true">127.0.0.1</domain>
    </domain-config>
</network-security-config>
`
    );

    createAndWriteFile(`${params_.output}/app/src/main/res/xml/network_security_config.xml`, content_);
}




export function createAndroidResValues() {
    let resDir = `${params_.output}/app/src/main/res`;

    // Colors
    outclear(generateHeader("xml"));
    outn(`<resources>`)
    outn(`    <color name="primary">#6200EE</color>`)
    outn(`    <color name="primaryDark">#3700B3</color>`)
    outn(`    <color name="accent">#03DAC5</color>`)
    outn(`    <color name="splashScreenBackground">#030f17</color>`)
    outn(`</resources>`)
    createAndWriteFile(`${resDir}/values/colors.xml`, content_)

    // Splash screen
    let splashIconSize = 192;
    outclear(generateHeader("xml"));
    outn(`<resources>`)
    outn(`    <dimen name="splashScreenIconSize">${splashIconSize}dp</dimen>`)
    outn(`</resources>`)
    createAndWriteFile(`${resDir}/values/splash.xml`, content_)

    //strings
    outclear(generateHeader("xml"));
    outn(`<resources>`)
    outn(`    <string name="appName">${project_.name}</string>`)
    outn(`    <string name="appDisplayName">${project_.displayName}</string>`)
    if (project_.admobAppId) {
        outn(`    <string name="admobAppId">${project_.admobAppId}</string>`);
    }

    if (project_.crashlytics) {
        outn(`    <string name='com.crashlytics.android.build_id'>none</string>`);
    }
    
    outn(`</resources>`)
    createAndWriteFile(`${resDir}/values/strings.xml`, content_)

    // styles
    generateStyles(false);
    createAndWriteFile(`${resDir}/values/styles.xml`, content_)


    generateStyles(true);
    createAndWriteFile(`${resDir}/values-v27/styles.xml`, content_)
}

/** @param {boolean} v27 */
export function generateStyles(v27) {
    outclear(generateHeader("xml"));

    outn(`<resources>`)
    outn(`    <style name="Theme.App" parent="Theme.AppCompat.Light.NoActionBar">`)
    outn(`        <item name="android:windowFullscreen">true</item>`)
    outn(`        <item name="android:windowContentOverlay">@null</item>`)

    //outn(`        <!-- Customize your theme here. -->`)
    outn(`        <item name="colorPrimary">@color/primary</item>`)
    outn(`        <item name="colorPrimaryDark">@color/primaryDark</item>`)
    outn(`        <item name="colorAccent">@color/accent</item>`)
    nl()

    if (v27) {
        outn(`        <item name="android:windowLayoutInDisplayCutoutMode">shortEdges</item>`)
    }

    outn(`        <item name="android:windowTranslucentStatus">true</item>`)
    outn(`        <item name="android:windowTranslucentNavigation">true</item>`)
    nl()
    outn(`        <item name="android:navigationBarColor">@android:color/transparent</item>`);
    // nl()
    // outn(`        <!-- Optional: set to transparent if your app is drawing behind the status bar. -->`)
    outn(`        <item name="android:statusBarColor">@android:color/transparent</item>`)
    // nl()
    // outn(`        <!-- Optional: set the status bar light and content dark. -->`)
    outn(`        <item name="android:windowLightStatusBar">true</item>`);
    outn(`    </style>`)

    outn(`    <style name="Theme.App.Starting" parent="Theme.SplashScreen">`)
    outn(`        <item name="windowSplashScreenBackground">@color/splashScreenBackground</item>`)
    outn(`        <item name="windowSplashScreenAnimatedIcon">@drawable/splash_screen</item>`)
    outn(`        <item name="postSplashScreenTheme">@style/Theme.App</item>`)
    outn(`    </style>`)
    outn(`</resources>`);
}

export function createAndroidLayout() {
    outclear(generateHeader("xml"));

    outn(`
<androidx.constraintlayout.widget.ConstraintLayout 
    xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:app="http://schemas.android.com/apk/res-auto"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    tools:context=".MainActivity">

</androidx.constraintlayout.widget.ConstraintLayout>
`
    );

    createAndWriteFile(`${params_.output}/app/src/main/res/layout/activity_main.xml`, content_)
}



export function createLaunchAssets() {
    let mainDir = `${params_.output}/app/src/main`;
    let resDir = `${mainDir}/res`;
    let iconAssetsDir = processValueBody(project_.appIconDir, vars_);


    // Play Store icon
    linkFile(iconAssetsDir, mainDir, "ic_launcher-playstore.png");

    //
    // Drawable
    //
    // Splash Screen
    outclear(generateHeader("xml"));
    outn(`
<layer-list xmlns:android="http://schemas.android.com/apk/res/android">
    <item
        android:width="@dimen/splashScreenIconSize"
        android:height="@dimen/splashScreenIconSize"
        android:drawable="@drawable/ic_launcher_foreground"
        android:gravity="center" />
</layer-list>
`);    

    createAndWriteFile(`${resDir}/drawable/splash_screen.xml`, content_);

    linkFile(iconAssetsDir, `${resDir}/drawable`, "ic_launcher_background.png");
    linkFile(iconAssetsDir, `${resDir}/drawable`, "ic_launcher_foreground.png");



    //
    // res/layout
    //

    // res/mipmap-anydpi-v26
    outclear(generateHeader("xml"));
    outn(`
<adaptive-icon xmlns:android="http://schemas.android.com/apk/res/android">
    <background android:drawable="@mipmap/ic_launcher_background"/>
    <foreground android:drawable="@mipmap/ic_launcher_foreground"/>
</adaptive-icon>
`);

    createAndWriteFile(`${resDir}/mipmap-anydpi-v26/ic_launcher.xml`, content_);
    createAndWriteFile(`${resDir}/mipmap-anydpi-v26/ic_launcher_round.xml`, content_);


    let mipmaps = ["mipmap-hdpi", "mipmap-mdpi", "mipmap-xhdpi", "mipmap-xxhdpi", "mipmap-xxxhdpi"];
    let mipmapFiles = ["ic_launcher.png", "ic_launcher_background.png", "ic_launcher_foreground.png", "ic_launcher_round.png"];
    for (let mipmap of mipmaps) {
        for (let mmfile of mipmapFiles) {
            linkFile(`${iconAssetsDir}/${mipmap}`, `${resDir}/${mipmap}`, mmfile);
        }
    }
}
