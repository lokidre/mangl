
export type GenSampleContext = {
    config: ProjectConfig;
    project: SampleProject;

    yaml: any;
    genproj: any;

    projectDir: string;
    manglRootDir: string;
    manglSamplesDir: string;
    manglSamplesCommonDir: string;
}

export type ProjectConfig = {
    apeal_dir: string;
    apeal_scripts_dir: string,  // Directory where scripts are found to load YAML

    cognit_dir: string;
    cognit_scripts_dir: string;

    mangl_dir: string,
    mangl_include_dir: string,
    mangl_src_dir: string,
    mangl_3rdparty_dir: string,
    mangl_scripts_dir: string,
    mangl_cmake_dir: string,
}


export type SampleProject = {
    name: string;
    compile_definitions?: string[];
    include_directories?: string[];
    set: string[];
    cmake?: string;

    targets: SampleProjectTarget[];

    options?: SampleProjectOptions;
    console?: boolean;
    multiplayer?: boolean;
    bluetooth?: boolean;

    ads?: boolean;
    admob?: boolean;
    admobAppId?: string;
    admobBannerId?: string;
    admobInterstitialId?: string;
}

export type SampleProjectOptions = {
    multiplayer?: boolean;
    console?: boolean;
    bluetooth?: boolean;
}

export type SampleProjectTarget = {
    name: string;
    source?: string;
    sources: string[];
}