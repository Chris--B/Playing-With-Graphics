#pragma once

#include "CommonDefs.hpp"
#include "GraphicsObject.hpp"

class btCollisionObject;

class Entity {
public:
    Entity() = delete;
    Entity(GraphicsObject *graphics, btCollisionObject *phys_body)
        : m_graphics(graphics), m_phys_body(phys_body) {}
    ~Entity() = default;

    void draw() const { getGraphics()->draw(); }

    // Whoever allocates is responsible for cleanup. i.e. Not Us.
    void setGraphics(GraphicsObject *graphics) { m_graphics = graphics; }
    void setPhysBody(btCollisionObject *phys_body) { m_phys_body = phys_body; }

    GraphicsObject *getGraphics() const { return m_graphics; }
    btCollisionObject *getPhysBody() const { return m_phys_body; }

public:
    // Multiple things might look the same.
    GraphicsObject *m_graphics = nullptr;
    // But nothing needs to share the same physics body.
    btCollisionObject *m_phys_body = nullptr;
};
