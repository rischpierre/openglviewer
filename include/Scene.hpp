#ifndef OPENGLVIEWER_SCENE_H
#define OPENGLVIEWER_SCENE_H

#include "Camera.hpp"
#include "Gizmo.hpp"
#include "Grid.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "SceneDescription.hpp"

namespace oglv {
class Scene {
public:
    explicit Scene(SceneDescription s);
    void load_meshes(const std::vector<std::string> &obj_files);
    void draw();

    void create_light() {
        // todo move light in scene description
        m_light = Light();
        m_light.position = glm::vec3(10, 10, 3);
        m_light.intensity = glm::vec3(0.5, 0.4, 0.3);
    }
    Camera *get_camera() { return &m_camera; };
    Light *get_light() { return &m_light; };
    std::shared_ptr<Gizmo> get_gizmo() { return std::make_shared<Gizmo>(m_gizmo); };

    std::shared_ptr<Mesh> get_mesh(unsigned int id) {

        id -= 4; // id 0 -> 4 is for the background and the gizmo
        if (id >= 0 && id < m_meshes.size()) {
            return m_meshes[id];
        }
        return nullptr;
    }
    void unset_transform_all() {
        for (const std::shared_ptr<Mesh> &m : m_meshes) {
            m->set_ready_to_transform(false);
        }
    }
    void deselect_all() {
        for (const std::shared_ptr<Mesh> &m : m_meshes) {
            m->set_selected(false);
        }
    }
    bool is_one_mesh_selected() {
        for (const auto &m : m_meshes) {
            if (m->is_selected())
                return true;
        }
        return false;
    }

    void set_gizmo_visibility(bool v) { m_gizmo.set_visible(v); }

    std::shared_ptr<GizmoAxis> get_gizmo_axis_from_id(GLint i);

    void deselect_gizmo() {
        m_gizmo.get_m_gizmo_arrow_x()->set_selected(false);
        m_gizmo.get_m_gizmo_arrow_y()->set_selected(false);
        m_gizmo.get_m_gizmo_arrow_z()->set_selected(false);
    };

private:
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    SceneDescription m_scene_desciption;
    Grid m_grid;
    Camera m_camera;
    Light m_light;
    Gizmo m_gizmo;
};
} // namespace oglv
#endif // OPENGLVIEWER_SCENE_H
