#pragma once

#include "CommonDefs.hpp"

class Camera {
public:
    Camera(glm::vec3 eye,                  //
           glm::vec3 center = glm::vec3(), //
           glm::vec3 up     = glm::vec3(0, 1, 0))
        : m_eye(eye), m_forward(center - eye), m_up(up) {
        renormalize();
    }

    glm::mat4x4 lookAt() const {
        return glm::lookAt(m_eye, m_eye + m_forward, m_up);
    }

    glm::vec3 forward() const {
        assert(abs(glm::length(m_forward) - 1.0f) < 1e-4f);
        return m_forward;
    }
    glm::vec3 right() const { return glm::cross(forward(), up()); }
    glm::vec3 up() const {
        assert(abs(glm::length(m_up) - 1.0f) < 1e-4f);
        return m_up;
    }

    void moveBy(glm::vec3 delta) { m_eye += delta; }
    void moveTo(glm::vec3 pos) { m_eye = pos; }
    void rotate(float theta, float phi) {
        // TODO: Adjust up as well?
        m_forward = glm::rotate(m_forward, theta, up());
        m_forward = glm::rotate(m_forward, phi, right());
        renormalize();
    }

    glm::vec3 pos() const { return m_eye; }

private:
    // Life is easier if we can assume forward() and up() always return normal
    // vectors.
    void renormalize() {
        m_forward = glm::normalize(m_forward);
        m_up      = glm::normalize(m_up);
    }

    glm::vec3 m_eye;
    glm::vec3 m_forward;
    glm::vec3 m_up;
};
