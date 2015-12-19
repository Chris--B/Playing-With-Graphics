#include "gl_defs.hpp"

class Camera {
public:
    Camera(glm::vec3 eye,                  //
           glm::vec3 center = glm::vec3(), //
           glm::vec3 up = glm::vec3(0, 1, 0))
        : m_eye(eye), m_facing(center - eye), m_up(up) {}

    glm::mat4x4 lookAt() const {
        return glm::lookAt(m_eye, m_eye + m_facing, m_up);
    }

    glm::vec3 forward() const { return m_facing; }
    glm::vec3 right() const { return glm::cross(forward(), up()); }
    glm::vec3 up() const { return m_up; }

    void moveBy(glm::vec3 delta) { m_eye += delta; }

    // These will be implemented as they are required. I leave declarations here
    // to remind me where I wanted to take this.

    glm::vec3 pos() const;

    void moveTo(glm::vec3 eye);

    void rotate(glm::vec3 eulers);

private:
    glm::vec3 m_eye;
    glm::vec3 m_facing;
    glm::vec3 m_up;
};
