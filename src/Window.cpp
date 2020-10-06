#include "Window.h"

#include <map>

#include "opengl.h"
#include "glfw.h"
#include "error.h"
#include "WindowCallback.h"
#include "debug.h"

static int glfwBool(bool b);

static void errorCallback(int error, const char* description);
static GLFWwindow *init(const Window::params &p);
static void deInit();
static void initWindow(GLFWwindow *window);
static GLFWwindow *doCreateWindow(const Window::params &p);
static GLFWwindow *makeWindow(const Window::params &p);
static void destroyWindow(GLFWwindow *window);

static void mouseMove(GLFWwindow *window, double mouseX, double mouseY);
static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void mouseClick(GLFWwindow *window, int button, int action, int mods);
static void mouseScroll(GLFWwindow *window, double xOffset, double yOffset);
static void charCallback(GLFWwindow* window, unsigned int codepoint);

typedef std::map<GLFWwindow*, WindowCallback*> wincallbacks_t;
typedef wincallbacks_t::iterator wincallbacks_it_t;
wincallbacks_t callbacks;
int contextCount = 0;

Window::Window(const params &params): handle(makeWindow(params)) {
}
Window::~Window() {
    destroyWindow(handle);
}

void Window::setCursorMode(cursorMode mode) {
    int m;
    switch (mode) {
        case CURSOR_NORMAL:
            m = GLFW_CURSOR_NORMAL;
            break;
        case CURSOR_HIDDEN:
            m = GLFW_CURSOR_HIDDEN;
            break;
        case CURSOR_DISABLED:
            m = GLFW_CURSOR_DISABLED;
            break;
        default:
            return;
    }
    glfwSetInputMode(handle, GLFW_CURSOR, m);
}
void Window::toggleInputMode(inputMode mode, bool toggle) {
    int m;
    switch (mode) {
        case STICKY_KEYS:
            m = GLFW_STICKY_KEYS;
            break;
        case STICKY_MOUSE_BUTTONS:
            m = GLFW_STICKY_MOUSE_BUTTONS;
            break;
        case LOCK_KEYS_MODS:
            m = GLFW_LOCK_KEY_MODS;
            break;
        case RAW_MOUSE_MOTION:
            m = GLFW_RAW_MOUSE_MOTION;
            break;
        default:
            return;
    }
    int val = glfwBool(toggle);
    glfwSetInputMode(handle, m, val);
}

void Window::clearBuffers(int bitfield) {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}
void Window::swapBuffers() {
    glfwSwapBuffers(handle);
}
void Window::makeCurrent() {
    glfwMakeContextCurrent(handle);
}

void Window::setShouldClose(bool value) {
    glfwSetWindowShouldClose(handle, glfwBool(value));
}
bool Window::shouldClose() const {
    ASSERT(handle != nullptr);
    return glfwWindowShouldClose(handle);
}

bool Window::keyPressed(int key) const {
    return glfwGetKey(handle, key) == GLFW_PRESS;
}

void Window::setEventCallback(WindowCallback &c) {
    callbacks[handle] = &c;
}
void Window::removeEventCallback() {
    callbacks.erase(handle);
}
GLFWwindow *Window::getHandle() {
    return handle;
}

void Window::pollEvents() {
    glfwPollEvents();
}

static int glfwBool(bool b) {
    return b ? GLFW_TRUE : GLFW_FALSE;
}

void errorCallback(int error, const char* description) {
    serror(false) << "GLFW error [code=" << error <<  " ]: " << description;
}

GLFWwindow *init(const Window::params &p) {
    glfwSetErrorCallback(errorCallback);

    if (glfwInit() == GLFW_FALSE) {
        serror(true) << "Failed to init GLFW";
        return nullptr;
    }
    // A glfw window is required to properly initialize GLAD
    GLFWwindow *window = doCreateWindow(p);
    if (window == NULL) {
        deInit();
        contextCount = 0;
        serror(true) << "Failed to make GLFW window";
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        deInit();
        contextCount = 0;
        serror(true) << "Failed to initialize OpenGL loader (GLAD)";
        return nullptr;
    }
    contextCount = 1;
    return window;

    #undef FUNC_SIG
}

void deInit() {
    glfwTerminate();
}

void initWindow(GLFWwindow *window) {
    glfwSetCursorPosCallback(window, mouseMove);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseClick);
    glfwSetScrollCallback(window, mouseScroll);
    glfwSetCharCallback(window, charCallback);
}

GLFWwindow *doCreateWindow(const Window::params &p) {
    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, p.contextVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, p.contextVersionMinor);
    int prophile;

    using pfp = Window::params::prophileType;

    switch (p.prophile) {
        case pfp::CORE:
            prophile = GLFW_OPENGL_CORE_PROFILE;
            break;
        case pfp::COMPAT:
            prophile = GLFW_OPENGL_COMPAT_PROFILE;
            break;
        case pfp::ANY:
            prophile = GLFW_OPENGL_ANY_PROFILE;
            break;
        default:
            return nullptr;
    }
    glfwWindowHint(GLFW_OPENGL_PROFILE, prophile);

    glfwWindowHint(GLFW_SAMPLES, p.MSAASamples);

    #define GLFW_BOOL(x) (x == true ? GLFW_TRUE : GLFW_FALSE)

    glfwWindowHint(GLFW_RESIZABLE, GLFW_BOOL(p.resizable));
    glfwWindowHint(GLFW_VISIBLE, GLFW_BOOL(p.visible));
    glfwWindowHint(GLFW_DECORATED, GLFW_BOOL(p.decorated));
    glfwWindowHint(GLFW_FOCUSED, GLFW_BOOL(p.focused));
    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_BOOL(p.autoIconify));
    glfwWindowHint(GLFW_FLOATING, GLFW_BOOL(p.floating));
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_BOOL(p.maximized));
    glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_BOOL(p.centerCursor));
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_BOOL(p.framebufferTransparent));
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_BOOL(p.focusOnshow));
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_BOOL(p.scaleToMonitor));

    #undef GLFW_BOOL

    return glfwCreateWindow(p.width, p.height, p.title, NULL, NULL);
}

GLFWwindow *makeWindow(const Window::params &p) {
    GLFWwindow *window;
    if (contextCount <= 0) {
        window = init(p);
    } else {
        contextCount++;
        window = doCreateWindow(p);
        if (window == NULL) {
            serror(true) << "Failed to make GLFW window";
            return nullptr;
        }
    }
    initWindow(window);
    return window;
}

void destroyWindow(GLFWwindow *window) {
    contextCount--;
    glfwDestroyWindow(window);
    if (contextCount <= 0) {
        deInit();
        contextCount = 0;
    }
}

void mouseMove(GLFWwindow *window, double mouseX, double mouseY) {
    wincallbacks_it_t loc = callbacks.find(window);
    if (loc != callbacks.end()) {
        loc->second->mouseMoved(
            static_cast<float>(mouseX),
            static_cast<float>(mouseY)
        );
    }
}
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    wincallbacks_it_t loc = callbacks.find(window);
    if (loc != callbacks.end()) {
        loc->second->keyPressed(key, action, mods);
    }
}


void mouseClick(GLFWwindow *window, int button, int action, int mods) {
    wincallbacks_it_t loc = callbacks.find(window);
    if (loc != callbacks.end()) {
        loc->second->mouseClicked(button, action == GLFW_PRESS, mods);
    }
}
void mouseScroll(GLFWwindow *window, double xOffset, double yOffset) {
    wincallbacks_it_t loc = callbacks.find(window);
    if (loc != callbacks.end()) {
        loc->second->mouseScrolled(
            static_cast<float>(xOffset),
            static_cast<float>(yOffset)
        );
    }
}
void charCallback(GLFWwindow* window, unsigned int codepoint) {
    wincallbacks_it_t loc = callbacks.find(window);
    if (loc != callbacks.end()) {
        loc->second->charInput(codepoint);
    }
}

