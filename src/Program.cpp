#include "Program.h"

#include "opengl.h"
#include "glfw.h"
#include "debug.h"

#include "Matrix.h"
#include "Mesh.h"

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
    c.setShaderMan(shaderMan);
    c.setCamera(camera);
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

    float v[12] = {
        -1, -1, 0,
        -1, 1, 0,
        1, 1, 0,
        1, -1, 0
    };

    unsigned int i[6] = {
        0, 1, 2,
        0, 2, 3
    };

    Mesh m;
    m.setVertices(12, v);
    m.setIndices(6, i);
    m.setParam(0, 3, 3, 0);

    Matrix testMat(0, 0, 0);
    testMat.xScale = 0.5;
    testMat.yScale = 0.5;

    window.setCursorMode(Window::CURSOR_DISABLED);
    window.getMousePos(lastMouseX, lastMouseY);
    window.setEventCallback(*this);

    float start = glfwGetTime();

    while (!window.shouldClose()) {
        glClearColor(0.1, 0.1, 0.1, 1);
        window.clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Reder scene
        c.getShaders().setBody(c);
        testMat.load(c);
        m.render();

        window.swapBuffers();

        window.pollEvents();

        processInput(glfwGetTime() - start);
        start = glfwGetTime();

        ASSERT_GL("main loop");
    }
}
