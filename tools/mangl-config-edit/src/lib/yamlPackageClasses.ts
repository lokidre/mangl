
export class YamlPackage {
    config: PackageDescConfig;
}

export class PackageDescConfig {
    sections: YamlSection[];
}

export class YamlSection {
    name: string;
    description?: string;
    properties?: YamlProp[]; //VarObj[]
    custom?:boolean

    constructor(name: string, description?: string, properties?: YamlProp[]) {
        this.name = name;
        this.description = description || '';
        this.properties = properties || [];
    }
}

export class YamlProp {
    name: string;
    description?: string;
    type?: 'string' | 'int' | 'bool' | 'real' | 'selection';
    value?: any;
    selection?: string[];
    editable?: boolean;
}
