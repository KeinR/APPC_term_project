#ifndef WINDOW_CALLBACK_H_INCLUDED
#define WINDOW_CALLBACK_H_INCLUDED

class WindowCallback {
public:
    ~WindowCallback();
    virtual void keyPressed(int key, int action, int mods);
    virtual void mouseMoved(float mouseX, float mouseY);
    virtual void mouseClicked(int button, bool pressed, int mods);
    virtual void mouseScrolled(float xOffset, float yOffset);
    virtual void charInput(unsigned int codepoint);
};

#endif
