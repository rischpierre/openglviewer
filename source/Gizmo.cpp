#include "Gizmo.hpp"

#include <cmath>
#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "spdlog/spdlog.h"

#include "Camera.hpp"
#include "Light.hpp"
#include "Shader.hpp"

void oglv::GizmoAxis::draw(oglv::Camera *camera) {

    m_shader.use();
    m_shader.set_uniform_mat4("view", camera->get_view());
    m_shader.set_uniform_mat4("projection", camera->get_projection());
    m_shader.set_uniform_mat4("model", m_transform);
    m_shader.set_uniform_3f("color", m_color);

    m_selected ? m_shader.set_uniform_bool("overlay", true)
               : m_shader.set_uniform_bool("overlay", false);

    glUseProgram(m_shader.get_program_id());
    glBindVertexArray(m_vao);

    glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

oglv::GizmoAxis::GizmoAxis(const std::vector<float> &vertices,
                           long size_vertices,
                           const std::vector<unsigned int> &indices,
                           long size_indices,
                           const std::string &vert_shader,
                           const std::string &frag_shader)
    : m_shader(vert_shader, frag_shader), m_vao(0), m_vbo(0), m_ebo(0), m_vertices(vertices),
      m_indices(indices) {

    m_num_indices = static_cast<int>(size_indices / sizeof(unsigned int));

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao); // need to bind the Vertex array here so the future
                              // VBO will be connected to the VAO.
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, size_vertices, &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_indices, &m_indices[0], GL_STATIC_DRAW);
    // this is the pointer that will get the vertices positions or other
    // VERTEX_SHADER buffer attributes
    GLsizei stride = 3 * sizeof(float);
    // position
    glVertexAttribPointer(
        /*layout location*/ 0,
        /*size*/ 3,
        /*type*/ GL_FLOAT,
        /*normalize*/ GL_FALSE,
        /*stride*/ stride,
        /*pointer*/ (void *)nullptr);
    glEnableVertexAttribArray(/*index (layout location*/ 0);
    glBindVertexArray(0);
}

oglv::GizmoAxis::~GizmoAxis() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}
void oglv::Gizmo::draw(oglv::Camera *camera) {

    glStencilFunc(GL_ALWAYS, 1, -1);
    m_gizmo_arrow_x->draw(camera);

    glStencilFunc(GL_ALWAYS, 2, -1);
    m_gizmo_arrow_y->draw(camera);

    glStencilFunc(GL_ALWAYS, 3, -1);
    m_gizmo_arrow_z->draw(camera);
}

void oglv::Gizmo::apply_transform(glm::mat4 t) const {

    glm::mat4 m_transform_x(1.f);
    m_transform_x = glm::rotate(m_transform_x, glm::radians(90.f), glm::vec3(0, 0, 1));
    m_gizmo_arrow_x->set_transform(t * m_transform_x);

    glm::mat4 m_transform_y(1.f);
    m_gizmo_arrow_y->set_transform(t);

    glm::mat4 m_transform_z(1.f);
    m_transform_z = glm::rotate(m_transform_z, glm::radians(90.f), glm::vec3(1, 0, 0));
    m_gizmo_arrow_z->set_transform(m_transform_z);
    m_gizmo_arrow_z->set_transform(t * m_transform_z);
}

oglv::Gizmo::Gizmo() {

    Assimp::Importer importer;
    importer.SetPropertyFloat("PP_GSN_MAX_SMOOTHING_ANGLE", 45);

    const aiScene *assimp_scene = importer.ReadFile(
        "../resources/arrow.obj",
        aiProcess_GenSmoothNormals | aiProcess_ForceGenNormals | aiProcess_Triangulate);

    if (!assimp_scene || assimp_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !assimp_scene->mRootNode) {
        spdlog::error("ASSIMP {}", importer.GetErrorString());
        exit(-1);
    }

    aiMesh *mesh = assimp_scene->mMeshes[0];

    if (mesh == nullptr) {
        spdlog::error("ASSIMP: unable to load mesh");
    }

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        unsigned int *indices_ = mesh->mFaces[i].mIndices;

        // there is only triangles
        indices.push_back(*(indices_));
        indices.push_back(*(indices_ + 1));
        indices.push_back(*(indices_ + 2));
    }

    m_gizmo_arrow_x = std::make_shared<GizmoAxis>(vertices,
                                                  vertices.size() * sizeof(float),
                                                  indices,
                                                  indices.size() * sizeof(unsigned int),
                                                  "../shaders/gizmo.vert",
                                                  "../shaders/gizmo.frag");
    m_gizmo_arrow_y = std::make_shared<GizmoAxis>(vertices,
                                                  vertices.size() * sizeof(float),
                                                  indices,
                                                  indices.size() * sizeof(unsigned int),
                                                  "../shaders/gizmo.vert",
                                                  "../shaders/gizmo.frag");
    m_gizmo_arrow_z = std::make_shared<GizmoAxis>(vertices,
                                                  vertices.size() * sizeof(float),
                                                  indices,
                                                  indices.size() * sizeof(unsigned int),
                                                  "../shaders/gizmo.vert",
                                                  "../shaders/gizmo.frag");

    m_gizmo_arrow_x->set_color(glm::vec3(0.6f, 0, 0));
    m_gizmo_arrow_y->set_color(glm::vec3(0, 0.6f, 0));
    m_gizmo_arrow_z->set_color(glm::vec3(0, 0, 0.6f));
    m_gizmo_arrow_x->set_direction(glm::vec3(1, 0, 0));
    m_gizmo_arrow_y->set_direction(glm::vec3(0, 1, 0));
    m_gizmo_arrow_z->set_direction(glm::vec3(0, 0, 1));
}
std::shared_ptr<oglv::GizmoAxis> oglv::Gizmo::get_selected_gizmo_axis() const {
    if (m_gizmo_arrow_x->is_selected()) return m_gizmo_arrow_x;
    if (m_gizmo_arrow_y->is_selected()) return m_gizmo_arrow_y;
    if (m_gizmo_arrow_z->is_selected()) return m_gizmo_arrow_z;
    return nullptr;
}
