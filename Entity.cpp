#include "Entity.hpp"

void Entity::draw(const glm::mat4x4 &projection) const {
    glm::mat4x4 modelview;

    // ... something motionstate
    m_phys_body->getWorldTransform().getOpenGLMatrix(glm::value_ptr(modelview));

    assert(!btFuzzyZero(glm::length(modelview[0])));
    assert(!btFuzzyZero(glm::length(modelview[1])));
    assert(!btFuzzyZero(glm::length(modelview[2])));
    assert(!btFuzzyZero(glm::length(modelview[3])));

    this->getGraphics()->draw(modelview * projection);
}
