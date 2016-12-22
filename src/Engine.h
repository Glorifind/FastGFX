#ifndef ENGINE_H
#define ENGINE_H

#include "SpriteFont.h"
#include "Sprite.h"
#include "SpriteLayer.h"
#include "PolygonLayer.h"
#include "LineLayer.h"
#include "shaders.h"

#include <map>
#include <vector>
#include <string>
#include <memory>

#ifndef EMSCRIPTEN
#include "concurrentqueue.h"
#include "Image.h"
#endif

namespace fgfx {

  class Engine : public std::enable_shared_from_this<Engine> {
  protected:
    std::map<std::string, std::shared_ptr<Sprite>> sprites;
    std::map<std::string, std::shared_ptr<SpriteFont>> spriteFonts;

#ifdef EMSCRIPTEN
    std::vector<std::shared_ptr<Sprite>> spritesToLoad;
    std::vector<std::shared_ptr<SpriteFont>> spriteFontsToLoad;
#endif

#ifndef EMSCRIPTEN
  public:
    enum class AssetType {
      Sprite,
      SpriteFont
    };
    struct LoadTask {
      AssetType type;
      std::string path;
    };
    moodycamel::ConcurrentQueue<LoadTask> loadQueue;

    using UploadTask = std::function<void()>;
    moodycamel::ConcurrentQueue<UploadTask> uploadQueue;
  protected:
#endif

    friend class Sprite;
    friend class SpriteFont;

    std::function<void(double,float)> renderFunction;

  public:
    double renderTime;
    float renderDelta;

    int width;
    int height;

    double realWidth;
    double realHeight;

    Engine();
    ~Engine();

    std::shared_ptr<Sprite> getSprite(std::string spriteName);
    std::shared_ptr<SpriteFont> getSpriteFont(std::string spriteFontName);

#ifdef EMSCRIPTEN
    int getSpritesToLoadCount();
    std::shared_ptr<Sprite> getSpriteToLoad(int n);
    void clearSpritesToLoad();
    int getSpriteFontsToLoadCount();
    std::shared_ptr<SpriteFont> getSpriteFontToLoad(int n);
    void clearSpriteFontsToLoad();
#endif

    void reloadSprite(std::shared_ptr<Sprite> spritep);
    void setRenderFunction(std::function<void(float,float)> renderFunctionp);
    void render(double time, float delta, int widthp, int heightp);

    std::shared_ptr<SpriteLayer> createSpriteLayer();
    std::shared_ptr<PolygonLayer> createPolygonLayer();
    std::shared_ptr<LineLayer> createLineLayer();

#ifndef EMSCRIPTEN
    void mainLoop();
#endif
  };

};

#endif // ENGINE_H
