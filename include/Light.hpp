#ifndef OPENGLVIEWER_LIGHT_H
#define OPENGLVIEWER_LIGHT_H
#include "glm/vec3.hpp"

namespace oglv {
struct Light {
    glm::vec3 position;
    glm::vec3 intensity;
};
} // namespace oglv

#endif // OPENGLVIEWER_LIGHT_H
