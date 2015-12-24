#pragma once

#include "CommonDefs.hpp"

class Camera {
public:
    Camera(glm::vec3 eye,                  //
           glm::vec3 center = glm::vec3(), //
           glm::vec3 up = glm::vec3(0, 1, 0))
        : m_eye(eye), m_forward(center - eye), m_up(up) {}

    glm::mat4x4 lookAt() const {
        return glm::lookAt(m_eye, m_eye + m_forward, m_up);
    }

    glm::vec3 forward() const { return m_forward; }
    glm::vec3 right() const { return glm::cross(forward(), up()); }
    glm::vec3 up() const { return m_up; }

    void moveBy(glm::vec3 delta) { m_eye += delta; }
    void moveTo(glm::vec3 pos) { m_eye = pos; }
    void rotate(float theta, float phi) {
        m_forward = glm::rotate(forward(), theta, up());
        m_forward = glm::rotate(forward(), phi, right());
    }

    glm::vec3 pos() const { return m_eye; }

private:
    glm::vec3 m_eye;
    glm::vec3 m_forward;
    glm::vec3 m_up;
};
