//
// Created by Michał Łaszczewski on 22/12/16.
//

#ifndef FGFX_TEXTURE_H
#define FGFX_TEXTURE_H


#include <string>
#include "config.h"

namespace fgfx {

  class Texture {
  public:
    std::string name;
    int texture;
    bool initialized;
    bool unloaded;
    int width;
    int height;
    GLint internalFormat;
    GLenum format;
    GLenum type;

    int framebuffer;
    GLint old_fbo;
    GLint old_viewport[4];

    Texture(std::string namep);
    Texture(std::string namep, GLuint tex);
    Texture(int widthp, int heightp, GLint internalFormatp, GLenum formatp, GLenum typep);
    Texture(int widthp, int heightp, GLint internalFormatp, GLenum formatp, GLenum typep, GLuint tex);
    void upload(int widthp, int heightp, GLint internalFormatp, GLenum formatp, GLenum typep, void* buffer);

    void resize(int widthp, int heightp);
    void beginRender();
    void endRender(bool clean = false);

  };

}


#endif //FGFX_TEXTURE_H
