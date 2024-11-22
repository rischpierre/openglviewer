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
        std::shared_ptr<Mesh> selected_mesh = w->get_scene()->get_selected_mesh();
        if (selected_mesh) {
            selected_mesh->set_ready_to_transform(true);
            w->get_scene()->set_gizmo_visibility(true);
        } else {
            w->get_scene()->unset_transform_all();
        }

    } else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        auto *w = (Window *)glfwGetWindowUserPointer(window);
        w->set_mode(SELECTION);
        w->get_scene()->set_gizmo_visibility(false);
        w->get_scene()->unset_transform_all();
    }
}

void oglv::EventManager::_cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) {
    auto *win = (Window *)glfwGetWindowUserPointer(window);
    Scene* scene = win->get_scene();

    // orbit cam
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) &&
        glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

        scene->get_camera()->add_yaw(last_cursor_pos_x - static_cast<float>(xpos));
        scene->get_camera()->add_pitch(last_cursor_pos_y - static_cast<float>(ypos));
    }
    // pan
    else if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) &&
             glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        scene->get_camera()->pan(static_cast<float>(ypos) - last_cursor_pos_y,
                                          static_cast<float>(xpos) - last_cursor_pos_x);
    }
    // dolly
    else if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) &&
             glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        scene->get_camera()->dolly(last_cursor_pos_y - static_cast<float>(ypos));
    }

    // transform gizmo
    if (win->get_mode() == TRANSFORM){
        std::shared_ptr<Mesh> mesh = scene->get_selected_mesh();
        if (mesh) {

            GLint id = 0;
            glReadPixels(static_cast<int>(xpos),
                         win->get_scene_description_ptr()->screen_height - static_cast<int>(ypos) - 1,
                         1,
                         1,
                         GL_STENCIL_INDEX,
                         GL_INT,
                         &id);
            std::shared_ptr<GizmoAxis> gizmo_axis = scene->get_gizmo_axis_from_id(id);
            scene->deselect_gizmo();
            if (gizmo_axis) {
                gizmo_axis->set_selected(true);
                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                    // todo transform
                    std::cout << "transform\n";
                }
            }

        }
    }


    last_cursor_pos_x = static_cast<float>(xpos);
    last_cursor_pos_y = static_cast<float>(ypos);
}
