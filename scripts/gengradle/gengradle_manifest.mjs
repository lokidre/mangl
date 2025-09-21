// @ts-check

import {generateHeader} from './generate_header.mjs';
import {params_, project_, setup_} from './gengradle_generate.mjs';
import {createAndWriteFile} from './lib_fs.mjs';
import {content_, nl, outclear, outn} from './lib_io.mjs';

export function createAndroidManifestxml() {
    outclear(generateHeader('xml'));

    let permissions = new Map();
    let metadata = new Map();
    let properties = new Map();
    let applicationProps = new Map();

    applicationProps.set('name', '.MainApplication');
    applicationProps.set('allowBackup', 'true');
    applicationProps.set('icon', '@mipmap/ic_launcher');
    applicationProps.set('label', '@string/appDisplayName');
    applicationProps.set('roundIcon', '@mipmap/ic_launcher_round');
    applicationProps.set('fitsSystemWindows', 'true');
    applicationProps.set('theme', '@style/Theme.App.Starting');

    metadata.set('android.app.lib_name', {value: 'main'});

    if (project_.multiplayer || project_.ads) {
        permissions.set('android.permission.INTERNET', {});
        applicationProps.set('networkSecurityConfig', '@xml/network_security_config');
    }

    if (project_.firebase) {
        metadata.set('google_analytics_default_allow_analytics_storage', {value: 'true'});
        metadata.set('google_analytics_default_allow_ad_storage', {value: 'true'});
        metadata.set('google_analytics_default_allow_ad_user_data', {value: 'true'});
        metadata.set('google_analytics_default_allow_ad_personalization_signals', {value: 'true'});
    }

    if (project_.ads) {
        permissions.set('com.google.android.gms.permission.AD_ID', {});

        // properties.set('android.adservices.AD_SERVICES_CONFIG', {
        //     resource: '@xml/gma_ad_services_config',
        //     replace: 'android:resource',
        // });

        //        outn(`        <property android:name="android.adservices.AD_SERVICES_CONFIG" android:resource="@xml/gma_ad_services_config" tools:replace="android:resource" />`)

        metadata.set('com.google.android.gms.ads.APPLICATION_ID', {value: '@string/admobAppId'});

        metadata.set('com.google.android.gms.ads.flag.OPTIMIZE_INITIALIZATION', {value: 'true'});
        metadata.set('com.google.android.gms.ads.flag.OPTIMIZE_AD_LOADING', {value: 'true'});

        //outn(`        <meta-data android:name="com.google.android.gms.ads.flag.OPTIMIZE_INITIALIZATION" android:value="true"/>`)
        //outn(`        <meta-data android:name="com.google.android.gms.ads.flag.OPTIMIZE_AD_LOADING" android:value="true"/>`)

        permissions.set('android.permission.ACCESS_FINE_LOCATION', {});
        permissions.set('android.permission.ACCESS_COARSE_LOCATION', {});
        permissions.set('android.permission.ACCESS_NETWORK_STATE', {});
        permissions.set('android.permission.ACCESS_WIFI_STATE', {});
        permissions.set('android.permission.CHANGE_WIFI_STATE', {});
    }

    if (project_.bluetooth) {
        //
        // Request legacy Bluetooth permissions on older devices
        //
        // permissions.set("android.permission.BLUETOOTH", {maxSdkVersion: 30});
        // permissions.set("android.permission.BLUETOOTH_ADMIN", {maxSdkVersion: 30});
        // permissions.set("android.permission.ACCESS_FINE_LOCATION", {maxSdkVersion: 30});
        // permissions.set("android.permission.ACCESS_COARSE_LOCATION", {maxSdkVersion: 30});
        permissions.set('android.permission.BLUETOOTH', {});
        permissions.set('android.permission.BLUETOOTH_ADMIN', {});
        permissions.set('android.permission.ACCESS_FINE_LOCATION', {});
        permissions.set('android.permission.ACCESS_COARSE_LOCATION', {});
        permissions.set('android.permission.BLUETOOTH_SCAN', {});
        // permissions.set("android.permission.BLUETOOTH_SCAN", {maxSdkVersion: 30});
        // permissions.set("android.permission.BLUETOOTH_SCAN", {minSdkVersion: 31, usesPermissionFlags: "neverForLocation"});
        permissions.set('android.permission.BLUETOOTH_ADVERTISE', {});
        permissions.set('android.permission.BLUETOOTH_CONNECT', {});
        permissions.set('android.permission.REQUEST_IGNORE_BATTERY_OPTIMIZATIONS', {});
    }


    //package="${project_.namespace}"

    outn(
        `<manifest xmlns:android="http://schemas.android.com/apk/res/android" xmlns:tools="http://schemas.android.com/tools">`
    );

    // outn(`    <uses-sdk android:minSdkVersion="${setup_.minSdk}" android:targetSdkVersion="${setup_.targetSdk}" />`);

    for (let [key, value] of permissions) {
        let params = `android:name="${key}"`;
        if (value.maxSdkVersion) params += ` android:maxSdkVersion="${value.maxSdkVersion}"`;
        if (value.minSdkVersion) params += ` android:maxSdkVersion="${value.minSdkVersion}"`;
        if (value.usesPermissionFlags) params += ` android:usesPermissionFlags="${value.usesPermissionFlags}"`;

        outn(`    <uses-permission ${params}/>`);
    }

    outn(`    <application`);
    for (let [key, value] of applicationProps) {
        outn(`        android:${key}="${value}"`);
    }
    outn(`    >`);

    for (let [key, value] of properties) {
        outn(
            `        <property android:name="${key}" android:resource="${value.resource}" tools:replace="${value.replace}"/>`
        );
    }

    for (let [key, value] of metadata) {
        outn(`        <meta-data android:name="${key}" android:value="${value.value}"/>`);
    }

    // outn(`        <meta-data android:name="android.app.lib_name" android:value="main" />`)
    // nl();

    // if (project_.ads) {
    // outn(`        <property android:name="android.adservices.AD_SERVICES_CONFIG" android:resource="@xml/gma_ad_services_config" tools:replace="android:resource" />`)
    // nl();
    // outn(`        <meta-data android:name="com.google.android.gms.ads.APPLICATION_ID" android:value="@string/admob_app_id"/>`)
    // outn(`        <meta-data android:name="com.google.android.gms.ads.flag.OPTIMIZE_INITIALIZATION" android:value="true"/>`)
    // outn(`        <meta-data android:name="com.google.android.gms.ads.flag.OPTIMIZE_AD_LOADING" android:value="true"/>`)
    // }

    outn(`
        <activity
            android:name=".MainActivity"
            android:exported="true"
            android:screenOrientation="portrait"
            android:configChanges="orientation|keyboardHidden"
            android:windowSoftInputMode="adjustResize"
        >

            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
                <action android:name="\${applicationId}.TRANSMOGRIFY" />
            </intent-filter>

        </activity>
`);

    outn(`    </application>`);

    outn(`
    <queries>
        <intent>
            <action android:name="android.intent.action.SEND" />
            <data android:mimeType="text/plain" />
        </intent>
    </queries>
`);

    outn(`</manifest>`);

    createAndWriteFile(`${params_.output}/app/src/main/AndroidManifest.xml`, content_);
}
