#include "Engine.h"
#include <emscripten.h>

namespace fgfx {

  std::shared_ptr<Engine> engine;

  Engine::Engine() {
    SpriteLayer::initializeSpriteProgram();
    LineLayer::initializeLineProgram();
    PolygonLayer::initializePolygonProgram();

    //currentTime=0;
  }

  Engine::~Engine() {

  }

  std::shared_ptr<Sprite> Engine::getSprite(std::string spriteName) {
    emscripten_log(EM_LOG_ERROR, "LOADING SPRITE!!! %s\n",spriteName.c_str());
    auto it = sprites.find(spriteName);
    if (it != sprites.end()) {
      std::shared_ptr<Sprite> sprite = it->second;
      if(sprite->unloaded) reloadSprite(sprite);
    }
    std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(spriteName);
    spritesToLoad.push_back(sprite);
    sprites[spriteName] = sprite;
    return sprite;
  }

  void Engine::reloadSprite(std::shared_ptr<Sprite> spritep) {
    spritesToLoad.push_back(spritep);
    spritep->unloaded=false;
  }

  std::shared_ptr<SpriteFont> Engine::getSpriteFont(std::string spriteFontName) {
    auto it = spriteFonts.find(spriteFontName);
    if (it != spriteFonts.end()) return it->second;
    std::shared_ptr<SpriteFont> spriteFont = std::make_shared<SpriteFont>(spriteFontName);
    spriteFontsToLoad.push_back(spriteFont);
    spriteFonts[spriteFontName] = spriteFont;
    return spriteFont;
  }

  int Engine::getSpritesToLoadCount() {
    return spritesToLoad.size();
  }
  std::shared_ptr<Sprite> Engine::getSpriteToLoad(int n) {
    return spritesToLoad[n];
  }
  void Engine::clearSpritesToLoad() {
    spritesToLoad.clear();
  }


  int Engine::getSpriteFontsToLoadCount() {
    return spriteFontsToLoad.size();
  }
  std::shared_ptr<SpriteFont> Engine::getSpriteFontToLoad(int n) {
    return spriteFontsToLoad[n];
  }
  void Engine::clearSpriteFontsToLoad() {
    spriteFontsToLoad.clear();
  }

  void Engine::setRenderFunction(std::function<void(float,float)> renderFunctionp) {
    renderFunction=renderFunctionp;
  }

  void Engine::render(double time, float delta, int widthp, int heightp) {
    renderTime = time;
    renderDelta = delta;
    width = widthp;
    height = heightp;
    if(renderFunction) renderFunction(time,delta);
  }

  std::shared_ptr<SpriteLayer> Engine::createSpriteLayer() {
    return std::make_shared<SpriteLayer>(this);
  }
  std::shared_ptr<PolygonLayer> Engine::createPolygonLayer() {
    return std::make_shared<PolygonLayer>(this);
  }
  std::shared_ptr<LineLayer> Engine::createLineLayer() {
    return std::make_shared<LineLayer>(this);
  }

};
