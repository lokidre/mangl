export type ProjectConfig = {
    apeal_scripts_dir: string,  // Directory where scripts are found to load YAML

    mangl_dir: string,
    mangl_include_dir: string,
    mangl_src_dir: string,
    mangl_3rdparty_dir: string,
    mangl_scripts_dir: string,
    mangl_cmake_dir: string,
}

export type GenerateGradleParams = {
    config: ProjectConfig,  // - Project config object
    project: string,  // Project description file
    cmake: GenerateGradleCmakeParams,
    output: string,  // Output directory
}

export type GenerateGradleCmakeParams = {
    config: string;
    sources: string[] | string | undefined;  // Source file groups
    include_directories: string[] | string | undefined;
    compile_definitions: string[] | string | undefined;
    cmakelists: string[] | undefined;  // Additional source lines
}


export type AndroidProject = {
    name: string,
    displayName: string,
    namespace: string,

    appId: string,
    appTag: string,
    versionCode: number,
    versionName: string,

    appIconDir: string,
    assets?: string[],

    buildType: "debug" | "release",
    debug: boolean;
    release: boolean;

    // kotlin?: boolean;  // Generate Kotlin project


    minSdk?: number,
    targetSdk?: number,
    compileSdk?: number,
    abiFilters?: string[],

    signingStoreFile?: string;
    signingStorePassword?: string;
    signingKeyAlias?: string;
    signingKeyPassword?: string;

    compileDefinitions?: string[];


    multiplayer?: boolean,
    bluetooth?: boolean;

    firebase?: boolean,

    crashlytics?: boolean;
    crashlyticsAppId?: string;
    
    ads?: boolean;
    admob?: boolean;
    admobAppId?: string;
    admobBannerId?: string;
    admobInterstitialId?: string;
    admobAppOpenId?: string;
    admobRewardedId?: string;

}


export type GradleSetup = {
    minSdk: number,
    targetSdk: number,
    compileSdk: number,

    ndkVersion: string,
    cmakeVersion: string,

    buildToolsVersion: string,
    root: {
        plugins: GradleSetupPlugin[],
    },

    module: {
        plugins: GradleSetupPlugin[],
    },

    dependencies: GradleSetupDependency[],
    dependenciesDebug: GradleSetupDependency[],
    dependenciesRelease: GradleSetupDependency[],
    dependenciesFirebase: GradleSetupDependency[],
    dependenciesAds: GradleSetupDependency[],

}

// export type GradleSetupPlugin = {
//     id: string,
//     version?: string,
// }

export type GradleSetupPlugin = {
    id: string,
    version?: string,
    platform?: boolean,

    kotlin?: boolean; // condition
    ads?: boolean, // condition
    firebase?: boolean, // condition
    crashlytics?: boolean;  // condition
    multiplayer?: boolean;
}


export type GradleSetupDependency = {
    name: string,
    version: string,
    platform?: boolean,

    kotlin?: boolean;
    ads?: boolean;  // condition
    firebase?: boolean;
    crashlytics?: boolean; // condition
    multiplayer?: boolean;
}

export type GradleDependencyOrPlugin = GradleSetupDependency | GradleSetupPlugin;

export type Lang = "xml" | "java" | "kotlin" | "gradle" | "cmake" | "cpp";

