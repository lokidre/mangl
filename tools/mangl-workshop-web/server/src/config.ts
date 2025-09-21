import fs from 'fs';
import path from 'path';
import yaml from 'js-yaml';

import {makeFilePath} from './tslib/filename';
import {ConfigDatabase} from './tsback/config_database';
import {ConfigAws} from './tsback/config_aws';


import './tsback/aws/aws_init';


export interface ConfigServer {
    port: number;
    api: string;
}

export interface ConfigLocalData {
    uploads: string;
}

export interface ConfigDev {
    rootToken?: string;
}

export interface Config {
    server: ConfigServer;
    db: ConfigDatabase;
    aws: ConfigAws;
    local: ConfigLocalData;
    dev?: ConfigDev;
}

export class Setup {
    configDir = path.join(path.dirname(__dirname), 'config');
    configFile = 'config.yaml';

    apiVersionMajor = 1;
    apiVer = 'v1';
    apiRoot = '/api/v1';

    constructor() {
        let configDirs = ['/srv/etc/southgate', '.', path.join(path.dirname(__dirname), 'config')];
        for (let dir of configDirs) {
            let configPath = makeFilePath(dir, this.configFile);
            if (fs.existsSync(configPath)) {
                this.configDir = dir;
                break;
            }
        }
    }
}

export function loadConfig(): Config {
    let configPath = makeFilePath(setup_.configDir, setup_.configFile);

    log_.info('Loading Config:', configPath);

    let configData = fs.readFileSync(configPath, 'utf-8');

    let config = yaml.load(configData.toString(), {
        filename: configPath,
        //schema: yaml.FAILSAFE_SCHEMA,
    }) as Config;

    if (!config.dev) {
        log_.setSettings({minLevel: 'fatal',})
    }

    return config;
}

global.setup_ = new Setup();
global.config_ = loadConfig();

