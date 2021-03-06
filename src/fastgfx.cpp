//
// Created by Michał Łaszczewski on 09/07/16.
//

#include <fastgfx.h>

#include <vector>
#include <memory>
#include <fastgfx.h>
#include "config.h"

#ifndef EMSCRIPTEN
#include <thread>
#endif

namespace fgfx {

#ifdef __ANDROID
  AAssetManager* assetManager;
#endif

#ifndef EMSCRIPTEN
  std::thread loaderThread;
  bool finished;
  void imageLoader();
#endif

#ifdef USE_GLFW
  GLFWwindow* glfwWindow;
#endif

  void destroy() {
#ifdef USE_GLFW
    //glfwDestroyWindow(fgfx::glfwWindow);
    glfwTerminate();
#endif
#ifndef EMSCRIPTEN
    finished = true;
    if(loaderThread.joinable()) loaderThread.join();
#endif
  }

  int init() {
#ifndef EMSCRIPTEN
    finished = false;
#endif
#ifdef EMSCRIPTEN
    EM_ASM(Module.initializeFastGfxRuntime());
    fgfx_log("ENGINE INITIALIZATION\n");
    fgfx::engine = std::make_shared<fgfx::Engine>();
    EM_ASM(Module.initializeFastGfxTextureManager());
#endif
#ifdef USE_GLFW
    if (!glfwInit()) {
      fgfx_log("could not init glfw")
      return -1;
    }
    glfwWindowHint(GLFW_ALPHA_BITS, 8);

    glfwWindow = glfwCreateWindow(800, 600, "FGFX", NULL, NULL);
    if (!glfwWindow) {
      fgfx_log("could not create window")
      return -1;
    }

    glfwMakeContextCurrent(glfwWindow);

    fgfx::engine = std::make_shared<fgfx::Engine>();
#endif
#ifdef __ANDROID
    fgfx::engine = std::make_shared<fgfx::Engine>();
#endif
#ifndef EMSCRIPTEN
    loaderThread = std::thread(imageLoader);
#endif
    return 0;
  }

#ifdef EMSCRIPTEN
  int lastFileLoad = 0;
  std::map<int, std::function<void(std::string)>> fileLoadCallbacks;
  extern "C" {
    FGFX_API void fgfx_fileLoaded(int id, const char* data, int length) {
      fileLoadCallbacks[id](std::string(data,length));
      fileLoadCallbacks.erase(id);
    }
  }
#endif

  void loadFile(std::string path, std::function<void(std::string)> cb) {
#ifndef EMSCRIPTEN
    cb(loadBuffer(path));
#endif
#ifdef EMSCRIPTEN
    lastFileLoad++;
    fileLoadCallbacks[lastFileLoad] = cb;
    EM_ASM_INT({
      var name = Module.Pointer_stringify($0);
      window.fgfxLoader.loadFile("assets/"+name).then(function(result) {
        var buffer = Module._malloc(result.length);
        Module.writeStringToMemory(result, buffer);
        Module._fgfx_fileLoaded($1, buffer, result.length);
        Module._free(buffer);
      }, function(error) {
        throw error;
      });
    }, path.c_str(), lastFileLoad);
#endif
  }
}

#ifdef EMSCRIPTEN
using namespace fgfx;

extern "C" {

FGFX_API
void fgfx_render(double time, float delta, int widthp, int heightp, double rw, double rh) {
  fgfx::engine->realWidth = rw;
  fgfx::engine->realHeight = rh;
  fgfx::engine->render(time, delta, widthp, heightp);
}

FGFX_API
int fgfx_getSpritesToLoadCount() {
  return fgfx::engine->getSpritesToLoadCount();
}

FGFX_API
fgfx::Sprite* fgfx_getSpriteToLoad(int id) {
  return fgfx::engine->getSpriteToLoad(id).get();
}

FGFX_API
void fgfx_clearSpritesToLoad(int id) {
  fgfx::engine->clearSpritesToLoad();
}

FGFX_API
fgfx::Sprite* fgfx_getSprite(const char* spriteName) {
  return fgfx::engine->getSprite(spriteName).get();
}

FGFX_API
const char* fgfx_Sprite_getName(fgfx::Sprite* sprite) {
  return sprite->name.c_str();
}

FGFX_API
void fgfx_Sprite_setTextureFragment(fgfx::Sprite* sprite, int textureIdp, float left, float top, float right, float bottom,
                                    int widthp, int heightp, bool preloaded) {
  sprite->setTextureFragment(textureIdp, left, top, right, bottom, widthp, heightp, preloaded);
}

FGFX_API
int fgfx_getSpriteFontsToLoadCount() {
  return fgfx::engine->getSpriteFontsToLoadCount();
}

FGFX_API
fgfx::SpriteFont* fgfx_getSpriteFontToLoad(int id) {
  return fgfx::engine->getSpriteFontToLoad(id).get();
}

FGFX_API
void fgfx_clearSpriteFontsToLoad(int id) {
  fgfx::engine->clearSpriteFontsToLoad();
}

FGFX_API
const char* fgfx_SpriteFont_getName(fgfx::SpriteFont* font) {
  return font->name.c_str();
}

FGFX_API
void fgfx_SpriteFont_setHeight(fgfx::SpriteFont* font, float linep, float basep) {
  font -> setHeight(linep, basep);
}

FGFX_API
void fgfx_SpriteFont_setCharacter(fgfx::SpriteFont* font, unsigned short character, fgfx::Sprite* sprite,
                            float xSize, float ySize, float xOffset, float yOffset, float advance) {
  font -> setCharacter(character,sprite->shared_from_this(),glm::vec2(xSize,ySize),glm::vec2(xOffset,yOffset),advance);
}


FGFX_API
int fgfx_getTexturesToLoadCount() {
  return fgfx::engine->getTexturesToLoadCount();
}

FGFX_API
fgfx::Texture* fgfx_getTextureToLoad(int id) {
  return fgfx::engine->getTextureToLoad(id).get();
}

FGFX_API
void fgfx_clearTexturesToLoad(int id) {
  fgfx::engine->clearTexturesToLoad();
}

FGFX_API
fgfx::Texture* fgfx_getTexture(const char* textureName) {
  return fgfx::engine->getTexture(textureName).get();
}

FGFX_API
const char* fgfx_Texture_getName(fgfx::Texture* texture) {
  return texture->name.c_str();
}

FGFX_API
void fgfx_Texture_setTexture(fgfx::Texture* texture, int textureId) {
  texture->texture = textureId;
  texture->initialized = true;
}

FGFX_API
int fgfx_Texture_getTexture(fgfx::Texture* texture) {
  if(!texture->initialized) return -1;
  return texture->texture;
}

};

#endif
