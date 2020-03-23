#include "window.h"

Window& Window::instance() {
    static Window instance;

    return instance;
}

Window::Window() {}

bool Window::init(int width, int height, char *title) {
    if (glwfWindow_) {
        return true;
    }

    if (!glfwInit()) {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glwfWindow_ = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!glwfWindow_) {
        glfwTerminate();

        return false;
    }

    glfwMakeContextCurrent(glwfWindow_);

    if (!gladLoadGL()) {
        glfwDestroyWindow(glwfWindow_);
        glfwTerminate();
         
        return false;
    }

    return true;
}

GLFWwindow *Window::glwfWindow() {
    return glwfWindow_;
}
