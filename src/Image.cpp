#include "Image.h"

#include <stb/stb_image.h>

#include "error.h"
#include "opengl.h"

Image::Image(): data(nullptr), width(0), height(0), channels(0) {
}
Image::Image(const std::string &path): Image() {
    load(path);
}
Image::Image(Image &&other) {
    steal(other);
}
Image::~Image() {
    free();
}
Image &Image::operator=(Image &&other) {
    free();
    steal(other);
    return *this;
}

void Image::load(const std::string &path) {
    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (data == NULL) {
        serror() << "Failed to load image: \"" << path << "\"";
    }
}
void Image::free() {
    if (data != nullptr) {
        stbi_image_free(data);
        data = nullptr;
    }
}
void Image::steal(Image &other) {
    data = other.data;
    width = other.width;
    height = other.height;
    channels = other.channels;
    other.data = nullptr;
}

int Image::getWidth() {
    return width;
}
int Image::getHeight() {
    return height;
}
int Image::getChannels() {
    return channels;
}
int Image::getGLFormat() {
    switch (getChannels()) {
        case 1: return GL_RED;
        case 2: return GL_RG;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default: return 0;
    }
}

unsigned char *Image::getData() {
    return data;
}
