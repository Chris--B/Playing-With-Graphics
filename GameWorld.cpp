#include "GameWorld.hpp"

#include "DebugDrawer.hpp"
#include "Util.hpp"

void GameWorld::update(float dt) { m_collision.world->stepSimulation(dt); }

void GameWorld::CollisionMembers::init() {
    pairCache  = new btDbvtBroadphase();
    config     = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(config);
    solver     = new btSequentialImpulseConstraintSolver();
    world = new btDiscreteDynamicsWorld(dispatcher, pairCache, solver, config);

    auto debug = new GLDebugDrawer();
    debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    world->setDebugDrawer(debug);

    // Initialize the world with gravity
    world->setGravity(btVector3(0, -10, 0));

    // ... and some objects.

    // Sides of the box.
    for (size_t i = 0; i < 1; i += 1) {
        btCollisionShape *groundShape = new btBoxShape(btVector3(50, 10, 50));
        // collisionShapes.push_back(groundShape);

        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, -5, 0));

        float mass = 0.0f;

        btVector3 localInertia;

        // Using motionstate is optional; it provides interpolation capabilities
        // and only syncronizes 'active' objects
        auto *myMotionState = new btDefaultMotionState(groundTransform);
        auto rbInfo         = btRigidBody::btRigidBodyConstructionInfo(
            mass, myMotionState, groundShape, localInertia);

        auto *body = new btRigidBody(rbInfo);
        body->setRestitution(0.5);

        world->addRigidBody(body);
    }

    // Dynamic rigid body
    for (size_t i = 0; i < 50; i += 1) {
        auto colShape = new btBoxShape(btVector3(1, 1, 1));
        // auto colShape = new btSphereShape(1.0f);
        // collisionShapes.push_back(colShape);

        btTransform startTransform;
        startTransform.setIdentity();
        btVector3 origin;
        origin.setX(getRand(-20, 20));
        origin.setY(getRand(10, 20));
        origin.setZ(getRand(-20, 20));
        startTransform.setOrigin(origin);
        startTransform.setRotation(
            btQuaternion(getRand(0, 6), getRand(0, 6), getRand(0, 6)));

        float mass = 1.0f;
        btVector3 localInertia;
        colShape->calculateLocalInertia(mass, localInertia);

        // Same as before, with motionstate
        auto *myMotionState = new btDefaultMotionState(startTransform);
        auto rbInfo         = btRigidBody::btRigidBodyConstructionInfo(
            mass, myMotionState, colShape, localInertia);

        btRigidBody *body = new btRigidBody(rbInfo);
        body->setRestitution(0.3f);

        world->addRigidBody(body);
    }
}

void GameWorld::CollisionMembers::deinit() {
    // Remove the rigid bodies and delete them.
    for (int i = world->getNumCollisionObjects() - 1; i >= 0; i -= 1) {
        assert(i < world->getNumCollisionObjects());
        btCollisionObject *obj = world->getCollisionObjectArray()[i];
        btRigidBody *body      = btRigidBody::upcast(obj);

        if (body && body->getMotionState()) {
            delete body->getMotionState();
        }

        world->removeCollisionObject(obj);
        delete obj;
    }

    // Delete everything else too.
    delete world;
    delete solver;
    delete pairCache;
    delete dispatcher;
    delete config;
}
