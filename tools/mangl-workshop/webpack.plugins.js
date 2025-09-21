const ForkTsCheckerWebpackPlugin = require('fork-ts-checker-webpack-plugin');
//const NodePolyfillPlugin = require("node-polyfill-webpack-plugin")

//const webpack = require("webpack");

const path = require('path')
const CopyWebpackPlugin = require('copy-webpack-plugin');
const assets = ['static', 'img', 'css', 'fonts']; // asset directories


// const CopyWebpackPlugin = require('copy-webpack-plugin');
// const path = require('path');


// const assets = ['assets'];
// const copyPlugins = new CopyWebpackPlugin(
//   {
//     patterns: assets.map((asset) => ({
//       from: path.resolve(__dirname, 'src', asset),
//       to: path.resolve(__dirname, '.webpack/renderer', asset)
//     }))
//   }
// );



module.exports = [
  new ForkTsCheckerWebpackPlugin(),
  //new NodePolyfillPlugin(),
  //new webpack.ExternalsPlugin("commonjs", ["leveldown"])

  /*
  new CopyWebpackPlugin({
    patterns: [
      'static',
    ]
  }),
  */
 
  
  new CopyWebpackPlugin({
    patterns: [
      {
        from: path.resolve(__dirname, 'static'),
        to: path.resolve(__dirname, '.webpack/main/')
      }
    ]
  }),
  

];
