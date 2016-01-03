#include "Entity.hpp"

#include "CommonDefs.hpp"

void Entity::draw(const glm::mat4x4 &projection,
                  const glm::mat4x4 &view) const {
    glm::mat4x4 model;
    // ... something motionstate
    m_phys_body->getWorldTransform().getOpenGLMatrix(glm::value_ptr(model));

    this->getGraphics()->draw(projection, view, model);
}
