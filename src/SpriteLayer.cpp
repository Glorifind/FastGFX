#include "SpriteLayer.h"
#include "SpriteBuffer.h"
#include "Engine.h"

#include <glm/gtc/type_ptr.hpp>
#include "config.h"

namespace fgfx {

  SpriteLayer::SpriteLayer(Engine* enginep) : Layer(enginep) {
    composition=1;
  };

  SpriteBuffer* SpriteLayer::getBuffer(int textureId) {
    for(auto it=buffers.begin(); it!=buffers.end(); ++it) {
      if((*it)->textureId == textureId) return *it;
    }
    SpriteBuffer* buf = new SpriteBuffer(textureId);
    buffers.push_back(buf);
    return buf;
  }

  void SpriteLayer::bufferSprite(const std::shared_ptr<Sprite>& sprite, glm::vec2 pos, glm::vec4 color, float size, float rotation) {
    //emscripten_log(EM_LOG_ERROR,"BUFFER SPRITE (%04.4lf,%04.4lf) %04.4lf textureId=%d\n",pos.x,pos.y,size,sprite->textureId);
    //emscripten_log(EM_LOG_ERROR,"BUFFER SPRITE %s textureId=%d\n",sprite->name.c_str(),sprite->textureId);
    //sprite->lastUseTime = engine->currentTime;
    if(sprite->unloaded) engine->reloadSprite(sprite);
    if(sprite->textureId == -1) return;
    SpriteBuffer* buf = getBuffer(sprite->textureId);
    buf->bufferSprite(sprite, pos, color, size, rotation);
  }

  void SpriteLayer::bufferSprite(const std::shared_ptr<Sprite>& sprite, const glm::mat4 &mat, glm::vec4 color) {
    //emscripten_log(EM_LOG_ERROR,"BUFFER SPRITE %s textureId=%d\n",sprite->name.c_str(),sprite->textureId);
    //sprite->lastUseTime = engine->currentTime;
    if(sprite->unloaded) engine->reloadSprite(sprite);
    if(sprite->textureId == -1) return;
    SpriteBuffer* buf = getBuffer(sprite->textureId);
    buf->bufferSprite(sprite, mat, color);
  }

  void SpriteLayer::rawBufferSprite(const std::shared_ptr<Sprite>& sprite, const glm::mat4 &mat, glm::vec4 color) {
    //emscripten_log(EM_LOG_ERROR,"BUFFER SPRITE %s textureId=%d\n",sprite->name.c_str(),sprite->textureId);
    //sprite->lastUseTime = engine->currentTime;
    if(sprite->unloaded) engine->reloadSprite(sprite);
    if(sprite->textureId == -1) return;
    SpriteBuffer* buf = getBuffer(sprite->textureId);
    buf->rawBufferSprite(sprite, mat, color);
  }

  GLuint SpriteLayer::spriteProgram = 0;
  GLuint SpriteLayer::spriteProgramAttribColor = 0;
  GLuint SpriteLayer::spriteProgramAttribPosition = 0;
  GLuint SpriteLayer::spriteProgramAttribCoord = 0;
  GLuint SpriteLayer::spriteProgramUniformSampler = 0;
  GLuint SpriteLayer::spriteProgramUniformCameraMatrix = 0;

  void SpriteLayer::initializeSpriteProgram() {

    const char vShaderStr[] =
        "attribute vec3 aVertexPosition;\n"\
        "attribute vec4 aVertexColor;\n"
        "attribute vec2 aTextureCoord;\n"
        "uniform mat4 uCameraMatrix;\n"
        "varying vec4 vColor;\n"
        "varying highp vec2 vTextureCoord;\n"
        "void main() {\n"
        "  gl_Position =uCameraMatrix*vec4(aVertexPosition, 1.0);\n"
        "  vColor = aVertexColor;\n"
        "  vTextureCoord = aTextureCoord;\n"
        "}\n";

    const char fShaderStr[] =
        "precision mediump float;\n"\
        "varying vec4 vColor;\n"
        "uniform sampler2D uSampler;\n"
        "varying highp vec2 vTextureCoord;\n"
        "void main() {\n"
        "  vec4 textureColor = texture2D(uSampler, vTextureCoord);\n"
        "  gl_FragColor = vColor*textureColor;\n"
        "}\n";

    auto vertexShader = fgfx::loadShader ( GL_VERTEX_SHADER, vShaderStr );
    auto fragmentShader = fgfx::loadShader ( GL_FRAGMENT_SHADER, fShaderStr );

    auto spriteProgram = fgfx::createProgram (vertexShader, fragmentShader);
    if(!spriteProgram) throw "nie ma programu!";
    SpriteLayer::spriteProgram = spriteProgram;

    SpriteLayer::spriteProgramAttribPosition = glGetAttribLocation(spriteProgram,"aVertexPosition");
    SpriteLayer::spriteProgramAttribColor = glGetAttribLocation(spriteProgram,"aVertexColor");
    SpriteLayer::spriteProgramAttribCoord = glGetAttribLocation(spriteProgram,"aTextureCoord");

    SpriteLayer::spriteProgramUniformCameraMatrix = glGetUniformLocation(spriteProgram,"uCameraMatrix");
    SpriteLayer::spriteProgramUniformSampler = glGetUniformLocation(spriteProgram,"uSampler");
  }

  void SpriteLayer::beginDraw(glm::mat4 cameraMatrix) {
    glUseProgram(SpriteLayer::spriteProgram);
    glEnableVertexAttribArray(SpriteLayer::spriteProgramAttribPosition);
    glEnableVertexAttribArray(SpriteLayer::spriteProgramAttribColor);
    glEnableVertexAttribArray(SpriteLayer::spriteProgramAttribCoord);

    glUniformMatrix4fv(SpriteLayer::spriteProgramUniformCameraMatrix, 1, GL_FALSE, (GLfloat*)glm::value_ptr(cameraMatrix));
  }

  void SpriteLayer::endDraw() {
    glDisableVertexAttribArray(SpriteLayer::spriteProgramAttribPosition);
    glDisableVertexAttribArray(SpriteLayer::spriteProgramAttribColor);
    glDisableVertexAttribArray(SpriteLayer::spriteProgramAttribCoord);
  }

  void SpriteLayer::reset() {
    for(auto it = buffers.begin(); it!=buffers.end(); ++it) {
      SpriteBuffer *buffer = *it;
      buffer->reset();
    }
  }

  void SpriteLayer::upload() {
    if(!visible) return;
    for(auto it = buffers.begin(); it!=buffers.end(); ++it) {
      SpriteBuffer *buffer = *it;
      buffer->upload();
    }
  }

  void SpriteLayer::render(glm::mat4 cameraMatrix) {
    if(!visible) return;
    beginDraw(cameraMatrix);
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_CULL_FACE);
    for(auto it = buffers.begin(); it!=buffers.end(); ++it) {
      SpriteBuffer* buffer=*it;
      glBindTexture(GL_TEXTURE_2D, buffer->textureId);
      glUniform1i(SpriteLayer::spriteProgramUniformSampler, 0);
      buffer->render();
    }

    endDraw();
  }


}
