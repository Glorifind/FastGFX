var fs = require('fs')
var webpack = require("webpack");
var path = require('path');

var compiler = webpack({
  context: __dirname,
  entry: "./runtime.js",
  devtool: "source-map",
  module: {
    loaders: [
      {
        test: /\.jsx?$/,
        exclude: [/(node_modules)/,/evaluator\.js/],
        loader: 'babel', // 'babel-loader' is also a legal name to reference
        query: {
          presets: ['es2015']
        }
      }
    ]
  },
  output: {
    path: __dirname,
    filename: "../runtime-js-compiled.js"//,
    // sourceMapFile: "index-compiled.js.map"
  },
  node: {
    fs: "empty"
  },
  plugins:[ /*
   new webpack.DefinePlugin({
   __VERSION__ : version
   }),
   new webpack.optimize.UglifyJsPlugin({
   sourceMap: true,
   mangle: true
   }),
   new webpack.optimize.OccurrenceOrderPlugin(),
   new webpack.optimize.DedupePlugin()//*/
  ]
})

compiler.run(function(err, stats) {
  if(err) {
    console.error('WEBPACK ERROR',err)
  }
  if(stats.compilation.errors && stats.compilation.errors.length) {

    console.error("ERRORS",stats.compilation.errors)
  }
});
