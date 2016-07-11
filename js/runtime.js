import { TextureManager } from "./TextureManager.js"
import { Animator } from "./Animator.js"

var canvas = document.getElementById('fgfx_canvas')
var gl = canvas.getContext('webgl') || canvas.getContext('experimental-webgl')

var animator = new Animator(false)

var textureManager

window.fgfxLoader = window.fgfxLoader || {
    loadImage: function(name) {
      var url = name
      return new Promise((resolve,reject) => {
        var image = new Image()
        image.crossOrigin = "anonymous"
        image.src = url
        if(image.complete) {
          return resolve(image)
        }
        image.onload=function() {
          resolve(image)
        }
        image.onerror=function() {
          reject('Could not load image '+url)
        }
      })
    }
  }

window.Module = window.Module || {}
window.Module.initializeFastGfxRuntime = function() {
  console.log("INIT!")
  GLctx = gl
  animator.addAnimation(animateFgfx)
}
window.Module.initializeFastGfxTextureManager = function() {
  textureManager = new TextureManager(gl,window.fgfxLoader)
}

var animateFgfx = function(time,delta) {
  textureManager.reload()
  GLctx = gl
  if( canvas.width != canvas.clientWidth || canvas.height != canvas.clientHeight ) {
    canvas.width = canvas.clientWidth
    canvas.height = canvas.clientHeight
    gl.viewport( 0, 0, canvas.width, canvas.height )
    console.log("CANVAS RESIZE")
  }
  window.Module._fgfx_render(time, delta, canvas.width, canvas.height)
}
