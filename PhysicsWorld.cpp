#include "PhysicsWorld.hpp"

#include "DebugDrawer.hpp"
#include "Util.hpp"

#include <iostream>

#include <cassert>

#include <iostream>

std::ostream &operator<<(std::ostream &os, const btVector3 &vec) {
    return os << vec.getX() << ", " << vec.getY() << ", " << vec.getZ();
}

void PhysicsWorld::init() {
    valid = true;

    overlappingPairCache = new btDbvtBroadphase();
    config               = new btDefaultCollisionConfiguration();
    dispatcher           = new btCollisionDispatcher(config);
    solver               = new btSequentialImpulseConstraintSolver();
    world                = new btDiscreteDynamicsWorld(
        dispatcher, overlappingPairCache, solver, config);

    // Setup wireframe debugging view.
    auto debugDrawer = new GLDebugDrawer();
    debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    // debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawAabb);
    world->setDebugDrawer(debugDrawer);

    // Initialize the world with gravity
    world->setGravity(btVector3(0, -10, 0));

    // ... and some objects.

    // Sides of the box.
    for (size_t i = 0; i < 1; i += 1) {
        btCollisionShape *groundShape = new btBoxShape(btVector3(50, 0, 50));
        collisionShapes.push_back(groundShape);

        btTransform groundTransform;
        groundTransform.setIdentity();

        float mass = 0.0f;

        btVector3 localInertia;

        // Using motionstate is optional; it provides interpolation capabilities
        // and only syncronizes 'active' objects
        auto *myMotionState = new btDefaultMotionState(groundTransform);
        auto rbInfo         = btRigidBody::btRigidBodyConstructionInfo(
            mass, myMotionState, groundShape, localInertia);

        auto *body = new btRigidBody(rbInfo);
        body->setRestitution(1.0);

        world->addRigidBody(body);
    }

    // Dynamic rigid body
    for (size_t i = 0; i < 50; i += 1) {
        auto colShape = new btBoxShape(btVector3(1, 1, 1));
        // auto colShape = new btSphereShape(1.0f);
        collisionShapes.push_back(colShape);

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

void PhysicsWorld::deinit() {
    if (!valid) {
        return;
    }
    valid = false;

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

    // Delete the collision shapes.
    for (int i = 0; i < collisionShapes.size(); i += 1) {
        delete collisionShapes[i];
        collisionShapes[i] = nullptr;
    }

    // Delete everything else too.
    delete world;
    delete solver;
    delete overlappingPairCache;
    delete dispatcher;
    delete config;
}

void PhysicsWorld::dump() {
    forEach([](int index, btVector3 o) {
        std::cout << "#" << index << ": " << o.getX() << ", " << o.getY()
                  << ", " << o.getZ() << ", "
                  << "\t\t";
    });
    std::cout << std::endl;
}

// TODO: I think we can do this with MotionStates.
void PhysicsWorld::forEach(ForEachFunc func) {
    if (!valid) {
        return;
    }
    // Print the location of each object.
    for (int i = 0; i < world->getNumCollisionObjects(); i += 1) {
        btCollisionObject *obj = world->getCollisionObjectArray()[i];
        btRigidBody *body      = btRigidBody::upcast(obj);

        btTransform trans;
        // Why?
        if (body && body->getMotionState()) {
            body->getMotionState()->getWorldTransform(trans);
        } else {
            trans = obj->getWorldTransform();
        }
        auto o = trans.getOrigin();
        func(i, o);
    }
}
