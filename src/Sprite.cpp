#include "Sprite.h"

#include <emscripten.h>
#include <cstring>

namespace fgfx {

  Sprite::Sprite(std::string namep) : name(namep), textureId(-1), width(0), height(0) {
  }

  std::string Sprite::getSpriteName() {
   // emscripten_log(EM_LOG_ERROR, "GET SPRITE NAME = %s\n",stringName.c_str());
    return name;
  }

  void Sprite::setTextureFragment(int textureIdp, float left, float top, float right, float bottom, int widthp,
                                  int heightp, bool preloadedp) {
    emscripten_log(EM_LOG_ERROR, "SET TEXTURE FRAGMENT %s %d %d %d\n",name.c_str(),textureIdp,width,height);
    textureId = textureIdp;
    coords[0] = glm::vec2(left,bottom);
    coords[1] = glm::vec2(right,bottom);
    coords[2] = glm::vec2(right,top);
    coords[3] = glm::vec2(left,top);
    width = widthp;
    height = heightp;
    preloaded = preloadedp;
  }

  void Sprite::unload() {
    textureId = -1;
    unloaded = true;
  }

};
