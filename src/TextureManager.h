#ifndef EMSCRIPTEN

#ifndef TANKS_TEXTUREMANAGER_H
#define TANKS_TEXTUREMANAGER_H

#include "SpritesTexture.h"

#include <map>
#include <vector>
#include <memory>

namespace fgfx {

  class TextureManager {
  public:

    /// TODO HTTP sprites support, loading sprites map
    std::map<std::string, SpriteInfo> loadedSprites;

    int maxSize;
    int spacing;

    std::vector<std::shared_ptr<SpritesTexture>> spriteTextures;

    TextureManager();
    ~TextureManager();

    void loadSprite(std::shared_ptr<Sprite> sprite);
    void loadSpriteFont(std::shared_ptr<SpriteFont> sprite);

    void clean();

  };

}

#endif //TANKS_TEXTUREMANAGER_H
#endif //EMSCRIPTEN