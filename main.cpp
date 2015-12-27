#include "Camera.hpp"
#include "CommonDefs.hpp"
#include "GameWorld.hpp"

#include <memory>

std::unique_ptr<GameWorld> game;
GLFWwindow *window = nullptr;

glm::vec2 windowSize = glm::vec2(1024, 1024);

bool keysPressed[GLFW_KEY_LAST] = {};
bool needsReset                 = false;

Camera camera(glm::vec3(20.0f, 50.0f, 20.0f));
Entity *player = nullptr;

btSphereShape ballShape = btSphereShape(0.2f);
btBoxShape boxShape = btBoxShape(btVector3(1, 1, 1));

GraphicsObject *cubeModel = nullptr;
GraphicsObject *model     = nullptr;


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
    cubeModel->loadObjFile("../OBJ/cube/cube.obj");

    model = new GraphicsObject();
    model->loadObjFile("../OBJ/lost_empire/lost_empire.obj");
    // model->loadObjFile("../OBJ/rungholt/rungholt.obj");

    // Player and their hitbox
    {
        auto *hitboxShape = new btBoxShape(btVector3(0.50f, 1.75f, 0.50f));
        const float mass = 77.0f; // kg
        btVector3 inertia;
        hitboxShape->calculateLocalInertia(mass, inertia);

        btTransform transform;
        transform.setOrigin(btVector3(0, 50, 0));

        auto *motionState = new btDefaultMotionState(transform);
        auto rbInfo       = btRigidBody::btRigidBodyConstructionInfo(
            mass, motionState, hitboxShape, inertia);
        auto *hitbox = new btRigidBody(rbInfo);

        player = new Entity(cubeModel, hitbox);
        game->addEntity(player);
    }

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

void update(double t, float dt) {
    if (needsReset) {
        needsReset = false;

        delete player;
        player = nullptr;

        delete model;
        delete cubeModel;
        game.release();

        initScene();
    }

    // TODO: Make this a function.
    // Scroll around with the left mouse button by clicking and dragging.
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // TODO: Save the last position and work off of that, OR
        // use glfwSetInputMode();
        // The arbitrary values control the feel of the camera and should
        // probably be loaded from a configuration file.
        static constexpr float sensitivity = 5.0f * -1.0e1f;

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        auto st = glm::vec2(x, y) / windowSize;

        auto delta = st - glm::vec2(0.5, 0.5);
        // If it's out of bounds, ignore it.
        if (abs(delta.x) > 0.5f) {
            delta.x = 0.0f;
        }
        if (abs(delta.y) > 0.5f) {
            delta.y = 0.0f;
        }

        // A nonlinear speed up towards the edges feels better.
        // Odd exponents presever sign.
        auto d3     = delta * delta * delta;
        auto angles = sensitivity * dt * d3;
        camera.rotate(angles.x, angles.y);
    }

    moveFromWASDQE(camera, 15.0f, dt);

    game->update(dt);
}

void render() {
    glm::mat4x4 projection = camera.lookAt();
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(projection));

    // Directional lighting
    glEnable(GL_LIGHTING);

    float light_dir[4] = {1.0f, 2.0f, 1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_dir);

    game->draw(projection);

    auto *world = game->world();
    world->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    world->debugDrawWorld();

    // world->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawAabb);
    // world->debugDrawWorld();
}

void resize(GLFWwindow *window, int width, int height) {
    windowSize = glm::vec2(width, height);
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, float(width) / height, 0.1, 1e5);
}

void handleMouseClick(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        // Add a ball into the scene with the right mouse button.
        if (action == GLFW_RELEASE) {
            // button.

            // Don't spawn in directly ontop of the camera.
            static float dist = 5 * ballShape.getRadius();
            // TODO: Adjust this with the user's click
            btVector3 dir = glm2bt(camera.forward());
            auto *body = makeBall(1.0f, glm2bt(camera.pos()) + dist * dir);
            body->setLinearVelocity(30 /*m/s*/ * dir);

            game->world()->addRigidBody(body);
        }
    }
}

template <typename DirectionalObject>
void moveFromWASDQE(DirectionalObject &obj, float speed, float dt) {
    glm::vec3 vel = glm::vec3();

    if (keysPressed[GLFW_KEY_SPACE]) {
        speed *= 5.0f;
    }

    if (keysPressed[GLFW_KEY_W]) {
        vel += obj.forward();
    }
    if (keysPressed[GLFW_KEY_S]) {
        vel -= obj.forward();
    }

    if (keysPressed[GLFW_KEY_D]) {
        vel += obj.right();
    }
    if (keysPressed[GLFW_KEY_A]) {
        vel -= obj.right();
    }

    // Normalize before doing up and down, the speed is indepdent of them.
    if (glm::length(vel) != 0.0f) {
        vel = glm::normalize(vel);
    }

    if (keysPressed[GLFW_KEY_Q]) {
        vel += obj.up();
    }
    if (keysPressed[GLFW_KEY_E]) {
        vel -= obj.up();
    }

    obj.moveBy(speed * dt * vel);
}

static void handleKeyboard(GLFWwindow *window, int key, int scancode,
                           int action, int mode) {
    if (key == GLFW_KEY_UNKNOWN) {
        std::cout << "Unknown key pressed!\n";
        return;
    }

    if (action == GLFW_PRESS) {
        keysPressed[key] = true;
    } else if (action == GLFW_RELEASE) {
        keysPressed[key] = false;
    }

    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;

        case GLFW_KEY_R:
            needsReset = true;
            break;
        }
    }
}

void initGLFW() {
    if (!glfwInit()) {
        std::cerr << "Bad things have happened." << std::endl;
        abort();
    }

    // We register this callback first, incase something
    // goes wrong with glfwCreateWindow.
    glfwSetErrorCallback([](int error, const char *description) {
        std::cerr << "[GLFW] Error #" << error << ": " << description
                  << std::endl;
    });

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // Set up our window and make it current.
    window = glfwCreateWindow(as<int>(windowSize.x),
                              as<int>(windowSize.y),
                              "Playing with GLFW",
                              nullptr,
                              nullptr);
    if (!window) {
        glfwTerminate();
        // This is easy to catch in debuggers.
        abort();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // The rest of the GLFW Callbacks
    glfwSetKeyCallback(window, handleKeyboard);
    glfwSetMouseButtonCallback(window, handleMouseClick);
    glfwSetFramebufferSizeCallback(window, resize);
}

void initOpenGL() {
    // Call this once to setup viewports, etc.
    resize(window, as<int>(windowSize.x), as<int>(windowSize.y));

    glm::vec4 color(glm::vec3(0, 178, 228) / 255.0f, 1.0f);
    glClearColor(color.r, color.g, color.b, color.a);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

int main() {
    // Chosen by fair dice roll.
    srand(24);

    initGLFW();
    initOpenGL();

    initScene();

    double oldt = glfwGetTime();

    // TODO: This loop needs work.
    while (!glfwWindowShouldClose(window)) {
        const double t    = glfwGetTime();
        const float dt    = as<float>(t - oldt);
        const float ratio = as<float>(windowSize.x) / windowSize.y;

        glfwPollEvents();
        update(t, dt);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render();

        glfwSwapBuffers(window);

        oldt = t;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
