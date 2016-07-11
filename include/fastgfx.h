//
// Created by Michał Łaszczewski on 01/07/16.
//

#ifndef ENGINE_FGFX_H
#define ENGINE_FGFX_H

#include <emscripten.h>
#define fgfx_log(...) emscripten_log(EM_LOG_ERROR, __VA_ARGS__)

#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

namespace fgfx {
  class Engine;
  class LineLayer;
  class SpriteLayer;
  class Layer;
  class Sprite;
  class PolygonLayer;
  class SpriteFont;

  int init();
  extern std::shared_ptr<Engine> engine;
}

#include "../src/Sprite.h"
#include "../src/Engine.h"
#include "../src/LineLayer.h"
#include "../src/SpriteLayer.h"
#include "../src/Layer.h"
#include "../src/PolygonLayer.h"
#include "../src/SpriteFont.h"


#endif //ENGINE_FGFX_H
