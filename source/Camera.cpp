#include "Camera.hpp"
#include "glm/gtx/quaternion.hpp"

#ifdef WIN32
#define _USE_MATH_DEFINES
#include "math.h"
#endif

oglv::Camera::Camera(int screen_width, int screen_height) {
    m_projection =
        glm::perspective(glm::radians(35.f),
                         static_cast<float>(screen_width) / static_cast<float>(screen_height),
                         1.f,
                         1000.f);
    update();
};

void oglv::Camera::add_yaw(float y) {
    // in order to keep m_yaw from -2pi to 2pi
    y *= 0.001f;
    m_yaw += y;
    while (m_yaw > M_PI * 2) {
        m_yaw -= M_PI * 2;
    }
    while (m_yaw < -M_PI * 2) {
        m_yaw += M_PI * 2;
    }
    update();
}

void oglv::Camera::add_pitch(float p) {
    p *= 0.001f;
    m_pitch += p;
    if (m_pitch < -M_PI_2)
        m_pitch = -M_PI_2;
    if (m_pitch > M_PI_2)
        m_pitch = M_PI_2;
    update();
}

void oglv::Camera::pan(float vertical, float horizontal) {
    m_target += m_left * horizontal * 0.003f;
    m_target += m_up * vertical * 0.003f;
    update();
}

void oglv::Camera::dolly(float d) {
    m_target += m_eye * d * 0.0005f;
    update();
}

glm::mat4 oglv::Camera::get_view() {
    // Compute inverse rotation q
    glm::quat q = m_orientation;
    q.x *= -1.0f;
    q.y *= -1.0f;
    q.z *= -1.0f;
    m_view = glm::mat4_cast(q);

    // Translate by inverse eyePosition.
    glm::vec3 v = -m_eye;
    glm::mat4 m = m_view;
    m_view[3] = (m[0] * v[0]) + (m[1] * v[1]) + (m[2] * v[2]) + m[3];

    return m_view;
}
void oglv::Camera::update() {

    glm::vec3 eye = glm::vec3(0, 0, 10);
    glm::quat rotation = glm::angleAxis(m_yaw, glm::vec3(0, 1, 0));
    rotation *= glm::angleAxis(m_pitch, glm::vec3(1, 0, 0));

    m_eye = rotation * eye + m_target;

    m_forward = glm::normalize(m_target - m_eye);
    m_left = glm::normalize(glm::cross(glm::vec3(0, 1, 0), m_forward));
    m_up = glm::cross(m_forward, m_left);

    glm::mat3 m;
    m[0] = -1.0f * m_left;    // first column, representing new x-axis orientation
    m[1] = m_up;              // second column, representing new y-axis orientation
    m[2] = -1.0f * m_forward; // third column, representing new z-axis orientation
    m_orientation = glm::quat_cast(m);
}