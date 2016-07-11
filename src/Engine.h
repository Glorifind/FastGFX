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


namespace fgfx {

  class Engine : public std::enable_shared_from_this<Engine> {
  protected:
    std::map<std::string, std::shared_ptr<Sprite>> sprites;
    std::vector<std::shared_ptr<Sprite>> spritesToLoad;
    friend class Sprite;

    std::map<std::string, std::shared_ptr<SpriteFont>> spriteFonts;
    std::vector<std::shared_ptr<SpriteFont>> spriteFontsToLoad;
    friend class SpriteFont;

    std::function<void(double,float)> renderFunction;

  public:
    double renderTime;
    float renderDelta;

    int width;
    int height;

    Engine();
    ~Engine();

    std::shared_ptr<Sprite> getSprite(std::string spriteName);
    std::shared_ptr<SpriteFont> getSpriteFont(std::string spriteFontName);

    int getSpritesToLoadCount();
    std::shared_ptr<Sprite> getSpriteToLoad(int n);
    void clearSpritesToLoad();

    int getSpriteFontsToLoadCount();
    std::shared_ptr<SpriteFont> getSpriteFontToLoad(int n);
    void clearSpriteFontsToLoad();

    void reloadSprite(std::shared_ptr<Sprite> spritep);
    void setRenderFunction(std::function<void(float,float)> renderFunctionp);
    void render(double time, float delta, int widthp, int heightp);

    std::shared_ptr<SpriteLayer> createSpriteLayer();
    std::shared_ptr<PolygonLayer> createPolygonLayer();
    std::shared_ptr<LineLayer> createLineLayer();
  };

};

#endif // ENGINE_H
