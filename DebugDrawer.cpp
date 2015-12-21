#include "DebugDrawer.hpp"

#include <iostream>

void GLDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to,
                             const btVector3 &color) {
    glPointSize(5.0f);

    glPushMatrix();
    {
        glColor3f(color.getX(), color.getY(), color.getZ());
        glBegin(GL_LINES);
        glVertex3f(from.getX(), from.getY(), from.getZ());
        glVertex3f(to.getX(), to.getY(), to.getZ());
        glEnd();

        glColor3f(
            0.5f * color.getX(), 0.5f * color.getY(), 0.5f * color.getZ());
        glBegin(GL_POINTS);
        glVertex3f(from.getX(), from.getY(), from.getZ());
        glVertex3f(to.getX(), to.getY(), to.getZ());
        glEnd();
    }
    glPopMatrix();
}

void GLDebugDrawer::draw3dText(const btVector3 &location,
                               const char *textString) {}

void GLDebugDrawer::reportErrorWarning(const char *warningString) {
    std::cerr << warningString << std::endl;
}

void GLDebugDrawer::drawContactPoint(const btVector3 &pointOnB,
                                     const btVector3 &normalOnB,
                                     btScalar distance, int lifeTime,
                                     const btVector3 &color) {}
