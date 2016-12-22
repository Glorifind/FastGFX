//
// Created by Michał Łaszczewski on 15/04/16.
//

#include "shaders.h"
#include <cstddef>
#include <cstdio>

namespace fgfx {

  GLuint loadShader(GLenum type, const char *shaderSrc) {
    GLuint shader;
    GLint compiled;

    //printf("Loading shader: \n%s\n",shaderSrc);

    // Create the shader object
    shader = glCreateShader(type);

    if (shader == 0)
      return 0;

    // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, NULL);

    // Compile the shader
    glCompileShader(shader);

    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
      GLint infoLen = 0;

      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

      if (infoLen > 1) {
        char *infoLog = new char[infoLen];

        glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
        printf("Error compiling shader:\n%s\n", infoLog);

        delete[] infoLog;
      }

      glDeleteShader(shader);
      return 0;
    }

    return shader;

  }

  GLuint createProgram( GLuint vertexShader, GLuint fragmentShader) {
    // Create the program object
    auto programObject = glCreateProgram ( );

    if ( programObject == 0 )
      return 0;

    glAttachShader ( programObject, vertexShader );
    glAttachShader ( programObject, fragmentShader );

    // Bind vPosition to attribute 0
    glBindAttribLocation ( programObject, 0, "vPosition" );

    // Link the program
    glLinkProgram ( programObject );

    GLint linked;

    // Check the link status
    glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

    if ( !linked )
    {
      GLint infoLen = 0;

      glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

      if ( infoLen > 1 )
      {
        char* infoLog = new char[infoLen];

        glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
        printf( "Error linking program:\n%s\n", infoLog );

        delete[] infoLog;
      }

      glDeleteProgram ( programObject );
      return GL_FALSE;
    }

    return programObject;
  }

}