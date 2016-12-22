//
// Created by Michał Łaszczewski on 09/12/16.
//


#ifndef EMSCRIPTEN // implemented in javascript

#include "fastgfx.h"
#include "Image.h"

namespace fgfx {
  extern bool finished;


  std::string loadBuffer(std::string path) {
    fgfx_log("READING FILE %s", path.c_str());
#ifdef __ANDROID
    AAsset* asset = AAssetManager_open(fgfx::assetManager, path.c_str(), AASSET_MODE_UNKNOWN);
    if (NULL == asset) {
        __android_log_print(ANDROID_LOG_ERROR, "FastGFX", "Image not found %s", path.c_str());
        throw ImageNotFoundError();
    }
    long size = AAsset_getLength(asset);
    std::string buffer = std::string(size, 0);
    AAsset_read (asset, (void*)buffer.data(), size);
    AAsset_close(asset);
    return buffer;
#else
    path = "assets/" + path;
    std::ifstream t(path);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    return str;
#endif
  }

  void ReadDataFromInputStream(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if(io_ptr == NULL)
      throw PngError();
    std::istream& stream = *(std::istream*)io_ptr;
    stream.read((char*)outBytes, byteCountToRead);
  }
  void parseRGB(std::shared_ptr<Image> image, const png_structp& png_ptr, const png_infop& info_ptr) {
    int w = image->width;
    int h = image->height;
    Color* data = image->data;

    const png_uint_32 bytesPerRow = png_get_rowbytes(png_ptr, info_ptr);
    png_bytep src_row = new png_byte[bytesPerRow];
    for(int y = 0; y < h; y++) {
      png_read_row(png_ptr, src_row, NULL);
      Color* dest_row = data + w * y;
      for(int x = 0; x < w; x++) {
        memcpy(dest_row + x, src_row + (x * 3), 3);
        dest_row[x].a = 255;
      }
    }
  }
  void parseRGBA(std::shared_ptr<Image> image, const png_structp& png_ptr, const png_infop& info_ptr) {
    int w = image->width;
    int h = image->height;
    Color* data = image->data;
    const png_uint_32 bytesPerRow = png_get_rowbytes(png_ptr, info_ptr);
    png_bytep src_row = new png_byte[bytesPerRow];
    for(int y = 0; y < h; y++) {
      png_read_row(png_ptr, src_row, NULL);
      Color* dest_row = data + w * y;
      memcpy(dest_row, src_row, 4 * w);
    }
  }
  static void png_error_handler(png_structp png_ptr, png_const_charp msg) {
    fprintf(stderr, "readpng2 libpng error: %s\n", msg);
    fflush(stderr);
  }

  std::shared_ptr<Image> decodePngImage(std::string data) {
    std::istringstream stream(data);
    unsigned int sig_read = 0;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, png_error_handler, NULL);
    if(png_ptr == NULL) throw PngError();
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(info_ptr == NULL) {
      png_destroy_read_struct(&png_ptr, NULL, NULL);
      throw PngError();
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      throw PngError();
    }
    png_set_read_fn(png_ptr, &stream, ReadDataFromInputStream);
    png_set_sig_bytes(png_ptr, sig_read);
    png_read_info(png_ptr, info_ptr);
    png_uint_32 width = 0;
    png_uint_32 height = 0;
    int bitDepth = 0;
    int colorType = -1;
    png_uint_32 retval = png_get_IHDR(png_ptr, info_ptr, &width, &height, &bitDepth, &colorType, NULL, NULL, NULL);
    if(retval != 1) throw new PngError();

    std::shared_ptr<Image> image = std::make_shared<Image>(width, height);
    switch(colorType)
    {
      case PNG_COLOR_TYPE_RGB:
        parseRGB(image, png_ptr, info_ptr);
        break;
      case PNG_COLOR_TYPE_RGB_ALPHA:
        parseRGBA(image, png_ptr, info_ptr);
        break;
      default:
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        throw new PngError();
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return image;
  }

  Image::Image(int w, int h) {
    width = w;
    height = h;
    data = new Color[w*h];
  }
  Image::~Image() {
    delete[] data;
  }
  void Image::clear() {
    memset(data, 0,  width*height*sizeof(Color));
  }
  void Image::put(std::shared_ptr<Image> src, int x, int y) {
    int sw = src->width;
    int sh = src->height;
    Color* src_data = src->data;
    Color* dest_data = data;
    int dw = width;
    //int dh = height;
    for (int sy = 0; sy < sh; sy++) {
      for (int sx = 0; sx < sw; sx++) {
        int dx = x + sx;
        int dy = y + sy;
        dest_data[dy * dw + dx] = src_data[sy * sw + sx];
      }
    }
  }
}

#endif