#pragma once

#include "CommonDefs.hpp"

#include "tinyobjloader/tiny_obj_loader.h"

#include <vector>

struct GraphicsObject {
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;

    std::vector<std::pair<GLuint, GLsizei>> vao_pairs;

    void draw() const;

    void loadObjFile(const std::string &filename);
};
