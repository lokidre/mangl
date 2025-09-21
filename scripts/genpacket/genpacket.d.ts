export type ProjectConfig = {
    apeal_scripts_dir: string,  // Directory where scripts are found to load YAML
}

export type GenPacketParams = {
    config: ProjectConfig;  // Project config object
    desc: string;  // Project graphics description file
    target: string;  // Output file name
    include: string;
    nohpp?: boolean;  // Do not generate c++ header files
}

export type PropertyType = 'pod' | 'char' | 'int' | 'string' | 'enum' | 'set' | 'vector' | 'map' | 'array' | 'object'

export type Property = {
    type: PropertyType;
    defaultValue: string;
    elemType: string;
    valueType: PropertyType;
    underlying: string;
}
