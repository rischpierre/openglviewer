#include "Window.hpp"

#include <cmath>

#include "spdlog/spdlog.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"

#include "Camera.hpp"
#include "EventManager.hpp"
#include "Gizmo.hpp"
#include "Scene.hpp"
#include "SceneDescription.hpp"

oglv::Window::Window(oglv::SceneDescription s) : m_scene_description(std::move(s)) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_scene_description.screen_width,
                                m_scene_description.screen_height,
                                "learn OpenGl",
                                nullptr,
                                nullptr);
    if (!m_window) {
        spdlog::error("creating window");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGL()) {
        spdlog::error("unable to load glad");
        exit(-1);
    }
}

void oglv::Window::run(oglv::Scene *scene) {
    m_scene = scene;

    // todo remove and use the scene description for transform
    std::shared_ptr<oglv::Mesh> mesh = m_scene->get_mesh(4);
    if (mesh) {
        auto m = glm::mat4(1.f);
        m = glm::translate(m, glm::vec3(3, 0, 0));
        mesh->set_transform(m);
    }

    glEnable(GL_DEPTH_TEST);

    auto event_manager = oglv::EventManager(m_window, this);
    long frame = 0;
    while (!glfwWindowShouldClose(m_window)) {

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClearStencil(0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        m_scene->draw();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
        frame++;
    }

    glfwTerminate();
}
