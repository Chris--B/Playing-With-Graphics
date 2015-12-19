#include "btBulletDynamicsCommon.h"

#include <functional>

class PhysicsWorld {
public:
    using ForEachFunc = std::function<void(int, btVector3)>;

    PhysicsWorld() = default;
    ~PhysicsWorld() { deinit(); }

    void init();
    void deinit();

    void dump();

    void update(float dt) {
        if (valid) {
            world->stepSimulation(dt, 10);
        }
    }

    void forEach(ForEachFunc func);

    void debugDrawWorld() { world->debugDrawWorld(); }

private:
    btAlignedObjectArray<btCollisionShape *> collisionShapes;

    btBroadphaseInterface *overlappingPairCache = nullptr;
    btDefaultCollisionConfiguration *config     = nullptr;
    btCollisionDispatcher *dispatcher           = nullptr;
    btSequentialImpulseConstraintSolver *solver = nullptr;
    btDiscreteDynamicsWorld *world              = nullptr;

    bool valid = false;
};
