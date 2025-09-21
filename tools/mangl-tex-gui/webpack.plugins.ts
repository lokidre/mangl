import type IForkTsCheckerWebpackPlugin from 'fork-ts-checker-webpack-plugin';

import path from 'path';
import CopyWebpackPlugin from 'copy-webpack-plugin';
const assets = ['static', 'img', 'css', 'fonts']; // asset directories


// eslint-disable-next-line @typescript-eslint/no-var-requires
import ForkTsCheckerWebpackPlugin from 'fork-ts-checker-webpack-plugin';

export const plugins = [
  new ForkTsCheckerWebpackPlugin({
    logger: 'webpack-infrastructure',
  }),

  new CopyWebpackPlugin({
    patterns: [
      {
        from: path.resolve(__dirname, 'static'),
        to: path.resolve(__dirname, '.webpack/main/')
      }
    ]
  }),

];
