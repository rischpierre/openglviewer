#include "Scene.hpp"

#include <iostream>
#include <memory>
#include <utility>

#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "spdlog/spdlog.h"

#include "Camera.hpp"
#include "Gizmo.hpp"
#include "Grid.hpp"
#include "Mesh.hpp"
#include "SceneDescription.hpp"

oglv::Scene::Scene(SceneDescription s)
    : m_scene_description(std::move(s)), m_camera(s.screen_width, s.screen_height), m_grid(),
      m_light() {
    _create_light();
    _load_meshes();
    m_gizmo = Gizmo();
}
std::shared_ptr<oglv::Mesh> oglv::Scene::get_mesh(unsigned int id) {

    id -= 4; // id 0 -> 4 is for the background and the gizmo
    if (id >= 0 && id < m_meshes.size()) {
        return m_meshes[id];
    }
    return nullptr;
}
void oglv::Scene::unset_transform_all() const {
    for (const std::shared_ptr<Mesh> &m : m_meshes) {
        m->set_ready_to_transform(false);
    }
}
void oglv::Scene::deselect_all() const {
    for (const std::shared_ptr<Mesh> &m : m_meshes) {
        m->set_selected(false);
    }
}
std::shared_ptr<oglv::Mesh> oglv::Scene::get_selected_mesh() const {
    for (const auto &m : m_meshes) {
        if (m->is_selected())
            return m;
    }
    return nullptr;
}

void oglv::Scene::deselect_gizmo() const {
    m_gizmo.get_m_gizmo_arrow_x()->set_selected(false);
    m_gizmo.get_m_gizmo_arrow_y()->set_selected(false);
    m_gizmo.get_m_gizmo_arrow_z()->set_selected(false);
};
void oglv::Scene::_create_light() {
    m_light = Light();
    m_light.position = m_scene_description.light_position;
    m_light.intensity = m_scene_description.light_intensity;
}

void oglv::Scene::_load_meshes() {
    int i = 0;
    for (const std::string &obj_file : m_scene_description.obj_files) {

        Assimp::Importer importer;
        importer.SetPropertyFloat("PP_GSN_MAX_SMOOTHING_ANGLE", 45);

        const aiScene *assimp_scene = importer.ReadFile(
            obj_file,
            aiProcess_GenSmoothNormals | aiProcess_ForceGenNormals | aiProcess_Triangulate);

        if (!assimp_scene || assimp_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
            !assimp_scene->mRootNode) {
            spdlog::error("ASSIMP {}", importer.GetErrorString());
            exit(-1);
        }
        for (unsigned int m = 0; m < assimp_scene->mNumMeshes; m++) {
            aiMesh *mesh = assimp_scene->mMeshes[m];
            std::vector<float> vertices;
            std::vector<unsigned int> indices;

            for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
                vertices.push_back(mesh->mVertices[j].x);
                vertices.push_back(mesh->mVertices[j].y);
                vertices.push_back(mesh->mVertices[j].z);
                vertices.push_back(mesh->mNormals[j].x);
                vertices.push_back(mesh->mNormals[j].y);
                vertices.push_back(mesh->mNormals[j].z);
                vertices.push_back(mesh->mTextureCoords[0][j].x);
                vertices.push_back(mesh->mTextureCoords[0][j].y);
            }
            for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
                unsigned int *indices_ = mesh->mFaces[j].mIndices;

                // there is only triangles since triangulation is forced on load.
                indices.push_back(*(indices_));
                indices.push_back(*(indices_ + 1));
                indices.push_back(*(indices_ + 2));
            }
            const auto name = (std::string)mesh->mName.C_Str();
            std::shared_ptr<oglv::Mesh> mesh_ptr =
                std::make_shared<oglv::Mesh>(name,
                                             &vertices[0],
                                             vertices.size() * sizeof(float),
                                             &indices[0],
                                             indices.size() * sizeof(unsigned int),
                                             "../shaders/mesh.vert",
                                             "../shaders/mesh.frag",
                                             m_scene_description.textures[i]);
            m_meshes.emplace_back(mesh_ptr);
        }
        i++;
    }
}

void oglv::Scene::draw() {
    // background: 0, gizmo axis: 1-3, meshes: 4+
    int i = 4;
    for (const std::shared_ptr<oglv::Mesh> &m : m_meshes) {

        // draw the stencil buffer, the background is 0
        glStencilFunc(GL_ALWAYS, i, -1);
        m->draw(&m_camera, &m_light, &m_gizmo);

        i++;
    }
    m_grid.draw(&m_camera);

    if (m_gizmo.is_visible()) {
        glClear(GL_DEPTH_BUFFER_BIT);
        m_gizmo.draw(&m_camera);
    }
}
std::shared_ptr<oglv::GizmoAxis> oglv::Scene::get_gizmo_axis_from_id(GLint i) {
    switch (i) {
    case 1:
        return m_gizmo.get_m_gizmo_arrow_x();
    case 2:
        return m_gizmo.get_m_gizmo_arrow_y();
    case 3:
        return m_gizmo.get_m_gizmo_arrow_z();
    default:
        return nullptr;
    }
}
