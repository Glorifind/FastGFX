#ifndef SPRITE_H
#define SPRITE_H

#include <glm/vec2.hpp>
#include <string>

#include <ApiObject.h>

namespace fgfx {

  class Sprite : public ApiObject {
    friend class SpriteBuffer;
  protected:
    glm::vec2 coords[4];
  public:
    std::string name;
    int textureId;
    int width;
    int height;
    int lastUseTime;
    bool preloaded;
    bool unloaded;
    Sprite(std::string namep);
    void setTextureFragment(int textureIdp, float left, float top, float right, float bottom, int widthp, int heightp);
    std::string getSpriteName();
    void unload();
  };

};

#endif // SPRITE_H
