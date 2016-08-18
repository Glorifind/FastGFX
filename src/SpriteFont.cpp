//
// Created by Michał Łaszczewski on 04/05/16.
//

#include "SpriteFont.h"

#include <glm/gtc/matrix_transform.hpp>

#include <emscripten.h>

namespace fgfx {

  SpriteFont::SpriteFont(std::string namep) : name(namep) {

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

  void SpriteFont::renderColorCodedString(std::shared_ptr<SpriteLayer> layer, glm::mat4 mat, glm::vec4 color, std::string text) {
    glm::vec4 ccolor = color;
    float x = 0;
    for(int i = 0; i<text.size(); i++) {
      char c = text[i];

      if(c == L'\\') {
        i++;
        int j;
        for(j = i+1; j<text.size(); j++) {
          if(text[j] == '\\') break;
        }
        int len = j-i;
        //  emscripten_log(EM_LOG_CONSOLE,"COLOR CODE %d %d %d\n",i,j,len);
        if(len == 0) {
          c = '\\';
        } else {
          if(len>2) {
            std::string rs("0");
            std::string gs("0");
            std::string bs("0");
            std::string as("f");
            switch(len) {
              case 3 :
                rs = text.substr(i+0,1);
                gs = text.substr(i+1,1);
                bs = text.substr(i+2,1);
                as = std::string("f");
                break;
              case 4 :
                rs = text.substr(i+0,1);
                gs = text.substr(i+1,1);
                bs = text.substr(i+2,1);
                as = text.substr(i+3,1);
                break;
              case 6 :
                rs = text.substr(i+0,2);
                gs = text.substr(i+2,2);
                bs = text.substr(i+4,2);
                as = std::string("ff");
                break;
              case 8 :
                rs = text.substr(i+0,2);
                gs = text.substr(i+2,2);
                bs = text.substr(i+4,2);
                as = text.substr(i+6,2);
                break;
            }
            if(rs.size() == 2) {
              ccolor = glm::vec4(
                  std::stoi(rs,NULL,16)/255,
                  std::stoi(gs,NULL,16)/255,
                  std::stoi(bs,NULL,16)/255,
                  std::stoi(as,NULL,16)/255);
            } else {
              ccolor = glm::vec4(
                  std::stoi(rs,NULL,16)/15,
                  std::stoi(gs,NULL,16)/15,
                  std::stoi(bs,NULL,16)/15,
                  std::stoi(as,NULL,16)/15);
            }
          } else {
            ccolor = color;
          }
          i=j+1;
          if(i >= text.size()) break;
          c = text[i];
        }
      }
      auto it = characters.find(c);
      if (it != characters.end()) {
        SpriteCharacter charStruct = it->second;
        glm::vec2 pos(x + charStruct.size.x * 0.5 - charStruct.offset.x * 0.5,
                      -charStruct.size.y * 0.5 - charStruct.offset.y);
        auto translate = glm::translate(glm::mat4(), glm::vec3(pos, 0));
        auto scale = glm::scale(glm::mat4(), glm::vec3(charStruct.size.x * 0.5, charStruct.size.y * 0.5, 1));
        layer->bufferSprite(charStruct.sprite, mat * translate * scale, ccolor);
        x += charStruct.advance;
      }

    }
  }

  glm::vec2 SpriteFont::measureColorCodeString(std::string text) {
    float height=0;
    float width=0;
    for(int i=0; i<text.size(); i++) {
      char c = text[i];
      if(c == L'\\') {
        i++;
        int j;
        for(j = i+1; j<text.size(); j++) {
          if(text[j] == '\\') break;
        }
        int len = j-i;
        //  emscripten_log(EM_LOG_CONSOLE,"COLOR CODE %d %d %d\n",i,j,len);
        if(len == 0) {
          c = '\\';
        } else {
          i=j+1;
          if(i >= text.size()) break;
          c = text[i];
        }
      }
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

