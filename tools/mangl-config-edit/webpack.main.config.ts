import type {Configuration} from 'webpack';

import {rules} from './webpack.rules.ts';
import {plugins} from './webpack.plugins.ts';

let devMode = process.env.WEBPACK_SERVE === 'true' ? true : false;

export const mainConfig: Configuration = {
    entry: './src/main.ts',
    target: 'electron-main',

    watch: devMode,
    watchOptions: devMode
        ? {
              aggregateTimeout: 2000,
              ignored: ['node_modules/**'],
          }
        : undefined,

    // Put your normal webpack config below here
    module: {
        rules,
    },
    plugins,
    resolve: {
        extensions: ['.js', '.ts', '.jsx', '.tsx', '.css', '.json', '.yaml'],
    },
};
