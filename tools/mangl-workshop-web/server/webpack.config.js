const path = require('path');
const webpack = require('webpack');
const nodeExternals = require('webpack-node-externals');

let buildDir = 'build';
let srcMain = './src/main.ts';

module.exports = {
  mode: 'production',
  devtool: 'source-map',
  entry: {
    server: srcMain,
  },
  output: {
    path: path.join(__dirname, buildDir),
    filename: 'index.js',
  },
  target: 'node',
  node: {
    __dirname: false,
    __filename: false,
  },
  externals: [nodeExternals()],
  module: {
    rules: [
      {
        exclude: /node_modules/,
      },
    ],
  },
};
