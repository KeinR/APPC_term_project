#include "Cubemap.h"

#include "opengl.h"
#include "error.h"
#include "debug.h"
#include "Image.h"

Cubemap::Cubemap() {
    init();
}
Cubemap::~Cubemap() {
    deInit();
}

void Cubemap::deInit() {
    glDeleteTextures(1, &handle);
}
void Cubemap::init() {
    glGenTextures(1, &handle);
}

void Cubemap::load(const std::string &path) {

    bind();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    const char *names[6] = {
        "right",
        "left",
        "up",
        "down",
        "back",
        "front"
    };

    for (int i = 0; i < 6; i++) {
        Image img(path + "_" + names[i] + ".png");
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, img.getGLFormat(),
            img.getWidth(), img.getHeight(),
            0, img.getGLFormat(), GL_UNSIGNED_BYTE, img.getData()
        );
    }

    // glPixelStorei(GL_UNPACK_ROW_LENGTH, width);

    // ASSERT_GL("pl");
    // // int xOffset = 0;
    // for (int i = 0; i < 6; i++) {
    //     std::cout << "i = " << i << '\n';
    //     // glTexImage2D(
    //     //     GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
    //     //     0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
    //     // );
    //     glTexImage2D(
    //         GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
    //         0, format, size, size, 0, format, GL_UNSIGNED_BYTE, data + size * i * channels
    //     );
    //     ASSERT_GL("lo");
    //     // xOffset += stride;
    // }

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
void Cubemap::bind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
}
