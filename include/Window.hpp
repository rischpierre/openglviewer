#ifndef OPENGLVIEWER_WINDOW_H
#define OPENGLVIEWER_WINDOW_H

#include "Gizmo.hpp"
#include "Mesh.hpp"
#include "Scene.hpp"
#include "SceneDescription.hpp"
#include "glad/glad.h" // should be included before glfw3 because glfw needs glad
#include "spdlog/spdlog.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <utility>

namespace oglv {

enum editor_mode { TRANSFORM, SELECTION };

class Window {
public:
    explicit Window(SceneDescription s);
    void run(Scene *);

    Scene *get_scene() { return m_scene; };
    void set_mode(const enum editor_mode &m) {

        // todo make a map to print the name of the mode
        spdlog::info("switch mode to {}", int(m));
        m_editor_current_mode = m;
    };
    enum editor_mode get_mode() const { return m_editor_current_mode; };
    std::shared_ptr<SceneDescription> get_scene_description_ptr() {
        return std::make_shared<SceneDescription>(m_scene_description);
    };

private:
    std::shared_ptr<Mesh> m_mesh_selected;

    enum editor_mode m_editor_current_mode = SELECTION;

    GLFWwindow *m_window;
    SceneDescription m_scene_description;
    Scene *m_scene;
};
} // namespace oglv

#endif // OPENGLVIEWER_WINDOW_H
