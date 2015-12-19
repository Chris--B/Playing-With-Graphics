#include "btBulletDynamicsCommon.h"

#define GLEW_STATIC
#include "GL/glew.h"
#include "GL/glut.h"

class GLDebugDrawer : public btIDebugDraw {
public:
    virtual void setDebugMode(int debugMode) override {
        m_debugMode = debugMode;
    }
    virtual int getDebugMode() const override { return m_debugMode; }

    virtual void reportErrorWarning(const char *warningString) override;

    virtual void drawLine(const btVector3 &from, const btVector3 &to,
                          const btVector3 &color) override;
    virtual void draw3dText(const btVector3 &location,
                            const char *textString) override;
    virtual void drawContactPoint(const btVector3 &pointOnB,
                                  const btVector3 &normalOnB, btScalar distance,
                                  int lifeTime,
                                  const btVector3 &color) override;

private:
    int m_debugMode = 0;
};
