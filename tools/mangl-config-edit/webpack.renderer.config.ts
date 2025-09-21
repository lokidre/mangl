import type { Configuration } from 'webpack';

import { rules } from './webpack.rules.ts';
import { plugins } from './webpack.plugins.ts';

rules.push({
  test: /\.css$/,
  use: [{ loader: 'style-loader' }, { loader: 'css-loader' }],
});

export const rendererConfig: Configuration = {
  target: 'electron-renderer',

  module: {
    rules,
  },
  plugins,
  resolve: {
    extensions: ['.js', '.ts', '.jsx', '.tsx', '.css', '.yaml'],
  },
};
