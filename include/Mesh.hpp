#ifndef OPENGLVIEWER_MESH_H
#define OPENGLVIEWER_MESH_H

#include "Camera.hpp"
#include "Gizmo.hpp"
#include "Light.hpp"
#include "Shader.hpp"

namespace oglv {
class Mesh : public std::enable_shared_from_this<Mesh> {

public:
    Mesh(const std::string &name,
         const float *vertices,
         long size_vertices,
         const unsigned int *indices,
         long size_indices,
         const std::string &vert_shader,
         const std::string &frag_shader);
    ~Mesh();
    std::shared_ptr<Mesh> get_ptr() { return shared_from_this(); }
    void set_selected(bool s) { m_selected = s; };
    bool is_selected() const { return m_selected; };
    void draw(Camera *camera, Light *light, Gizmo *gizmo);
    glm::mat4 *get_transform() { return &m_transform; }
    void set_transform(glm::mat4 m) { m_transform = m; }
    void translate(glm::vec3 t) { m_transform = glm::translate(m_transform, t); };
    void set_ready_to_transform(bool ready) { m_ready_to_transform = ready; }
    std::string get_name() const { return m_name; };

private:
    std::string m_name;
    bool m_selected = false;
    bool m_ready_to_transform = false;
    unsigned int m_vao, m_vbo, m_ebo;
    int m_num_indices;
    Shader m_shader;
    glm::mat4 m_transform = glm::mat4(1.f);
};
} // namespace oglv

#endif // OPENGLVIEWER_MESH_H
