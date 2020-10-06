#include "Program.h"

#include "opengl.h"

Window::params getWindowParams() {
    Window::params p;
    p.title = "Solar";
    p.width = 500;
    p.height = 500;
    p.resizable = false;
    return p;
}

Program::Program(): window(getWindowParams()) {
}
void Program::run() {

    glClearColor(0.1, 0.1, 0.1, 1);
    while (!window.shouldClose()) {
        window.clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        window.swapBuffers();

        // Render..

        window.pollEvents();
    }
}
