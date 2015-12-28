#include "btBulletDynamicsCommon.h"

#include "CommonDefs.hpp"

class GLDebugDrawer : public btIDebugDraw {
public:
    GLDebugDrawer() : m_debugMode(btIDebugDraw::DBG_FastWireframe) {}

    virtual void setDebugMode(int debugMode) override {
        m_debugMode = debugMode;
    }

    virtual int getDebugMode() const override { return m_debugMode; }

    virtual void reportErrorWarning(const char *warningString) override;

    virtual void drawLine(const btVector3 &from, const btVector3 &to,
                          const btVector3 &color) override;

    virtual void drawContactPoint(const btVector3 &pointOnB,
                                  const btVector3 &normalOnB, btScalar distance,
                                  int lifeTime,
                                  const btVector3 &color) override;

    virtual void draw3dText(const btVector3 &location,
                            const char *textString) override;

private:
    int m_debugMode = 0;
};
