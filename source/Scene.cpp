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
    : m_scene_desciption(std::move(s)), m_camera(s.screen_width, s.screen_height), m_grid(),
      m_light() {
    create_light();
    load_meshes(m_scene_desciption.obj_files);
    m_gizmo = oglv::Gizmo();
}

void oglv::Scene::load_meshes(const std::vector<std::string> &obj_files) {

    for (const std::string &obj_file : obj_files) {

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
        for (uint m = 0; m < assimp_scene->mNumMeshes; m++) {
            aiMesh *mesh = assimp_scene->mMeshes[m];
            std::vector<float> vertices;
            std::vector<uint> indices;

            for (uint i = 0; i < mesh->mNumVertices; i++) {
                vertices.push_back(mesh->mVertices[i].x);
                vertices.push_back(mesh->mVertices[i].y);
                vertices.push_back(mesh->mVertices[i].z);
                vertices.push_back(mesh->mNormals[i].x);
                vertices.push_back(mesh->mNormals[i].y);
                vertices.push_back(mesh->mNormals[i].z);

                // todo get the UV
                vertices.push_back(0.f);
                vertices.push_back(0.f);
            }
            for (uint i = 0; i < mesh->mNumFaces; i++) {
                uint *indices_ = mesh->mFaces[i].mIndices;

                // there is only triangles
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
                                             indices.size() * sizeof(uint),
                                             "../shaders/mesh.vert",
                                             "../shaders/mesh.frag");
            m_meshes.emplace_back(mesh_ptr);
        }
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
