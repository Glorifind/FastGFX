#include "Engine.h"
#include <emscripten.h>

namespace fgfx {

  Engine::Engine() {

    const char vShaderStr[] =
        "attribute vec4 vPosition;    \n"
            "void main()                  \n"
            "{                            \n"
            "   gl_Position = vPosition;  \n"
            "}                            \n";

    const char fShaderStr[] =
        "precision mediump float;\n"\
            "void main()                                  \n"
            "{                                            \n"
            "  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
            "}                                            \n";

    auto vertexShader = fgfx::loadShader ( GL_VERTEX_SHADER, vShaderStr );
    auto fragmentShader = fgfx::loadShader ( GL_FRAGMENT_SHADER, fShaderStr );

    spritesProgram = fgfx::createProgram (vertexShader, fragmentShader);

    spritesProgramPositionLocation=glGetAttribLocation(spritesProgram,"vPosition");
    glEnableVertexAttribArray(spritesProgramPositionLocation);

    SpriteLayer::initializeSpriteProgram();
    LineLayer::initializeLineProgram();
    PolygonLayer::initializePolygonProgram();

    currentTime=0;
  }

  Engine::~Engine() {

  }

  std::shared_ptr<Sprite> Engine::getSprite(std::string spriteName) {
    emscripten_log(EM_LOG_ERROR, "LOADING SPRITE!!! %s\n",spriteName.c_str());
    auto it = sprites.find(spriteName);
    if (it != sprites.end()) return it->second;
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

  int Engine::getEngineTime() {
    return currentTime;
  }
  int Engine::incEngineTime() {
    return currentTime++;
  }

};
