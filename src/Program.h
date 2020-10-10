#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include <memory>
#include <string>

#include "Window.h"
#include "WindowCallback.h"
#include "ShaderMan.h"
#include "Context.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"

#include "sim/Planet.h"

class Program: public WindowCallback {
    typedef std::shared_ptr<Texture> tex_t;
    typedef std::shared_ptr<sim::Planet> planet_t;

    Window window;
    ShaderMan shaderMan;
    Context c;
    Camera camera;
    Mesh sphereMesh;
    Mesh cubeMesh;
    float lastMouseX;
    float lastMouseY;
    void processInput(float deltaTime);
    tex_t loadTex(const std::string &name);
    planet_t newPlanet(const tex_t &texture);
public:
    Program();
    void keyPressed(int key, int action, int mods) override;
    void mouseMoved(float mouseX, float mouseY) override;
    void run();
};

#endif
