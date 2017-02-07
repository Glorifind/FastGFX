//
// Created by Michał Łaszczewski on 12/12/16.
//
#ifndef EMSCRIPTEN

#include "TextureManager.h"
#include "Image.h"
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <string>
#include <sstream>

namespace fgfx {

  TextureManager::TextureManager() {
    maxSize = 512;
    spacing = 2;
  }

  TextureManager::~TextureManager() {

  }

  void TextureManager::loadSprite(std::shared_ptr<Sprite> sprite) {
    auto it = loadedSprites.find(sprite->name);
    if(it != loadedSprites.end()) {
      SpriteInfo& info = it->second;
      info.sprite = sprite;
      info.sprite->setTextureFragment(info.texture, info.xmin, info.ymax, info.xmax, info.ymin,
                                      info.image->width, info.image->height, false);
      return;
    }
    /* TODO: Async loading
      var loading = this.loadingSprites.get(name)
      if(loading) {
        loading.sprite = sprite
        return;
      }
      var sprite = {
        coords: null,
        sprite: sprite,
        texture: 0,
        preloaded: false,
        name: name,
        image: null
      }
      this.loadingSprites.set(name,sprite)
     */

    SpriteInfo info;
    std::shared_ptr<Image> image = decodePngImage(loadBuffer(sprite->name));

    fgfx_log("DECODED SPRITE IMAGE %d x %d", image->width, image->height);

    info.image = image;
    info.sprite = sprite;

    bool bigger = false;
    bool added = false;
    while(image->width + spacing * 2 > maxSize || image->height + spacing * 2 > maxSize) {
      maxSize *= 2;
      bigger = true;
    }
    if(!bigger) {
      for (auto & spriteTexture : spriteTextures) {
        if (spriteTexture->tryAddSprite(info)) {
          added = true;
          break;
        }
      }
    }
    if(!added) { // new texture
      auto ntex = std::make_shared<SpritesTexture>(maxSize, spacing);
      spriteTextures.push_back(ntex);
      ntex->tryAddSprite(info);
    }
    loadedSprites[sprite->name] = info;
  }
  static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
  }
  std::istringstream& readAttrib(std::istringstream& in, std::string& name, std::string& value) {
    std::getline(in, name, '=');
    name = ltrim(name);
    in >> value;
    return in;
  }
  template <typename T>
  std::string to_string(T value)
  {
    std::ostringstream os ;
    os << value ;
    return os.str() ;
  }
  void uploadSpriteFont(const SpriteFontInfo& info) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.image->width, info.image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 info.image->data);
    glGenerateMipmap(GL_TEXTURE_2D);

    std::istringstream in(info.data);
    std::string line;
    while(std::getline(in,line)) {
      std::istringstream lineIn(line);
      std::string command;
      std::string key;
      std::string value;
      lineIn >> command;
      if(command == "info") {
        /// Not needed
      } else if(command == "common") {
        while(readAttrib(lineIn, key, value)) {
          if(key=="lineHeight") info.font->lineHeight = atof(value.c_str());
          else if(key=="base") info.font->baseLine = atof(value.c_str());
          else if(key=="scaleW") info.font->scale.x = atof(value.c_str());
          else if(key=="scaleH") info.font->scale.y = atof(value.c_str());
        }
      } else if(command == "info") {
        /// Not needed
      } else if(command == "page") {
        /// Not needed
      } else if(command == "chars") {
        /// Not needed
      } else if(command == "char") {
        unsigned short character;
        int x,y,width,height,xoffset,yoffset,xadvance;
        while(readAttrib(lineIn, key, value)) {
          if(key=="id") character = atoi(value.c_str());
          else if(key=="x") x = atoi(value.c_str());
          else if(key=="y") y = atoi(value.c_str());
          else if(key=="width") width = atoi(value.c_str());
          else if(key=="height") height = atoi(value.c_str());
          else if(key=="xoffset") xoffset = atoi(value.c_str());
          else if(key=="yoffset") yoffset = atoi(value.c_str());
          else if(key=="xadvance") xadvance = atoi(value.c_str());
        }
        std::string spriteName = "font_"+info.font->name+"_"+to_string(character);
        std::shared_ptr<Sprite> sprite = engine->getEmptySprite(spriteName);
        sprite->setTextureFragment(texture,
                                   1.0f * (x + 0.5f) / info.image->width,
                                   1.0f * (y + 0.5f) / info.image->height,
                                   1.0f * (x + width + 0.5f) / info.image->width,
                                   1.0f * (y + height + 0.5f) / info.image->height,
                                   width, height, true);
        info.font->setCharacter(character, sprite, glm::vec2(width, height), glm::vec2(xoffset, yoffset), xadvance);
      }
    }
  }
  void TextureManager::loadSpriteFont(std::shared_ptr<SpriteFont> spriteFont) {
    if(spriteFont->name.size() == 0 ) throw PngError();
    auto it = loadedSpriteFonts.find(spriteFont->name);
    if(it != loadedSpriteFonts.end()) {
      SpriteFontInfo& info = it->second;
      info.font = spriteFont;
      fgfx::engine->uploadQueue.enqueue([=]{
        uploadSpriteFont(info);
      });
      return;
    }
    SpriteFontInfo info;
    std::shared_ptr<Image> image = decodePngImage(loadBuffer(spriteFont->name + ".png"));
    auto fontDescription = loadBuffer(spriteFont->name + ".fnt");
    info.image = image;
    info.font = spriteFont;
    info.data = fontDescription;
    fgfx::engine->uploadQueue.enqueue([=]{
      uploadSpriteFont(info);
    });
    loadedSpriteFonts[spriteFont->name] = info;
  }
  void TextureManager::loadTexture(std::shared_ptr<Texture> texture) {
    if(texture->name.size() == 0 ) throw PngError();
    auto it = loadedTextures.find(texture->name);
    if(it != loadedTextures.end()) {
      TextureInfo& info = it->second;
      info.texture = texture;
      fgfx::engine->uploadQueue.enqueue([=]{
        texture->upload(info.image->width, info.image->height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, info.image->data);
      });
      return;
    }
    TextureInfo info;
    std::shared_ptr<Image> image = decodePngImage(loadBuffer(texture->name));
    info.image = image;
    info.texture = texture;
    fgfx::engine->uploadQueue.enqueue([=]{
      texture->upload(info.image->width, info.image->height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, info.image->data);
    });
    loadedTextures[texture->name] = info;
    return;
  }

  void TextureManager::clean() {
    for (auto & spriteTexture : spriteTextures) {
      spriteTexture->clean();
    }
  }

}

#endif

