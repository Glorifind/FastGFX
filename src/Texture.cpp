//
// Created by Michał Łaszczewski on 22/12/16.
//

#include "Texture.h"

namespace fgfx {

  Texture::Texture(std::string namep) : name(namep) {
    texture = -1;
    framebuffer = -1;
    unloaded = false;
  };

  Texture::Texture(int widthp, int heightp, GLint internalFormatp, GLenum formatp, GLenum typep) {
    width = widthp;
    height = heightp;
    unloaded = false;
    framebuffer = -1;
    internalFormat = internalFormatp;
    format = formatp;
    type = typep;
    GLuint tex;
    glGenTextures(1, &tex);
    texture = tex;
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  void Texture::upload(int widthp, int heightp, GLint internalFormatp, GLenum formatp, GLenum typep, void* buffer) {
    width = widthp;
    height = heightp;
    internalFormat = internalFormatp;
    format = formatp;
    type = typep;
    unloaded = false;
    GLuint tex;
    glGenTextures(1, &tex);
    texture = tex;
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  void Texture::resize(int widthp, int heightp) {
    if(width != widthp || height != heightp) {
      width = widthp;
      height = heightp;
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, 0);
    }
  }
  void Texture::beginRender() {
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
    glGetIntegerv(GL_VIEWPORT, old_viewport);
    if(framebuffer == -1) {
      GLuint fbo;
      glGenFramebuffers(1, &fbo);
      framebuffer = fbo;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glViewport(0, 0, width, height);
  }
  void Texture::endRender(bool clean) {
    glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
    glViewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);
    if(clean) {
      GLuint fbo = framebuffer;
      glDeleteFramebuffers(1, &fbo);
      framebuffer = -1;
    }
  }

}
