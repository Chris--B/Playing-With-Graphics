#include "btBulletDynamicsCommon.h"

#include "CommonDefs.hpp"

#include <vector>

class GLDebugDrawer : public btIDebugDraw {
public:
    GLDebugDrawer() = default;
    GLDebugDrawer(GLint vao, GLint shader);

    virtual void setDebugMode(int debugMode) override {
        m_debugMode = debugMode;
    }

    virtual int getDebugMode() const override { return m_debugMode; }

    virtual void reportErrorWarning(const char *warningString) override;

    virtual void drawLine(const btVector3 &from,
                          const btVector3 &to,
                          const btVector3 &color) override;

    virtual void drawContactPoint(const btVector3 &pointOnB,
                                  const btVector3 &normalOnB,
                                  btScalar         distance,
                                  int              lifeTime,
                                  const btVector3 &color) override;

    virtual void draw3dText(const btVector3 &location,
                            const char *     textString) override;

    void flushLines() override;

    void updateBuffers();
    void updateUniforms(const glm::mat4x4 &projection, const glm::mat4x4 &view);

private:
    struct Vertex {
        glm::vec3 point;
        glm::vec3 color;

        Vertex(glm::vec3 point, glm::vec3 color) : point(point), color(color) {}
    };
    GLuint vao    = 0;
    GLuint vbo    = 0;
    GLuint shader = 0; // TODO: Write a shader for this

    int m_debugMode =
        btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawFrames;
    std::vector<Vertex> m_verts;
};
