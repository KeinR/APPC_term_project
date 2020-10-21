#include "Program.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

#include "opengl.h"
#include "glfw.h"
#include "debug.h"

#include "Matrix.h"
#include "Mesh.h"

#include "meshgen.h"
#include "opengl.h"

#include <memory>
#include "sim/Simulation.h"
#include "sim/Planet.h"

#include "Texture.h"
#include "Cubemap.h"

Window::params getWindowParams() {
    Window::params p;
    p.title = "Solar";
    p.width = 500;
    p.height = 500;
    p.resizable = false;
    p.MSAASamples = 10;
    return p;
}

Program::Program():
    window(getWindowParams()),
    shaderMan("data/shaders")
{
    sphereMesh = mg::sphere();
    cubeMesh = mg::cube();

    camera.setRenderDist(1e10);
    camera.setSpeed(40);

    c.setShaderMan(shaderMan);
    c.setCamera(camera);
    c.setSphereMesh(sphereMesh);
}

Program::tex_t Program::loadTex(const std::string &name) {
    Texture::tparam texParams{
        GL_REPEAT,
        GL_REPEAT,
        GL_LINEAR_MIPMAP_LINEAR,
        GL_LINEAR
    };
    std::shared_ptr<Texture> tex = std::make_shared<Texture>(texParams);
    tex->loadImage("data/textures/" + name, false);
    tex->genMipmap();
    return tex;
}

Program::planet_t Program::newPlanet(const tex_t &texture, float scale) {
    return std::make_shared<sim::Planet>(texture, scale * 0.3);
}

void Program::initPlanet(const planet_t &p, double distance, double velocity, double incl) {
    p->setPosition(glm::dvec3(0, distance, 0));
    p->setVelocity(glm::dvec3(velocity * std::cos(glm::radians(incl)), 0, velocity * std::sin(glm::radians(incl))));
}

void Program::keyPressed(int key, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE) {
        window.setShouldClose(true);
    }
}
void Program::mouseMoved(float mouseX, float mouseY) {
    camera.processMouse(mouseX - lastMouseX, lastMouseY - mouseY);
    lastMouseX = mouseX;
    lastMouseY = mouseY;
}

void Program::processInput(float deltaTime) {
    if (window.keyPressed(GLFW_KEY_W)) {
        camera.processKey(Camera::dir::FORWARD, deltaTime);
    }
    if (window.keyPressed(GLFW_KEY_A)) {
        camera.processKey(Camera::dir::LEFT, deltaTime);
    }
    if (window.keyPressed(GLFW_KEY_S)) {
        camera.processKey(Camera::dir::BACKWARD, deltaTime);
    }
    if (window.keyPressed(GLFW_KEY_D)) {
        camera.processKey(Camera::dir::RIGHT, deltaTime);
    }
    if (window.keyPressed(GLFW_KEY_Q)) {
        camera.processKey(Camera::dir::DOWN, deltaTime);
    }
    if (window.keyPressed(GLFW_KEY_E)) {
        camera.processKey(Camera::dir::UP, deltaTime);
    }
}

void Program::run() {

    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    glDisable(GL_CULL_FACE);

    // float v[12] = {
    //     -1, -1, 0,
    //     -1, 1, 0,
    //     1, 1, 0,
    //     1, -1, 0
    // };

    // unsigned int i[6] = {
    //     0, 1, 2,
    //     0, 2, 3
    // };

    // Mesh m = mg::sphere();
    // m.setVertices(12, v);
    // m.setIndices(6, i);
    // m.setParam(0, 3, 3, 0);

    // Matrix testMat(0, 0, 0);
    // testMat.xScale = 0.5;
    // testMat.yScale = 0.5;

    sim::Simulation sim;


    tex_t sunTex = loadTex("2k_sun.jpg");
    planet_t sun = newPlanet(sunTex, 3);
    sun->setMass(1.989e30); // Wow that's MASSive
    sim.push(sun);

    tex_t mercuryTex = loadTex("2k_mercury.jpg");
    planet_t mercury = newPlanet(mercuryTex, 0.4);
    mercury->setMass(0.330e24);
    initPlanet(mercury, 57.9e9, 47.4e3, 7.0);
    sim.push(mercury);

    double f = 0.330e24 * 1.989e30;
    std::cout << "f = " << f << '\n';

    tex_t venusTex = loadTex("2k_venus_atmosphere.jpg");
    planet_t venus = newPlanet(venusTex, 1);
    venus->setMass(4.87e24);
    initPlanet(venus, 108.2e9, 35e3, 3.4);
    sim.push(venus);

    tex_t earthTex = loadTex("2k_earth_daymap.jpg");
    planet_t earth = newPlanet(earthTex, 1);
    earth->setMass(5.97e24);
    initPlanet(earth, 149.6e9, 29.8e3, 0.0);
    sim.push(earth);

    // tex_t moonTex = loadTex("2k_moon.jpg");
    // planet_t moon = newPlanet(moonTex, 0.3);
    // moon->setMass(0.073e24);
    // initPlanet(moon, 5.384e9 + 149.6e9, 1.0e3 + 29.8e3, 0.0);
    // sim.push(moon);

    // std::cout << (moon->getPosition() - earth->getPosition()).y << '\n';

    // glm::dvec3 p(10, 10, 0);
    // glm::dvec3 v(-0.03, 0, 0);
    // p1->setPosition(p);
    // p1->setVelocity(v);
    // sim.push(p1);

    // a dot b = 0

    Cubemap skybox;
    skybox.load("data/skybox/skybox");

    c.setScale(1e-10);
    // c.setScale(1);

    window.setCursorMode(Window::CURSOR_DISABLED);
    window.getMousePos(lastMouseX, lastMouseY);
    window.setEventCallback(*this);

    float start = glfwGetTime();

    float speed = 1e5;
    float iterations = 1e2;
    // float speed = 1;
    // float iterations = 1;

    while (!window.shouldClose()) {
        glClearColor(0.1, 0.1, 0.1, 1);
        window.clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Reder scene
        // testMat.load(c);
        // m.render();
        glDepthMask(GL_FALSE);
        c.getShaders().setSkybox(c);

        glm::mat4 model = c.getCamera().getProj() * glm::mat4(glm::mat3(c.getCamera().getView())) * glm::mat4(1.0f);
        glUniformMatrix4fv(c.getMatrixUniform(), 1, GL_FALSE, glm::value_ptr(model));

        skybox.bind();
        cubeMesh.render();

        glDepthMask(GL_TRUE);

        c.getShaders().setBody(c);
        sim.render(c);

        window.swapBuffers();

        window.pollEvents();

        processInput(glfwGetTime() - start);
        for (int i = 0; i < iterations; i++) {
            sim.run((glfwGetTime() - start) * speed);
        }
        start = glfwGetTime();

        ASSERT_GL("main loop");
    }
}
