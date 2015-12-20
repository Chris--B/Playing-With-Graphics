#pragma once

#include "Camera.hpp"
#include "gl_defs.hpp"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

class GameWorld {
public:
    GameWorld() { m_collision.init(); }
    ~GameWorld() { m_collision.deinit(); }

    void update(float dt);

    btDiscreteDynamicsWorld *world() { return m_collision.world; }

private:
    // ========== Physics Engine ==============================================
    btAlignedObjectArray<btCollisionShape *> collideables;

    // Instead of naming each member m_collision*, just make a namespace.
    // Except you can't do that to member variables, so make a struct intead.
    struct CollisionMembers {
        btBroadphaseInterface *pairCache  = nullptr;
        btCollisionConfiguration *config  = nullptr;
        btCollisionDispatcher *dispatcher = nullptr;
        btConstraintSolver *solver        = nullptr;

        // This is the object we'll reference most.
        btDiscreteDynamicsWorld *world = nullptr;

        void init();
        void deinit();
    };
    CollisionMembers m_collision;
};
