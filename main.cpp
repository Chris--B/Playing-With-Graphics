#include "Camera.hpp"
#include "CommonDefs.hpp"
#include "GameWorld.hpp"

#include <memory>

std::unique_ptr<GameWorld> game;
btSphereShape ballShape = btSphereShape(0.2f);
btBoxShape boxShape = btBoxShape(btVector3(1, 1, 1));

GraphicsObject *cubeModel = nullptr;
GraphicsObject *model     = nullptr;

Camera camera(glm::vec3(20.0f, 10.0f, 20.0f));

glm::ivec2 mouse;
glm::ivec2 window = glm::vec2(1024, 1024);

bool keysPressed[256] = {};

bool needsReset = false;

// Apply WASD + QE commands to an object. Space for extra speed.
// A DirectionalObject must have the following methods:
//      void moveBy(glm::vec3);
//      glm::vec3 forward(); // W and S
//      glm::vec3 right();   // A and D
//      glm::vec3 up();      // Q and E
// 'forward()', 'right()', and 'up()' should be normalized.
template <typename DirectionalObject>
void moveFromWASDQE(DirectionalObject &obj, float speed, float dt);

btRigidBody *makeBall(float mass, const btVector3 &pos) {
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(pos);

    btVector3 inertia;
    ballShape.calculateLocalInertia(mass, inertia);

    auto *motionState = new btDefaultMotionState(transform);
    auto rbInfo       = btRigidBody::btRigidBodyConstructionInfo(
        mass, motionState, &ballShape, inertia);

    auto *body = new btRigidBody(rbInfo);
    assert(body != nullptr);
    body->setRestitution(0.3f);
    body->setFriction(1.0f);
    body->setRollingFriction(1.0f);
    return body;
}

void initScene() {
    game = std::make_unique<GameWorld>();

    glEnable(GL_LIGHT0);

    cubeModel = new GraphicsObject();
    cubeModel->loadObjFile("../tinyobjloader/cube.obj");

    model = new GraphicsObject();
    model->loadObjFile("../OBJ/lost_empire/lost_empire.obj");
    // model->loadObjFile("../OBJ/rungholt/rungholt.obj");

    // Add a ground platform
    {
        std::cout << "Loading mesh for collision." << std::endl;
        // Load the mesh into an index vertex array.
        auto *triVertexArray = new btTriangleIndexVertexArray();

        for (const auto &shape : model->shapes) {
            btIndexedMesh submesh;

            // Triangle indices
            submesh.m_numTriangles = as<int>(shape.mesh.indices.size()) / 3;
            submesh.m_triangleIndexBase
                = (unsigned char *)shape.mesh.indices.data();
            submesh.m_triangleIndexStride = 3 * sizeof(shape.mesh.indices[0]);

            // Vertex coordinates
            submesh.m_numVertices = as<int>(shape.mesh.positions.size()) / 3;
            submesh.m_vertexBase = (unsigned char *)shape.mesh.positions.data();
            submesh.m_vertexStride = 3 * sizeof(shape.mesh.positions[0]);

            triVertexArray->addIndexedMesh(submesh);
        }
        std::cout << "Done." << std::endl;

        // auto *groundShape = new btBoxShape(btVector3(200, 1, 200));
        auto *groundShape = new btBvhTriangleMeshShape(triVertexArray, true);
        groundShape->setMargin(0.1f);

        auto *motionState = new btDefaultMotionState();
        auto rbInfo       = btRigidBody::btRigidBodyConstructionInfo(
            0.0f, motionState, groundShape);

        btCollisionObject *body = new btRigidBody(rbInfo);
        body->setRestitution(0.1f);
        body->setFriction(10.0f);

        // Rendering this is really, really expensive.
        int flags = body->getCollisionFlags();
        body->setCollisionFlags(
            flags | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

        Entity *ground = new Entity(model, body);

        game->addEntity(ground);
    }

    // Add some cubes
    for (size_t i = 0; i < 10; i += 1) {
        btTransform transform;
        transform.setIdentity();

        btVector3 pos;
        pos.setX(getRand(-20, 20));
        pos.setY(getRand(25, 50));
        pos.setZ(getRand(-20, 20));
        transform.setOrigin(pos);

        constexpr float PI = 3.14159f;
        float yaw          = getRand(-PI, PI);
        float pitch        = getRand(-PI, PI);
        float roll         = getRand(-PI, PI);
        transform.setRotation(btQuaternion(yaw, pitch, roll));

        const float mass = 1.0f;
        btVector3 inertia;
        ballShape.calculateLocalInertia(mass, inertia);

        auto *motionState = new btDefaultMotionState(transform);
        auto rbInfo       = btRigidBody::btRigidBodyConstructionInfo(
            mass, motionState, &boxShape, inertia);

        auto *body = new btRigidBody(rbInfo);
        body->getWorldTransform().setRotation(
            btQuaternion(getRand(0, 6), getRand(0, 6), getRand(0, 6)));
        body->setRestitution(0.3f);

        Entity *cube = new Entity(cubeModel, body);

        game->addEntity(cube);
    }
}

void update(int) {
    float dt = 1.0f / 60;
    glutTimerFunc(1000 / 60, update, 0);

    if (needsReset) {
        needsReset = false;
        game       = std::make_unique<GameWorld>();
    }

    moveFromWASDQE(camera, 15.0f, dt);

    game->update(dt);

    glutPostRedisplay();
}

void render() {
    glEnable(GL_LIGHTING);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4x4 projection = camera.lookAt();
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(projection));

    // Directional lighting
    float light_dir[4] = {1.0f, 2.0f, 1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_dir);

    game->draw(projection);

    auto *world = game->world();
    world->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    world->debugDrawWorld();

    // world->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawAabb);
    // world->debugDrawWorld();

    glFlush();
}

void resize(int width, int height) {
    window = glm::ivec2(width, height);
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, float(width) / height, 0.1, 1e5);
}

void handleKeyPress(unsigned char key, int x, int y) {
    keysPressed[as<int>(key)] = true;
    switch (key) {
    case 27: // ESC
        glutLeaveMainLoop();
        break;

        // TODO: Make a nice command system.
    }
}

void handleMouseMotion(int x, int y) {
    int dx = x - mouse.x;
    int dy = y - mouse.y;

    camera.rotate(-1e-3f * as<float>(dx), -1e-3f * as<float>(dy));

    mouse = glm::vec2(x, y);
}

void handleMouseClick(int button, int state, int x, int y) {
    mouse = glm::ivec2(x, y);

    if (state == GLUT_DOWN) {
        switch (button) {
        case GLUT_RIGHT_BUTTON: {
            auto *body = makeBall(1e-2f, glm2bt(camera.pos()));
            body->setLinearVelocity(glm2bt(camera.forward()));

            game->world()->addRigidBody(body);
        } break;
        }
    }
}

template <typename DirectionalObject>
void moveFromWASDQE(DirectionalObject &obj, float speed, float dt) {
    glm::vec3 vel = glm::vec3();

    if (keysPressed[' ']) {
        speed *= 5.0f;
    }

    if (keysPressed['w']) {
        vel += obj.forward();
    }
    if (keysPressed['s']) {
        vel -= obj.forward();
    }

    if (keysPressed['d']) {
        vel += obj.right();
    }
    if (keysPressed['a']) {
        vel -= obj.right();
    }

    // Normalize before doing up and down, the speed is indepdent of them.
    if (glm::length(vel) != 0.0f) {
        vel = glm::normalize(vel);
    }

    if (keysPressed['q']) {
        vel += obj.up();
    }
    if (keysPressed['e']) {
        vel -= obj.up();
    }

    obj.moveBy(speed * dt * vel);
}

void initOpenGL(int *argcp, char **argv) {
    glutInit(argcp, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA);

    glutInitWindowSize(1024, 1024);
    glutCreateWindow("This is what it's like when worlds collide!");

    constexpr float shade = 0.85f;
    glClearColor(shade, shade, shade, 1.0f);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                  GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

    glutReshapeFunc(resize);
    glutDisplayFunc(render);
    glutKeyboardFunc(handleKeyPress);
    glutKeyboardUpFunc([](unsigned char key, int, int) {
        keysPressed[as<int>(key)] = false; //
    });

    glutMotionFunc(handleMouseMotion);
    glutMouseFunc(handleMouseClick);
}

int main(int argc, char **argv) {
    srand(24);

    initOpenGL(&argc, argv);
    initScene();

    update(0);
    glutMainLoop();

    return 0;
}
