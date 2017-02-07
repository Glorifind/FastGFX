//
// Created by Michał Łaszczewski on 04/05/16.
//

#include "SpriteFont.h"

#include <glm/gtc/matrix_transform.hpp>

#include "config.h"

namespace fgfx {

  SpriteFont::SpriteFont(std::string namep) : name(namep) {
    scale = glm::vec2(1,1);
  }
  SpriteFont::~SpriteFont() {

  }

  void SpriteFont::setHeight(float linep, float basep) {
    lineHeight = linep;
    baseLine = basep;
  }
  void SpriteFont::setCharacter(unsigned short character, std::shared_ptr<Sprite> sprite, glm::vec2 size,
                                glm::vec2 offset, float advance) {
    //emscripten_log(EM_LOG_ERROR,"SETUP %s CHAR: %c %d\n",name.c_str(),character,character);
    SpriteCharacter characterStruct;
    characterStruct.size = size;
    characterStruct.offset = offset;
    characterStruct.sprite = sprite;
    characterStruct.advance = advance;
    characters[character] = characterStruct;
  }
  void SpriteFont::renderString(std::shared_ptr<SpriteLayer> layer, glm::mat4 mat, glm::vec4 color, std::string text){
    //emscripten_log(EM_LOG_ERROR,"RENDER %s STRING: %s\n",name.c_str(),text.c_str());
    float x=0;
    for(int i=0; i<text.size(); i++) {

      char c = text[i];
      //emscripten_log(EM_LOG_ERROR,"CHAR: %c %d\n",c,c);
      auto it=characters.find(c);
      if(it!=characters.end()) {
        SpriteCharacter charStruct = it->second;
        //emscripten_log(EM_LOG_ERROR,"RENDER CHAR: %c %d\n",c,c);
        glm::vec2 pos(x + charStruct.size.x*0.5 - charStruct.offset.x*0.5,-charStruct.size.y*0.5-charStruct.offset.y);
        auto translate = glm::translate(glm::mat4(),glm::vec3(pos,0));
        auto scale = glm::scale(glm::mat4(),glm::vec3(charStruct.size.x*0.5,charStruct.size.y*0.5,1));
        layer->rawBufferSprite(charStruct.sprite,mat*translate*scale,color);
        x += charStruct.advance;
      }
    }
  }

  glm::vec2 SpriteFont::measureString(std::string text) {
    float height=0;
    float width=0;
    for(int i=0; i<text.size(); i++) {
      char c = text[i];
      auto it=characters.find(c);
      if(it!=characters.end()) {
        SpriteCharacter charStruct = it->second;
        float h = charStruct.size.y+charStruct.offset.y;
        if(h>height) height = h;
        width+=charStruct.advance;
      }
    }
    return glm::vec2(width,lineHeight);
  }

}

