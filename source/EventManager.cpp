#include "EventManager.hpp"
#include "Gizmo.hpp"
#include "Mesh.hpp"
#include "Window.hpp"

static float last_cursor_pos_x;
static float last_cursor_pos_y;

oglv::EventManager::EventManager(GLFWwindow *glfw_window, Window *window)
    : m_window(window), m_glfw_window(glfw_window) {
    glfwSetKeyCallback(m_glfw_window, _key_callback);
    glfwSetMouseButtonCallback(m_glfw_window, _mouse_button_callback);
    glfwSetCursorPosCallback(m_glfw_window, _cursor_pos_callback);

    glfwSetWindowUserPointer(glfw_window, window);
}

void oglv::EventManager::_mouse_button_callback(GLFWwindow *window,
                                                int button,
                                                int action,
                                                int mods) {

    auto *w = (Window *)glfwGetWindowUserPointer(window);
    // select an object
    if (w->get_mode() == SELECTION && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        GLint id = 0;
        glReadPixels(static_cast<int>(xpos),
                     w->get_scene_description_ptr()->screen_height - static_cast<int>(ypos) - 1,
                     1,
                     1,
                     GL_STENCIL_INDEX,
                     GL_INT,
                     &id);

        w->get_scene()->deselect_all();

        std::shared_ptr<Mesh> mesh = w->get_scene()->get_mesh(id);
        if (mesh)
            mesh->set_selected(true);
    }
}

void oglv::EventManager::_key_callback(GLFWwindow *window,
                                       int key,
                                       int scancode,
                                       int action,
                                       int mods) {
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    } else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        auto *w = (Window *)glfwGetWindowUserPointer(window);
        w->set_mode(TRANSFORM);
        if (w->get_scene()->is_one_mesh_selected()) {
            w->get_scene()->set_gizmo_visibility(true);
        }

    } else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        auto *w = (Window *)glfwGetWindowUserPointer(window);
        w->set_mode(SELECTION);
        w->get_scene()->set_gizmo_visibility(false);
    }
}

void oglv::EventManager::_cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) {
    // orbit cam
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) &&
        glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

        auto *w = (Window *)glfwGetWindowUserPointer(window);
        w->get_scene()->get_camera()->add_yaw(last_cursor_pos_x - static_cast<float>(xpos));
        w->get_scene()->get_camera()->add_pitch(last_cursor_pos_y - static_cast<float>(ypos));
    }
    // pan
    else if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) &&
             glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        auto *w = (Window *)glfwGetWindowUserPointer(window);
        w->get_scene()->get_camera()->pan(static_cast<float>(ypos) - last_cursor_pos_y,
                                          static_cast<float>(xpos) - last_cursor_pos_x);
    }
    // dolly
    else if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) &&
             glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        auto *w = (Window *)glfwGetWindowUserPointer(window);
        w->get_scene()->get_camera()->dolly(last_cursor_pos_y - static_cast<float>(ypos));
    }
    // transform
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        auto *w = (Window *)glfwGetWindowUserPointer(window);
        if (w->get_mode() == TRANSFORM && w->get_scene()->is_one_mesh_selected()) {
        }
    }

    last_cursor_pos_x = static_cast<float>(xpos);
    last_cursor_pos_y = static_cast<float>(ypos);
}
