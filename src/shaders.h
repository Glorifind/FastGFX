//
// Created by Michał Łaszczewski on 15/04/16.
//

#ifndef ENGINE_SHADERS_H
#define ENGINE_SHADERS_H

#include "config.h"

namespace fgfx {


  GLuint loadShader ( GLenum type, const char *shaderSrc);
  GLuint createProgram( GLuint vertexShader, GLuint fragmentShader);

}


#endif //ENGINE_SHADERS_H
