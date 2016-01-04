#include "GameWorld.hpp"

#include "OpenGLDebugDrawer.hpp"
#include "ShaderUtils.hpp"

void GameWorld::update(float dt) { m_collision.world->stepSimulation(dt); }

void GameWorld::draw(const glm::mat4x4 &projection,
                     const glm::mat4x4 &view) const {
    auto           drawer   = m_collision.world->getDebugDrawer();
    GLDebugDrawer *glDrawer = dynamic_cast<GLDebugDrawer *>(drawer);
    assert(glDrawer != nullptr && "Are you using the GLDebugDrawer?");
    glDrawer->updateBuffers();
    glDrawer->updateUniforms(projection, view);

    for (auto *entity : m_entities) {
        entity->draw(projection, view);
    }
}

void GameWorld::addEntity(Entity *entity) {
    m_entities.push_back(entity);

    auto body = btRigidBody::upcast(entity->getPhysBody());
    assert(body != nullptr);
    m_collision.world->addRigidBody(body);

    // Keep the m_entities vector synced with our collision world.
    assert(m_entities.size() == m_collision.world->getNumCollisionObjects() &&
           "Are you adding to m_entities or m_collision.world without "
           "GameWorld::addEntity?");
}

// ========== Physics Engine ==================================================

void GameWorld::CollisionMembers::init() {
    pairCache  = new btDbvtBroadphase();
    config     = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(config);
    solver     = new btSequentialImpulseConstraintSolver();
    world = new btDiscreteDynamicsWorld(dispatcher, pairCache, solver, config);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    GLint shader = loadCompileAndLink("../glsl/bullet-debug/");
    auto  debug  = new GLDebugDrawer(vao, shader);
    world->setDebugDrawer(debug);

    // Initialize the world with gravity
    world->setGravity(btVector3(0, -9.81f, 0));
}

void GameWorld::CollisionMembers::deinit() {
    // Remove the rigid bodies and delete them.
    for (int i = world->getNumCollisionObjects() - 1; i >= 0; i -= 1) {
        assert(i < world->getNumCollisionObjects());
        btCollisionObject *obj  = world->getCollisionObjectArray()[i];
        btRigidBody *      body = btRigidBody::upcast(obj);

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
