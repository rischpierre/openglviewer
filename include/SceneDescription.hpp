#ifndef OPENGLVIEWER_SCENEDESCRIPTION_H
#define OPENGLVIEWER_SCENEDESCRIPTION_H

#include <glm/ext/matrix_transform.hpp>
#include <string>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"

namespace oglv {
struct SceneDescription {
    std::vector<std::string> obj_files = {"../resources/monkey_high.obj", "../resources/cube.obj"};

    std::vector<glm::mat4> transforms = {glm::mat4(0.f),
                                         glm::translate(object_2_transform, glm::vec3(3, 0, 0))

    };
    int screen_width = 800;
    int screen_height = 600;
    glm::mat4 object_2_transform = glm::mat4(1.f);
};
} // namespace oglv
#endif // OPENGLVIEWER_SCENEDESCRIPTION_H
