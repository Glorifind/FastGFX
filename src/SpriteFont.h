//
// Created by Michał Łaszczewski on 04/05/16.
//

#ifndef ENGINE_SPRITEFONT_H
#define ENGINE_SPRITEFONT_H

#include <glm/glm.hpp>
#include "SpriteLayer.h"

namespace fgfx {

  class SpriteCharacter {
  public:
    std::shared_ptr<Sprite> sprite;
    glm::vec2 size;
    glm::vec2 offset;
    float advance;
  };

  class SpriteFont {
  public:
    std::string name;
    std::map<unsigned short, SpriteCharacter> characters;

    float lineHeight;
    float baseLine;
    glm::vec2 scale;

    SpriteFont(std::string namep);
    ~SpriteFont();

    void setHeight(float linep, float basep);
    void setCharacter(unsigned short character, std::shared_ptr<Sprite> sprite, glm::vec2 size, glm::vec2 offset,
                      float advance);
    void renderString(std::shared_ptr<SpriteLayer> layer, glm::mat4 mat, glm::vec4 color, std::string text);
    glm::vec2 measureString(std::string text);
  };

}

#endif //ENGINE_SPRITEFONT_H
