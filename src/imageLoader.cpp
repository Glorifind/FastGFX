
#ifndef EMSCRIPTEN // implemented in javascript
#ifndef USE_GLFM
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#endif

#include <png.h>

#include <fastgfx.h>
#include "TextureManager.h"

namespace fgfx {
  extern bool finished;

  TextureManager textureManager;

  void loadSprite(std::string path) {
    std::shared_ptr<Sprite> sprite = fgfx::engine->getSprite(path);
    textureManager.loadSprite(sprite);
  }

  void loadTexture(std::string path) {
    std::shared_ptr<Texture> texture = fgfx::engine->getTexture(path);
    textureManager.loadTexture(texture);
  }

  void loadSpriteFont(std::string path) {
    std::shared_ptr<SpriteFont> spriteFont = fgfx::engine->getSpriteFont(path);
    textureManager.loadSpriteFont(spriteFont);
  }

  void imageLoader() {
    while(!finished) {
      Engine::LoadTask task;
      if (!fgfx::engine->loadQueue.try_dequeue(task)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(23));
        continue;
      }
      if(task.path.size() == 0) throw "WTF?!";
      switch(task.type) {
        case Engine::AssetType::Sprite :
          loadSprite(task.path);
          break;
        case Engine::AssetType::SpriteFont :
          loadSpriteFont(task.path);
          break;
        case Engine::AssetType::Texture :
          loadTexture(task.path);
          break;
      }
    }
  }

}

#endif