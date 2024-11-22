#ifndef OPENGLVIEWER_GIZMO_H
#define OPENGLVIEWER_GIZMO_H

#include "Camera.hpp"
#include "Shader.hpp"
#include <memory>
#include <vector>

namespace oglv {

class Gizmo;
class Mesh;
class GizmoAxis {

public:
    GizmoAxis(const std::vector<float> &vertices,
              long size_vertices,
              const std::vector<unsigned int> &indices,
              long size_indices,
              const std::string &vert_shader,
              const std::string &frag_shader);
    ~GizmoAxis();
    void set_selected(bool s) { m_selected = s; };
    bool is_selected() { return m_selected; };
    void draw(Camera *camera);
    glm::mat4 *get_transform() { return &m_transform; }
    void set_transform(glm::mat4 m) { m_transform = m; };
    void set_color(glm::vec3 c) { m_color = c; };
    void set_direction(const glm::vec3& dir){m_direction = dir; };
    glm::vec3 get_direction() const {return m_direction;};

private:
    glm::vec3 m_direction;
    bool m_selected = false;
    unsigned int m_vao, m_vbo, m_ebo;
    int m_num_indices;
    Shader m_shader;
    glm::mat4 m_transform = glm::mat4(1.f);
    glm::vec3 m_color;

    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
};
class Gizmo {

public:
    Gizmo();
    void draw(Camera *camera);
    void apply_transform(glm::mat4 t) const;
    [[nodiscard]] bool is_visible() const { return m_visible; };
    void set_visible(bool v) { m_visible = v; };
    std::shared_ptr<GizmoAxis> get_m_gizmo_arrow_x() const { return m_gizmo_arrow_x; };
    std::shared_ptr<GizmoAxis> get_m_gizmo_arrow_y() const { return m_gizmo_arrow_y; };
    std::shared_ptr<GizmoAxis> get_m_gizmo_arrow_z() const { return m_gizmo_arrow_z; };
    void set_linked_mesh(std::shared_ptr<Mesh> m) { m_linked_mesh = m; };
    std::shared_ptr<Mesh> get_linked_mesh() const { return m_linked_mesh; };
    std::shared_ptr<GizmoAxis> get_selected_gizmo_axis() const;

private:
    bool m_visible = false;
    std::shared_ptr<GizmoAxis> m_gizmo_arrow_x;
    std::shared_ptr<GizmoAxis> m_gizmo_arrow_y;
    std::shared_ptr<GizmoAxis> m_gizmo_arrow_z;
    std::shared_ptr<Mesh> m_linked_mesh;
};
} // namespace oglv

#endif // OPENGLVIEWER_GIZMO_H
