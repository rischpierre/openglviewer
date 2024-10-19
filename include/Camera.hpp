#ifndef OPENGLVIEWER_CAMERA_H
#define OPENGLVIEWER_CAMERA_H

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace oglv {
class Camera {
public:
    Camera(int screen_width, int screen_height);
    void update();

    glm::mat4 get_projection() const { return m_projection; };

    void add_yaw(float y);
    void add_pitch(float p);
    void pan(float vertical, float horizontal);
    void dolly(float d);
    glm::mat4 get_view();

private:
    glm::mat4 m_view = glm::mat4(0.f);
    glm::mat4 m_projection;
    glm::vec3 m_eye = glm::vec3(0);

    glm::vec3 m_target = glm::vec3(0); // target point on which the camera rotates

    float m_yaw = 0;   // rotate cam on the side side
    float m_pitch = 0; // rotate cam on the front

    glm::vec3 m_forward = glm::vec3(0);
    glm::vec3 m_left = glm::vec3(0);
    glm::vec3 m_up = glm::vec3(0);

    glm::quat m_orientation;
};
} // namespace oglv

#endif // OPENGLVIEWER_CAMERA_H
