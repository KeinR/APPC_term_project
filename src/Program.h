#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include "Window.h"
#include "WindowCallback.h"

class Program: public WindowCallback {
    Window window;
public:
    Program();
    void run();
};

#endif
