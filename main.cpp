#include "Camera.hpp"
#include "CommonDefs.hpp"
#include "GameWorld.hpp"

std::unique_ptr<GameWorld> game;
btSphereShape ballShape = btSphereShape(0.2f);

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
    return body;
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(camera.lookAt()));

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
            auto *body
                = makeBall(1e-2f, glm2bt(camera.pos()));
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

    game = std::make_unique<GameWorld>();

    initOpenGL(&argc, argv);

    update(0);
    glutMainLoop();

    return 0;
}
