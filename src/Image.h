#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <string>

class Image {
    unsigned char *data;
    int width;
    int height;
    int channels;
    void load(const std::string &path);
    void free();
    void steal(Image &other);
public:
    Image();
    Image(const std::string &path);
    Image(Image &&other);
    ~Image();
    Image &operator=(Image &&other);

    int getWidth();
    int getHeight();
    int getChannels();
    int getGLFormat();

    unsigned char *getData();
};

#endif
