#include "Camera.hpp"
#include "gl_defs.hpp"
#include "PhysicsWorld.hpp"

#include <iostream>

#include <cassert>

PhysicsWorld phys;

Camera camera(glm::vec3(20.0f, 10.0f, 20.0f));

glm::ivec2 mouse;

constexpr int resetFrames = 60 /*fps*/ * 10 /*seconds*/;
static int frame          = 0;

void update(int) {
    float dt = 1.0f / 60;
    glutTimerFunc(1000 / 60, update, 0);

    frame += 1;

    if (frame >= resetFrames) {
        phys.init();
        frame = 0;
    }

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
    switch (key) {
    case 27: // ESC
        glutLeaveMainLoop();
        break;
    }
}

void handleMouseMotion(int x, int y) {
    int dx = x - mouse.x;
    int dy = -(y - mouse.y); // +y is downward in screen space.

    mouse = glm::vec2(x, y);
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

    glutReshapeFunc(resize);
    glutDisplayFunc(render);
    glutKeyboardFunc(handleKeyPress);
    glutMotionFunc(handleMouseMotion);

    update(0);
    glutMainLoop();

    return 0;
}
