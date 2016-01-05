#pragma once

#include "Camera.hpp"
#include "CommonDefs.hpp"
#include "Entity.hpp"

#include <vector>

class GameWorld {
public:
    GameWorld() { m_collision.init(); }
    ~GameWorld() { m_collision.deinit(); }

    void update(float dt);

    void draw(const glm::mat4x4 &projection, const glm::mat4x4 &view) const;

    btDiscreteDynamicsWorld *world() { return m_collision.world; }

    void addEntity(Entity *entity);

private:
    std::vector<Entity *> m_entities;

    // ========== Physics Engine ==============================================

    // Instead of naming each member m_collision*, just make a namespace.
    // Except you can't do that to member variables, so make a struct intead.
    struct CollisionMembers {
        btBroadphaseInterface *   pairCache  = nullptr;
        btCollisionConfiguration *config     = nullptr;
        btCollisionDispatcher *   dispatcher = nullptr;
        btConstraintSolver *      solver     = nullptr;

        // This is the object we'll reference most.
        btDiscreteDynamicsWorld *world = nullptr;

        void init();
        void deinit();
    };
    CollisionMembers m_collision;
};
