#include "Engine.h"
#include "config.h"
#include "fastgfx.h"
#include <chrono>

namespace fgfx {

  std::shared_ptr<Engine> engine;

  Engine::Engine() {
    SpriteLayer::initializeSpriteProgram();
    LineLayer::initializeLineProgram();
    PolygonLayer::initializePolygonProgram();

    renderTime = 0;
  }

  Engine::~Engine() {

  }

  std::shared_ptr<Sprite> Engine::getSprite(std::string spriteName) {
    auto it = sprites.find(spriteName);
    if (it != sprites.end()) {
      std::shared_ptr<Sprite> sprite = it->second;
      if(sprite->unloaded) reloadSprite(sprite);
      return sprite;
    }
    fgfx_log("LOADING SPRITE!!! %s\n",spriteName.c_str());
    std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(spriteName);
#ifdef EMSCRIPTEN
    spritesToLoad.push_back(sprite);
#else
    LoadTask task = { .type = AssetType::Sprite, .path = spriteName};
    loadQueue.enqueue(task);
#endif
    sprites[spriteName] = sprite;
    return sprite;
  }

  void Engine::reloadSprite(std::shared_ptr<Sprite> spritep) {
#ifdef EMSCRIPTEN
    spritesToLoad.push_back(spritep);
#else
    LoadTask task = { .type = AssetType::Sprite, .path = spritep->name};
    loadQueue.enqueue(task);
#endif
    spritep->unloaded=false;
  }

  std::shared_ptr<SpriteFont> Engine::getSpriteFont(std::string spriteFontName) {
    auto it = spriteFonts.find(spriteFontName);
    if (it != spriteFonts.end()) return it->second;
    std::shared_ptr<SpriteFont> spriteFont = std::make_shared<SpriteFont>(spriteFontName);
#ifdef EMSCRIPTEN
    spriteFontsToLoad.push_back(spriteFont);
#else
    LoadTask task = { .type = AssetType::SpriteFont, .path = spriteFontName};
    loadQueue.enqueue(task);
#endif
    spriteFonts[spriteFontName] = spriteFont;
    return spriteFont;
  }

#ifdef EMSCRIPTEN
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
#endif

  void Engine::setRenderFunction(std::function<void(float,float)> renderFunctionp) {
    renderFunction = renderFunctionp;
  }

  void Engine::render(double time, float delta, int widthp, int heightp) {
    #ifndef EMSCRIPTEN
    Engine::UploadTask task;
    while (fgfx::engine->uploadQueue.try_dequeue(task)) {
      task();
    }
    #endif

    renderTime = time;
    renderDelta = delta;
    width = widthp;
    height = heightp;
    if(renderFunction) renderFunction(time, delta);
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

#ifdef USE_GLFW
  void Engine::mainLoop() {
    glfwMakeContextCurrent(glfwWindow);

    double lastTime = glfwGetTime() * 1000.0;
    while (!glfwWindowShouldClose(glfwWindow)) {
      double now = glfwGetTime() * 1000.0;
      double delta = now - lastTime;
      glfwGetFramebufferSize(glfwWindow, &width, &height);
      int w, h;
      glfwGetWindowSize(glfwWindow, &w, &h);
      realWidth = 0.264 * w; // pixels to mm
      realHeight = 0.264 * h; // pixels to mm
      render(now, delta, width, height);
      lastTime = now;
      glfwSwapBuffers(glfwWindow);
      glfwPollEvents();
    }
  }
#endif

};
