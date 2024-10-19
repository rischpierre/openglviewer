#include "Mesh.hpp"
#include "Camera.hpp"
#include "Gizmo.hpp"
#include "Light.hpp"
#include "Shader.hpp"

#include <memory>

void oglv::Mesh::draw(oglv::Camera *camera, Light *light, oglv::Gizmo *gizmo) {

    m_shader.use();
    m_shader.set_uniform_mat4("view", camera->get_view());
    m_shader.set_uniform_mat4("projection", camera->get_projection());
    m_shader.set_uniform_mat4("model", m_transform);
    m_shader.set_uniform_3f("light_position", light->position);
    m_shader.set_uniform_3f("light_intensity", light->intensity);
    m_selected ? m_shader.set_uniform_bool("overlay", true)
               : m_shader.set_uniform_bool("overlay", false);

    glBindTexture(GL_TEXTURE_2D, m_shader.get_id_texture());
    glUseProgram(m_shader.get_program_id());
    glBindVertexArray(m_vao);

    glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    if (m_ready_to_transform) {
        gizmo->apply_transform(m_transform);
        gizmo->set_linked_mesh(get_ptr());
    }
}
oglv::Mesh::Mesh(const std::string &name,
                 const float *vertices,
                 long size_vertices,
                 const unsigned int *indices,
                 long size_indices,
                 const std::string &vert_shader,
                 const std::string &frag_shader)
    : m_shader(vert_shader, frag_shader), m_vao(0), m_vbo(0), m_ebo(0), m_name(name) {

    m_num_indices = (int)(size_indices / sizeof(unsigned int));

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao); // need to bind the Vertex array here so the future
                              // VBO will be connected to the VAO.
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, size_vertices, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_indices, indices, GL_STATIC_DRAW);
    // this is the pointer that will get the vertices positions or other
    // VERTEX_SHADER buffer attributes
    GLsizei stride = 8 * sizeof(float);
    // position
    glVertexAttribPointer(
        /*layout location*/ 0,
        /*size*/ 3,
        /*type*/ GL_FLOAT,
        /*normalize*/ GL_FALSE,
        /*stride*/ stride,
        /*pointer*/ (void *)nullptr);

    // normals
    glVertexAttribPointer(
        /*layout location*/ 1,
        /*size*/ 3,
        /*type*/ GL_FLOAT,
        /*normalize*/ GL_FALSE,
        /*stride*/ stride,
        /*pointer*/ (void *)(sizeof(float) * 3));

    // tex coords
    glVertexAttribPointer(
        /*layout location*/ 2,
        /*size*/ 2,
        /*type*/ GL_FLOAT,
        /*normalize*/ GL_FALSE,
        /*stride*/ stride,
        /*offset*/ (void *)(sizeof(float) * 6));
    glEnableVertexAttribArray(/*index (layout location*/ 0);
    glEnableVertexAttribArray(/*index (layout location*/ 1);
    glEnableVertexAttribArray(/*index (layout location*/ 2);
    glBindVertexArray(0);
}

oglv::Mesh::~Mesh() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}
