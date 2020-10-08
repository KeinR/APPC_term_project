#include "Program.h"

#include "opengl.h"
#include "glfw.h"
#include "debug.h"

#include "Matrix.h"
#include "Mesh.h"

#include "meshgen.h"

#include <memory>
#include "sim/Simulation.h"
#include "sim/Planet.h"

Window::params getWindowParams() {
    Window::params p;
    p.title = "Solar";
    p.width = 500;
    p.height = 500;
    p.resizable = false;
    return p;
}

Program::Program():
    window(getWindowParams()),
    shaderMan("data/shaders")
{
    sphereMesh = mg::sphere();
    c.setShaderMan(shaderMan);
    c.setCamera(camera);
    c.setSphereMesh(sphereMesh);
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
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_DEPTH_TEST);

    glDisable(GL_CULL_FACE);
    // glEnable(GL_DEPTH_TEST);

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

    std::shared_ptr<sim::Planet> p0 = std::make_shared<sim::Planet>();
    std::shared_ptr<sim::Planet> p1 = std::make_shared<sim::Planet>();
    sim.push(p0);
    glm::vec3 p(1000, 1000, 0);
    glm::vec3 v(-10, 0, 10);
    p1->setPosition(p);
    p1->setVelocity(v);
    sim.push(p1);

    c.setScale(1e-2);

    window.setCursorMode(Window::CURSOR_DISABLED);
    window.getMousePos(lastMouseX, lastMouseY);
    window.setEventCallback(*this);

    float start = glfwGetTime();

    while (!window.shouldClose()) {
        glClearColor(0.1, 0.1, 0.1, 1);
        window.clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Reder scene
        // testMat.load(c);
        // m.render();
        c.getShaders().setBody(c);
        sim.render(c);

        window.swapBuffers();

        window.pollEvents();

        processInput(glfwGetTime() - start);
        sim.run(glfwGetTime() - start);
        start = glfwGetTime();

        ASSERT_GL("main loop");
    }
}
