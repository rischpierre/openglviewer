#include "EventManager.hpp"
#include "Gizmo.hpp"
#include "Mesh.hpp"
#include "Window.hpp"

static float last_cursor_pos_x;
static float last_cursor_pos_y;
static bool cursor_locked = false;

oglv::EventManager::EventManager(GLFWwindow *glfw_window, Window *window)
    : m_window(window), m_glfw_window(glfw_window) {
    glfwSetKeyCallback(m_glfw_window, _key_callback);
    glfwSetMouseButtonCallback(m_glfw_window, _mouse_button_callback);
    glfwSetCursorPosCallback(m_glfw_window, _cursor_pos_callback);

    glfwSetWindowUserPointer(glfw_window, window);
}

void oglv::EventManager::_mouse_button_callback(GLFWwindow *glfw_window,
                                                int button,
                                                int action,
                                                int mods) {

    auto *window = (Window *)glfwGetWindowUserPointer(glfw_window);
    // select an object
    if (window->get_mode() == SELECTION && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(glfw_window, &xpos, &ypos);
        GLint id = 0;
        glReadPixels(static_cast<int>(xpos),
                     window->get_scene_description_ptr()->screen_height - static_cast<int>(ypos) - 1,
                     1,
                     1,
                     GL_STENCIL_INDEX,
                     GL_INT,
                     &id);

        window->get_scene()->deselect_all();

        std::shared_ptr<Mesh> mesh = window->get_scene()->get_mesh(id);
        if (mesh)
            mesh->set_selected(true);
    }
    // lock cursor on gizmo
    else if (window->get_mode() == TRANSFORM && button == GLFW_MOUSE_BUTTON_LEFT &&
             action == GLFW_PRESS) {
        std::shared_ptr<Mesh> mesh = window->get_scene()->get_selected_mesh();
        std::shared_ptr<GizmoAxis> gizmo_axis =
            window->get_scene()->get_gizmo()->get_selected_gizmo_axis();
        if (mesh && gizmo_axis)
            cursor_locked = true;

    } else if (window->get_mode() == TRANSFORM && button == GLFW_MOUSE_BUTTON_LEFT &&
               action == GLFW_RELEASE)
        cursor_locked = false;
}

void oglv::EventManager::_key_callback(GLFWwindow *glfw_window,
                                       int key,
                                       int scancode,
                                       int action,
                                       int mods) {
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(glfw_window, true);
    } else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        auto *w = (Window *)glfwGetWindowUserPointer(glfw_window);
        w->set_mode(TRANSFORM);
        std::shared_ptr<Mesh> selected_mesh = w->get_scene()->get_selected_mesh();
        if (selected_mesh) {
            selected_mesh->set_ready_to_transform(true);
            w->get_scene()->set_gizmo_visibility(true);
        } else {
            w->get_scene()->unset_transform_all();
        }

    } else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        auto *window = (Window *)glfwGetWindowUserPointer(glfw_window);
        window->set_mode(SELECTION);
        window->get_scene()->set_gizmo_visibility(false);
        window->get_scene()->unset_transform_all();
    }
}

void oglv::EventManager::_cursor_pos_callback(GLFWwindow *glfw_window, double xpos, double ypos) {
    auto *window = (Window *)glfwGetWindowUserPointer(glfw_window);
    Scene *scene = window->get_scene();

    // orbit cam
    if (glfwGetKey(glfw_window, GLFW_KEY_LEFT_ALT) &&
        glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

        scene->get_camera()->add_yaw(last_cursor_pos_x - static_cast<float>(xpos));
        scene->get_camera()->add_pitch(last_cursor_pos_y - static_cast<float>(ypos));
    }
    // pan
    else if (glfwGetKey(glfw_window, GLFW_KEY_LEFT_ALT) &&
             glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        scene->get_camera()->pan(static_cast<float>(ypos) - last_cursor_pos_y,
                                 static_cast<float>(xpos) - last_cursor_pos_x);
    }
    // dolly
    else if (glfwGetKey(glfw_window, GLFW_KEY_LEFT_ALT) &&
             glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        scene->get_camera()->dolly(last_cursor_pos_y - static_cast<float>(ypos));
    }

    // transform gizmo
    else if (window->get_mode() == TRANSFORM) {
        std::shared_ptr<Mesh> mesh = scene->get_selected_mesh();
        if (mesh) {

            if (cursor_locked) {
                if (glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                    std::shared_ptr<GizmoAxis> g = scene->get_gizmo()->get_selected_gizmo_axis();
                    glm::vec3 direction = g->get_direction();

                    glm::vec3 up = window->get_scene()->get_camera()->get_up();
                    glm::vec3 left = window->get_scene()->get_camera()->get_left();
                    float dot_up = glm::dot(up, direction);
                    float dot_down = glm::dot(-up, direction);

                    float dot_left = glm::dot(left, direction);
                    float dot_right = glm::dot(-left, direction);

                    std::vector<float> t = {dot_up, dot_down, dot_left, dot_right};
                    int i = std::max_element(t.begin(), t.end()) - t.begin();

                    float delta_y = static_cast<float>(ypos - last_cursor_pos_y) * 0.01f;
                    float delta_x = static_cast<float>(xpos - last_cursor_pos_x) * 0.01f;

                    float delta = 0;
                    if (i == 0)
                        delta = -delta_y;
                    else if (i == 1)
                        delta = delta_y;
                    else if (i == 2)
                        delta = -delta_x;
                    else if (i == 3)
                        delta = delta_x;

                    mesh->translate(delta * direction);
                }

            } else {

                GLint id = 0;
                glReadPixels(static_cast<int>(xpos),
                             window->get_scene_description_ptr()->screen_height -
                                 static_cast<int>(ypos) - 1,
                             1,
                             1,
                             GL_STENCIL_INDEX,
                             GL_INT,
                             &id);
                std::shared_ptr<GizmoAxis> gizmo_axis = scene->get_gizmo_axis_from_id(id);
                scene->deselect_gizmo();
                if (gizmo_axis)
                    gizmo_axis->set_selected(true);
            }
        }
    }

    last_cursor_pos_x = static_cast<float>(xpos);
    last_cursor_pos_y = static_cast<float>(ypos);
}
