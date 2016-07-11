//
// Created by Michał Łaszczewski on 09/07/16.
//

#include <fastgfx.h>

#include <vector>
#include <memory>
#include <fastgfx.h>
#include <emscripten.h>

namespace fgfx {
  int init() {
    EM_ASM(Module.initializeFastGfxRuntime());
    emscripten_log(EM_LOG_ERROR, "ENGINE INITIALIZATION\n");
    fgfx::engine = std::make_shared<fgfx::Engine>();
    EM_ASM(Module.initializeFastGfxTextureManager());
    return 0;
  }
}

using namespace fgfx;

extern "C" {

EMSCRIPTEN_KEEPALIVE
void fgfx_render(double time, float delta, int widthp, int heightp) {
  fgfx::engine->render(time, delta, widthp, heightp);
}

EMSCRIPTEN_KEEPALIVE
int fgfx_getSpritesToLoadCount() {
  return fgfx::engine->getSpritesToLoadCount();
}

EMSCRIPTEN_KEEPALIVE
fgfx::Sprite* fgfx_getSpriteToLoad(int id) {
  return fgfx::engine->getSpriteToLoad(id).get();
}

EMSCRIPTEN_KEEPALIVE
void fgfx_clearSpritesToLoad(int id) {
  fgfx::engine->clearSpritesToLoad();
}

EMSCRIPTEN_KEEPALIVE
const char* fgfx_Sprite_getName(fgfx::Sprite* sprite) {
  return sprite->name.c_str();
}

EMSCRIPTEN_KEEPALIVE
void fgfx_Sprite_setTextureFragment(fgfx::Sprite* sprite, int textureIdp, float left, float top, float right, float bottom,
                                    int widthp, int heightp) {
  sprite->setTextureFragment(textureIdp,left,top,right,bottom,widthp,heightp);
}

EMSCRIPTEN_KEEPALIVE
int fgfx_getSpriteFontsToLoadCount() {
  return fgfx::engine->getSpriteFontsToLoadCount();
}

EMSCRIPTEN_KEEPALIVE
fgfx::SpriteFont* fgfx_getSpriteFontToLoad(int id) {
  return fgfx::engine->getSpriteFontToLoad(id).get();
}

EMSCRIPTEN_KEEPALIVE
void fgfx_clearSpriteFontsToLoad(int id) {
  fgfx::engine->clearSpriteFontsToLoad();
}

EMSCRIPTEN_KEEPALIVE
const char* fgfx_Font_getName(fgfx::SpriteFont* font) {
  return font->name.c_str();
}

EMSCRIPTEN_KEEPALIVE
void fgfx_Font_setHeight(fgfx::SpriteFont* font, float linep, float basep) {
  font -> setHeight(linep, basep);
}

EMSCRIPTEN_KEEPALIVE
void fgfx_Font_setCharacter(fgfx::SpriteFont* font, unsigned short character, fgfx::Sprite* sprite,
                            float xSize, float ySize, float xOffset, float yOffset, float advance) {
  font -> setCharacter(character,sprite->shared_from_this(),glm::vec2(xSize,ySize),glm::vec2(xOffset,yOffset),advance);
}



};