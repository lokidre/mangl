module.exports = {
  entry: './src/main.ts',
  target: 'electron-main',

  // Put your normal webpack config below here
  module: {
    rules: require('./webpack.rules'),
  },

  resolve: {
    extensions: ['.js', '.ts', '.jsx', '.tsx', '.css', '.json'],
  },

  externals: {
  },
}
