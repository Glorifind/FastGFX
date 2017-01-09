#ifndef EMSCRIPTEN

#ifndef FGFX_TEXTUREMANAGER_H
#define FGFX_TEXTUREMANAGER_H

#include "SpritesTexture.h"

#include <map>
#include <vector>
#include <memory>

namespace fgfx {

  class TextureInfo {
  public:
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Image> image;
  };

  class TextureManager {
  public:

    /// TODO HTTP sprites support, loading sprites map
    std::map<std::string, SpriteInfo> loadedSprites;
    std::map<std::string, TextureInfo> loadedTextures;

    int maxSize;
    int spacing;

    std::vector<std::shared_ptr<SpritesTexture>> spriteTextures;

    TextureManager();
    ~TextureManager();

    void loadSprite(std::shared_ptr<Sprite> sprite);
    void loadSpriteFont(std::shared_ptr<SpriteFont> sprite);
    void loadTexture(std::shared_ptr<Texture> texture);

    void clean();

  };

}

#endif //FGFX_TEXTUREMANAGER_H
#endif //EMSCRIPTEN