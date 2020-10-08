#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include "nocopylazy.h"

struct GLFWwindow;
class WindowCallback;

class Window: public nocopylazy {
    GLFWwindow *handle;
public:
    struct params {
        const char *title = "";
        int width = 300;
        int height = 300;

        int MSAASamples = 0;

        int contextVersionMajor = 3;
        int contextVersionMinor = 3;

        enum prophileType {
            CORE,
            COMPAT,
            ANY
        };
        prophileType prophile = prophileType::CORE;

        bool resizable = true;
        bool visible = true;
        bool decorated = true;
        bool focused = false;
        bool autoIconify = false;
        bool floating = false;
        bool maximized = false;
        bool centerCursor = false;
        bool framebufferTransparent = false;
        bool focusOnshow = false;
        bool scaleToMonitor = false;
    };

    enum inputMode {
        STICKY_KEYS,
        STICKY_MOUSE_BUTTONS,
        LOCK_KEYS_MODS,
        RAW_MOUSE_MOTION
    };
    enum cursorMode {
        CURSOR_NORMAL,
        CURSOR_HIDDEN,
        CURSOR_DISABLED
    };

    Window(const params &params);
    ~Window();
    void setCursorMode(cursorMode mode);
    void toggleInputMode(inputMode mode, bool toggle);

    void clearBuffers(int bitfield);
    void swapBuffers();
    void makeCurrent();

    void setShouldClose(bool value);
    bool shouldClose() const;

    bool keyPressed(int key) const;

    float getMouseX() const;
    float getMouseY() const;
    void getMousePos(float &x, float &y) const;
    void setMousePos(float x, float y);

    void setEventCallback(WindowCallback &c);
    void removeEventCallback();
    static void pollEvents();
    GLFWwindow *getHandle();
};

#endif
