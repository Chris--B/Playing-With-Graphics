#pragma once

#include "CommonDefs.hpp"

#include "tinyobjloader/tiny_obj_loader.h"

#include <vector>

struct GraphicsObject {
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    void draw() const;

    void loadObjFile(const std::string &filename);
};
