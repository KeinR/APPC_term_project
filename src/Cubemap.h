#ifndef CUBEMAP_H_INCLUDED
#define CUBEMAP_H_INCLUDED

#include <string>

#include "nocopylazy.h"

class Cubemap: public nocopylazy {
    unsigned int handle;
    void deInit();
    void init();
public:
    Cubemap();
    ~Cubemap();
    void load(const std::string &path);
    void bind();
};

#endif
