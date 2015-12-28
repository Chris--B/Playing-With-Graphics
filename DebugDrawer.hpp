#include "btBulletDynamicsCommon.h"

#include "CommonDefs.hpp"

#include <vector>

class GLDebugDrawer : public btIDebugDraw {
public:
    GLDebugDrawer() = default;
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

    void flushLines() override;

private:
    struct Line {
        btVector3 from;
        btVector3 to;
        btVector3 color;

        Line() = default;
        Line(const btVector3 &from, const btVector3 &to, const btVector3 &color)
            : from(from), to(to), color(color) {}
    };

    std::vector<Line> m_lines;
    int m_debugMode = btIDebugDraw::DBG_DrawWireframe;
};
