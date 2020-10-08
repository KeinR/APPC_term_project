#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include "Window.h"
#include "WindowCallback.h"
#include "ShaderMan.h"
#include "Context.h"
#include "Camera.h"

class Program: public WindowCallback {
    Window window;
    ShaderMan shaderMan;
    Context c;
    Camera camera;
    float lastMouseX;
    float lastMouseY;
    void processInput(float deltaTime);
public:
    Program();
    void keyPressed(int key, int action, int mods) override;
    void mouseMoved(float mouseX, float mouseY) override;
    void run();
};

#endif
