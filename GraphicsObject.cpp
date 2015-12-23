#include "GraphicsObject.hpp"

void GraphicsObject::draw() const {
    if (shapes.empty()) {
        return;
    }

    // TODO: glVertexPointer / VBOs.
    glBegin(GL_TRIANGLES);
    for (const auto &shape : shapes) {
        assert(shape.mesh.indices.size() % 3 == 0);

        int material_id = shape.mesh.material_ids[0];
        if (material_id != -1) {
            // glMaterialfv expects four floats, but tinyobjloader only saves
            // three.
            float diffuse[4] = {materials[material_id].diffuse[0],
                                materials[material_id].diffuse[1],
                                materials[material_id].diffuse[2],
                                1.0f};
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse);
        }
        for (const auto &idx : shape.mesh.indices) {
            if (!shape.mesh.normals.empty()) {
                glNormal3fv(&shape.mesh.normals[3 * idx]);
            }
            glVertex3fv(&shape.mesh.positions[3 * idx]);
        }
    }
    glEnd();
}

void GraphicsObject::loadObjFile(const std::string &filename) {
    std::string error;

    std::cout << "[tinyobjloader] Loading " << filename << std::endl;
    bool ok = tinyobj::LoadObj(
        shapes, materials, error, filename.c_str(), "../OBJ/lost_empire/");
    if (!ok) {
        std::cerr << "Error:  " << error << std::endl;
        abort();
    }

    size_t verts = 0;
    for (const auto &shape : shapes) {
        verts += shape.mesh.indices.size();
    }

    std::cout << "Loaded " << verts << " vertices\n"           //
              << "       " << materials.size() << " materials" //
              << std::endl;
}
