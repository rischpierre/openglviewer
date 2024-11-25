#ifndef OPENGLVIEWER_SCENE_H
#define OPENGLVIEWER_SCENE_H

#include "Camera.hpp"
#include "Gizmo.hpp"
#include "Grid.hpp"
#include "Mesh.hpp"
#include "Light.hpp"
#include "SceneDescription.hpp"

namespace oglv {

/// Manages all the objects in the scene
class Scene {
public:
    explicit Scene(SceneDescription s);

    /// Draw all the objects in the scene
    void draw();

    Camera *get_camera() { return &m_camera; };
    Light *get_light() { return &m_light; };
    std::shared_ptr<Gizmo> get_gizmo() { return std::make_shared<Gizmo>(m_gizmo); };
    std::shared_ptr<GizmoAxis> get_gizmo_axis_from_id(GLint i);

    /// Get the mesh by its id. the ids from 0 to 4 are reserved for
    /// the gizmo axis and the background
    std::shared_ptr<Mesh> get_mesh(unsigned int id);

    /// Un set the transform mode on all the meshes in the scene
    void unset_transform_all() const;

    /// Deselect all meshes in the scene
    void deselect_all() const;

    /// Deselect all the gizmo axis
    void deselect_gizmo() const;
    std::shared_ptr<Mesh> get_selected_mesh() const;
    void set_gizmo_visibility(bool v) { m_gizmo.set_visible(v); };

private:
    /// Create the light object to give the illumination direction and intensity
    void _create_light();

    /// Load all the meshes from the scene description
    void _load_meshes();

    std::vector<std::shared_ptr<Mesh>> m_meshes;
    SceneDescription m_scene_description;
    Grid m_grid;
    Camera m_camera;
    Light m_light;
    Gizmo m_gizmo;
};
} // namespace oglv
#endif // OPENGLVIEWER_SCENE_H
