#ifndef SPRITE_H
#define SPRITE_H

#include <glm/vec2.hpp>
#include <string>
#include <memory>

namespace fgfx {

  class Sprite : public std::enable_shared_from_this<Sprite> {
    friend class SpriteBuffer;
  public:
    glm::vec2 coords[4];
    std::string name;
    int textureId;
    int width;
    int height;
    float ratio;
    int lastUseTime;
    bool preloaded;
    bool unloaded;
    bool locked;
    Sprite(std::string namep);
    void setTextureFragment(int textureIdp, float left, float top, float right, float bottom, int widthp, int heightp,
                            bool preloadedp);
    std::string getSpriteName();
    void unload();
  };

};

#endif // SPRITE_H
