#ifndef ENGINE_H
#define ENGINE_H

#include "../include/fastgfx.h"
#include <map>
#include <vector>
#include <string>
#include <memory>

#include "shaders.h"

namespace fgfx {

  using namespace fgfx;

  class Engine {
  protected:
    std::map<std::string,std::shared_ptr<Sprite>> sprites;
    std::vector<std::shared_ptr<Sprite>> spritesToLoad;
    friend class Sprite;

    std::map<std::string,std::shared_ptr<SpriteFont>> spriteFonts;

    friend class View;

    GLuint spritesProgram;
    GLuint spritesProgramPositionLocation;
  public:
    int currentTime;

    Engine();
    ~Engine();

    std::shared_ptr<Sprite> getSprite(std::string spriteName);
    std::shared_ptr<SpriteFont> getSpriteFont(std::string spriteFontName);

    int getSpritesToLoadCount();
    std::shared_ptr<Sprite> getSpriteToLoad(int n);
    void clearSpritesToLoad();

    int getEngineTime();
    int incEngineTime();

    void reloadSprite(std::shared_ptr<Sprite> spritep);
  };

};

#endif // ENGINE_H
