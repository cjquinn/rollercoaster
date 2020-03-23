#ifndef WINDOW_H
#define WINDOW_H

#include "common.h"

class Window {
public:
    static Window& instance();

    bool init(int width, int height, char *title);

    GLFWwindow *glwfWindow();

private:
    Window();
    Window(const Window&);
    void operator=(const Window&);

    bool initGLEW();
    void initOpenGL();

    GLFWwindow *glwfWindow_;
};

#endif
