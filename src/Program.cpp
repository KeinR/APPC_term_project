#include "Program.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    p.MSAASamples = 4;
    return p;
}

Program::Program():
    window(getWindowParams()),
    shaderMan("data/shaders")
{
    sphereMesh = mg::sphere();
    cubeMesh = mg::cube();

    camera.setRenderDist(1e10);
    camera.setSpeed(4);

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
    glEnable(GL_DEPTH_TEST);

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

    Texture::tparam texParams{
        GL_REPEAT,
        GL_REPEAT,
        GL_LINEAR_MIPMAP_LINEAR,
        GL_LINEAR
    };

    std::shared_ptr<Texture> earth = std::make_shared<Texture>(texParams);
    // std::shared_ptr<Texture> earth = std::make_shared<Texture>();
    earth->loadImage("earth2048.bmp", false);
    earth->genMipmap();
    std::shared_ptr<sim::Planet> p0 = std::make_shared<sim::Planet>(earth);
    std::shared_ptr<sim::Planet> p1 = std::make_shared<sim::Planet>(earth);
    p0->setMass(1e9);
    sim.push(p0);
    glm::vec3 p(10, 10, 0);
    // glm::vec3 v(-1, 0, 2);
    p1->setPosition(p);
    // p1->setVelocity(v);
    sim.push(p1);

    Cubemap skybox;
    skybox.load("h");

    // c.setScale(1e-2);
    c.setScale(1);

    window.setCursorMode(Window::CURSOR_DISABLED);
    window.getMousePos(lastMouseX, lastMouseY);
    window.setEventCallback(*this);

    float start = glfwGetTime();

    float speed = 12;

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
        sim.run((glfwGetTime() - start) * speed);
        start = glfwGetTime();

        ASSERT_GL("main loop");
    }
}
