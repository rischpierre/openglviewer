#ifndef OPENGLVIEWER_EVENTMANAGER_HPP
#define OPENGLVIEWER_EVENTMANAGER_HPP

#include "Window.hpp"
#include <GLFW/glfw3.h>

namespace oglv {

/// Handles the user keyboard and mouse inputs
class EventManager {
public:
    EventManager(GLFWwindow *glfw_window, Window *window);

private:
    GLFWwindow *m_glfw_window;
    Window *m_window;

    /// Callbacks for the mouse for the object selection
    void static _mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

    /// Callbacks for the keyboard. Used mainly for the switch transform or selection mode
    void static _key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

    /// Callbacks for the cursor position on screen. Used for the camera controls and
    /// objects transform
    void static _cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);
};
} // namespace oglv
#endif // OPENGLVIEWER_EVENTMANAGER_HPP
