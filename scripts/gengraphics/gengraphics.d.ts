export type ProjectConfig = {
    apeal_scripts_dir: string,  // Directory where scripts are found to load YAML
}

export type GenerateGraphicsParams = {
    config: ProjectConfig;  // - Project config object
    desc: string;  // Project graphics description file
    target: string;  // Output file name
    include: string;
    nohpp?: boolean;  // Do not generate c++ header files
    // texmake?: string;  // Tex maker path
    // texparams?: string | string[];
    // fontmake?: string; // Font maker path
    // fontparams?: string | string[];
}
