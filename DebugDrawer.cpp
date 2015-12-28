#include "DebugDrawer.hpp"

#include <iostream>

void GLDebugDrawer::reportErrorWarning(const char *warningString) {
    std::cerr << "[DebugDrawer] " << warningString << std::endl;
}

void GLDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to,
                             const btVector3 &color) {
    if (!m_debugMode) {
        return;
    }

    glColor3f(color.getX(), color.getY(), color.getZ());

    glBegin(GL_LINES);
    glVertex3f(from.getX(), from.getY(), from.getZ());
    glVertex3f(to.getX(), to.getY(), to.getZ());
    glEnd();
}

// These don't get used, but we need to implement them,
void GLDebugDrawer::drawContactPoint(const btVector3 &pointOnB,
                                     const btVector3 &normalOnB,
                                     btScalar distance, int lifeTime,
                                     const btVector3 &color) {
    reportErrorWarning(__FUNCTION__ " is not implemented.");
    abort();
}

void GLDebugDrawer::draw3dText(const btVector3 &location,
                               const char *textString) {
    reportErrorWarning(__FUNCTION__ " is not implemented.");
    abort();
}