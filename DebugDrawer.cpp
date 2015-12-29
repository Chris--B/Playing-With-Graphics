#include "DebugDrawer.hpp"

#include <iostream>

void GLDebugDrawer::reportErrorWarning(const char *warningString) {
    std::cerr << "[DebugDrawer] " << warningString << std::endl;
}

void GLDebugDrawer::drawLine(const btVector3 &from,
                             const btVector3 &to,
                             const btVector3 &color) {
    m_lines.emplace_back(from, to, color);
}

void GLDebugDrawer::flushLines() {
    // TODO: VBO or something.
    glBegin(GL_LINES);
    for (const auto &line : m_lines) {
        glColor3f(line.color.getX(), line.color.getY(), line.color.getZ());
        glVertex3f(line.from.getX(), line.from.getY(), line.from.getZ());
        glVertex3f(line.to.getX(), line.to.getY(), line.to.getZ());
    }
    glEnd();

    m_lines.clear();
}

// These don't get used, but we need to give them an implementation.
#pragma region unused
void GLDebugDrawer::drawContactPoint(const btVector3 &pointOnB,
                                     const btVector3 &normalOnB,
                                     btScalar         distance,
                                     int              lifeTime,
                                     const btVector3 &color) {
    // TODO: Figure out what this is and implement it.
    reportErrorWarning(__FUNCTION__ " is not implemented.");
    abort();
}

void GLDebugDrawer::draw3dText(const btVector3 &location,
                               const char *     textString) {
    reportErrorWarning(__FUNCTION__ " is not implemented.");
    abort();
}
#pragma endregion
