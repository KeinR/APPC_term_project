#include "Texture.h"

#include <stdexcept>
#include <iostream>
#include <stb/stb_image.h>

#include "opengl.h"

// In-use enum values for Texture.
// def'd to be extra safe.
#define TEXTURE_TYPE GL_TEXTURE_2D
#define TEXTURE_PIXEL_TYPE GL_UNSIGNED_BYTE

// Default parameters.
constexpr Texture::tparam Texture::defaultParams{
    GL_CLAMP_TO_BORDER,
    GL_CLAMP_TO_BORDER,
    GL_NEAREST,
    GL_NEAREST
};

Texture::Texture() {
    init(defaultParams);
}

Texture::Texture(const tparam &param) {
    init(param);
}

Texture::~Texture() {
    deInit();
}

void Texture::init(const tparam &params) {
    glGenTextures(1, &buffer);
    setParams(params);
}

void Texture::deInit() {
    glDeleteTextures(1, &buffer);
}

void Texture::steal(Texture &other) {
    buffer = other.buffer;
    other.buffer = 0;
}

Texture::Texture(Texture &&other) {
    steal(other);
}
Texture &Texture::operator=(Texture &&other) {
    steal(other);
    return *this;
}

unsigned int Texture::get() {
    return buffer;
}

void Texture::bind() const {
    glBindTexture(TEXTURE_TYPE, buffer);
}
void Texture::unbind() {
    glBindTexture(TEXTURE_TYPE, 0);
}

void Texture::loadImage(const std::string &imgPath, bool flipOnLoad) {
    stbi_set_flip_vertically_on_load(flipOnLoad);

    int width, height, channels;
    unsigned char *data = stbi_load(imgPath.c_str(), &width, &height, &channels, 0);
    if (data == NULL) {
        std::cerr << "Texture::loadImage(const std::string&): Failed to load image \"" << imgPath << "\"" << '\n';
        return;
    }
    GLenum format = glFormat(channels);
    if (format == GL_INVALID_ENUM) {
        stbi_image_free(data);
        std::cerr << "Texture::loadImage(const std::string&): Bad number of channels in \"" << imgPath << "\" (" << channels << ", only 1-4 supported" << '\n';
    }

    setData(format, width, height, data);

    stbi_image_free(data);
}

void Texture::setData(int format, unsigned int width, unsigned int height, const data_t *data) {
    bind();
    glTexImage2D(TEXTURE_TYPE, 0, format, width, height, 0, format, TEXTURE_PIXEL_TYPE, data);
    unbind();
}

void Texture::genMipmap() {
    bind();
    glGenerateMipmap(GL_TEXTURE_2D);
    unbind();
}

void Texture::setDefaultParams() {
    setParams(defaultParams);
}
void Texture::setParams(const tparam &params) {
    bind();

    // x/y wrap parameter
    glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_S, params.xWrap);
    glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_T, params.yWrap);

    // min/max filtering parameter
    glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, params.minFilter);
    glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MAG_FILTER, params.maxFilter);

    unbind();
}

unsigned int Texture::glFormat(int channels) {
    switch (channels) {
        case 4: return GL_RGBA;
        case 3: return GL_RGB;
        case 2: return GL_RG;
        case 1: return GL_RED;
        default: return GL_INVALID_ENUM;
    }
}
