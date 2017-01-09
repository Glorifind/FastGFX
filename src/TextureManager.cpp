//
// Created by Michał Łaszczewski on 12/12/16.
//
#ifndef EMSCRIPTEN

#include "TextureManager.h"
#include "Image.h"

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
  void TextureManager::loadSpriteFont(std::shared_ptr<SpriteFont> sprite) {

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

