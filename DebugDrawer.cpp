#include "DebugDrawer.hpp"

#include <iostream>

void GLDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to,
                             const btVector3 &color) {

    // if (m_debugMode > 0)
    {
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
}

void GLDebugDrawer::draw3dText(const btVector3 &location,
                               const char *textString) {
    // glDisable(GL_LIGHTING);
    // glRasterPos3f(location.x(),  location.y(),  location.z());
    // BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
}

void GLDebugDrawer::reportErrorWarning(const char *warningString) {
    std::cerr << warningString << std::endl;
}

void GLDebugDrawer::drawContactPoint(const btVector3 &pointOnB,
                                     const btVector3 &normalOnB,
                                     btScalar distance, int lifeTime,
                                     const btVector3 &color) {
    // if (m_debugMode > 0)
    {
        // glDisable(GL_LIGHTING);
        // btVector3 to=pointOnB+normalOnB*distance;
        // const btVector3&from = pointOnB;
        // glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);

        // GLDebugDrawer::drawLine(from, to, color);

        // glRasterPos3f(from.x(),  from.y(),  from.z());
        // char buf[12];
        // sprintf(buf," %d",lifeTime);
        // BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);
    }
}
