#ifndef SPRITELAYER_H
#define SPRITELAYER_H

#include "Sprite.h"
#include "SpriteBuffer.h"
#include "Layer.h"
#include <glm/glm.hpp>
#include <vector>
#include <map>

#include <string>

#include "shaders.h"

namespace fgfx {

  class Engine;

  class SpriteLayer : public Layer {
  protected:
    std::vector<SpriteBuffer*> buffers;
    SpriteBuffer* getBuffer(int textureId);

    friend class Engine;
    friend class SpriteBuffer;

    static GLuint spriteProgram;
    static GLuint spriteProgramAttribPosition;
    static GLuint spriteProgramAttribColor;
    static GLuint spriteProgramAttribCoord;
    static GLuint spriteProgramUniformCameraMatrix;
    static GLuint spriteProgramUniformSampler;

    static void initializeSpriteProgram();
    void beginDraw(glm::mat4 cameraMatrix);
    void endDraw();
  public:
    SpriteLayer(Engine* enginep, std::string namep);

    void bufferSprite(std::shared_ptr<Sprite>& sprite, glm::vec2 pos, glm::vec4 color, double size, double rotation);
    void bufferSprite(std::shared_ptr<Sprite>& sprite, const glm::mat4 &mat, glm::vec4 color);
    virtual void reset() override ;
    virtual void upload() override ;
    virtual void render(glm::mat4 cameraMatrix) override ;
  };

}
#endif // SPRITELAYER_H
