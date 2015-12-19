#include "Camera.hpp"
#include "gl_defs.hpp"
#include "PhysicsWorld.hpp"
#include "Util.hpp"

#include <iostream>

#include <cassert>

PhysicsWorld phys;

Camera camera(glm::vec3(20.0f, 10.0f, 20.0f));

glm::ivec2 mouse;

bool keysPressed[256] = {};

constexpr int resetFrames = 60 /*fps*/ * 10 /*seconds*/;
static int frame          = 0;

// Apply WASD + QE commands to an object.
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
        phys.init();
        frame = 0;
    }

    moveFromWASDQE(camera, 15.0f, dt);

    phys.update(dt);

    glutPostRedisplay();
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(camera.lookAt()));

    phys.debugDrawWorld();

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
    int dy = -(y - mouse.y); // +y is downward in screen space.

    mouse = glm::vec2(x, y);
}

template <typename DirectionalObject>
void moveFromWASDQE(DirectionalObject &obj, float speed, float dt) {
    glm::vec3 vel = glm::vec3();

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

    if (keysPressed['q']) {
        vel += obj.up();
    }
    if (keysPressed['e']) {
        vel -= obj.up();
    }

    // Nothing was pressed, so there's nothing to do.
    if (glm::length(vel) == 0.0f) {
        return;
    }

    obj.moveBy(speed * dt * glm::normalize(vel));
}

int main(int argc, char **argv) {
    srand(24);
    phys.init();

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

    update(0);
    glutMainLoop();

    return 0;
}
