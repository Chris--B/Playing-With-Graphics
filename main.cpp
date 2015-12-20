#include "Camera.hpp"
#include "gl_defs.hpp"
#include "Util.hpp"

#include <iostream>

#include <cassert>

#include "GameWorld.hpp"

GameWorld *game;

Camera camera(glm::vec3(20.0f, 10.0f, 20.0f));

glm::ivec2 mouse;

bool keysPressed[256] = {};

constexpr int resetFrames = 60 /*fps*/ * 10 /*seconds*/;
static int frame          = 0;

// Apply WASD + QE commands to an object. Space for extra speed.
// A DirectionalObject must have the following methods:
//      void moveBy(glm::vec3);
//      glm::vec3 forward(); // W and S
//      glm::vec3 right();   // A and D
//      glm::vec3 up();      // Q and E
// 'forward()', 'right()', and 'up()' should be normalized.
template <typename DirectionalObject>
void moveFromWASDQE(DirectionalObject &obj, float speed, float dt);

void update(int) {
    float dt = 1.0f / 60;
    glutTimerFunc(1000 / 60, update, 0);

    frame += 1;

    if (frame >= resetFrames) {
        delete game;
        game  = new GameWorld();
        frame = 0;
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

int main(int argc, char **argv) {
    srand(24);

    game = new GameWorld();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA);

    glutInitWindowPosition((1920 - 1024) / 4, (1080 - 1024) / 4);
    glutInitWindowSize(1024, 1024);
    glutCreateWindow("This is what it's like when worlds collide!");

    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
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
    glutMouseFunc([](int button, int state, int x, int y) {
        mouse = glm::ivec2(x, y); //
    });

    update(0);
    glutMainLoop();

    return 0;
}
