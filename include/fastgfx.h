//
// Created by Michał Łaszczewski on 01/07/16.
//

#ifndef ENGINE_FGFX_H
#define ENGINE_FGFX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../src/config.h"

#include <memory>
#include <functional>

namespace fgfx {
  class Engine;
  class LineLayer;
  class SpriteLayer;
  class Layer;
  class Sprite;
  class PolygonLayer;
  class SpriteFont;
  class Texture;
  class TexturedLayer;
  class TextureTransitionLayer;

  int init();
  void destroy();
  extern std::shared_ptr<Engine> engine;

#ifdef USE_GLFW
  extern GLFWwindow* glfwWindow;
#endif

#ifdef __ANDROID
extern AAssetManager* assetManager;
#endif

  void loadFile(std::string path, std::function<void(std::string)>);
}

#include "../src/Sprite.h"
#include "../src/Engine.h"
#include "../src/LineLayer.h"
#include "../src/SpriteLayer.h"
#include "../src/Layer.h"
#include "../src/PolygonLayer.h"
#include "../src/SpriteFont.h"
#include "../src/Texture.h"
#include "../src/TexturedLayer.h"
#include "../src/TextureTransitionLayer.h"
#include "../src/Image.h"

#endif //ENGINE_FGFX_H
