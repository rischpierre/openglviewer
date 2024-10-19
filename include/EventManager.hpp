#ifndef OPENGLVIEWER_EVENTMANAGER_HPP
#define OPENGLVIEWER_EVENTMANAGER_HPP

#include "Window.hpp"
#include <GLFW/glfw3.h>

namespace oglv {
class EventManager {
public:
    EventManager(GLFWwindow *glfw_window, Window *window);

private:
    GLFWwindow *m_glfw_window;
    Window *m_window;

    void static _mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    void static _key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void static _cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);
};
} // namespace oglv
#endif // OPENGLVIEWER_EVENTMANAGER_HPP
