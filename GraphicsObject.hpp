#pragma once

#include "CommonDefs.hpp"

#include "tinyobjloader/tiny_obj_loader.h"

#include <vector>

struct GraphicsObject {
    GLint                            shader = 0;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;

    std::vector<std::pair<GLuint, GLsizei>> vao_pairs;

    void draw(const glm::mat4x4 &projection,
              const glm::mat4x4 &view,
              const glm::mat4x4 &model) const;

    void loadObjFile(const std::string &filename);
};
