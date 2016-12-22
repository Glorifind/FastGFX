//
// Created by Michał Łaszczewski on 09/12/16.
//

#ifndef FGFX_IMAGE_H
#define FGFX_IMAGE_H


#ifndef EMSCRIPTEN // implemented in javascript
#ifndef __ANDROID
#include <string>
#include <fstream>
#include <streambuf>
#endif

#include <png.h>
#include <fastgfx.h>
#include <memory.h>
#include <sstream>

namespace fgfx {

  std::string loadBuffer(std::string path);

  struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
  };
  class Image {
  public:
    int width;
    int height;
    Color* data;

    Image(int w, int h);
    ~Image();
    void clear();
    void put(std::shared_ptr<Image> that, int x, int y);
  };

  class PngError : public std::exception {
  };

  class ImageNotFoundError : public std::exception {
  };

  std::shared_ptr<Image> decodePngImage(std::string data);
}

#endif

#endif //FGFX_IMAGE_H
