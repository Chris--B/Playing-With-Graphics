#include "OpenGLDebugDrawer.hpp"

#include "ShaderUtils.hpp"

#include <iostream>

GLDebugDrawer::GLDebugDrawer(GLint vao, GLint shader)
    : vao(vao), shader(shader) {}

void GLDebugDrawer::reportErrorWarning(const char *warningString) {
    std::cerr << "[DebugDrawer] " << warningString << std::endl;
}

void GLDebugDrawer::drawLine(const btVector3 &from,
                             const btVector3 &to,
                             const btVector3 &color) {
    m_verts.emplace_back(bt2glm(from), bt2glm(color));
    m_verts.emplace_back(bt2glm(to), bt2glm(color));
}

void GLDebugDrawer::flushLines() {
    updateBuffers();
    assert(m_verts.size() % 2 == 0);

    glUseProgram(shader);
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, as<GLsizei>(m_verts.size() / 2));
    glBindVertexArray(0);

    // TODO: Maybe there's a way to cache stuff?
    m_verts.clear();
    glDeleteBuffers(1, &vbo);
    vbo = 0;
}

void GLDebugDrawer::updateBuffers() {
    if (m_verts.empty()) {
        return;
    }
    assert(vao != 0);
    glBindVertexArray(vao);
    glChk();

    glUseProgram(shader);

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(m_verts[0]) * m_verts.size(),
                 &m_verts[0],
                 GL_DYNAMIC_DRAW);

    // TODO: Real indices
    glEnableVertexAttribArray(Idx::vertex);
    glVertexAttribPointer(Idx::vertex,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void *)offsetof(Vertex, point));

    glEnableVertexAttribArray(Idx::ambient);
    glVertexAttribPointer(Idx::ambient,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void *)offsetof(Vertex, color));

    glChk();
}

void GLDebugDrawer::updateUniforms(const glm::mat4x4 &projection,
                                   const glm::mat4x4 &view) {
    assert(shader != 0);
    glUseProgram(shader);
    glUniformMatrix4fv(
        Idx::projection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(Idx::view, 1, GL_FALSE, glm::value_ptr(view));
    glUseProgram(0);
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
