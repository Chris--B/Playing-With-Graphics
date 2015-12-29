#include "GraphicsObject.hpp"

#include <iomanip>
#include <iostream>

void GraphicsObject::draw() const {
    if (shapes.empty()) {
        return;
    }

    // TODO: glVertexPointer / VBOs.
    glBegin(GL_TRIANGLES);
    for (const auto &shape : shapes) {
        for (const auto &idx : shape.mesh.indices) {
            int material_id = shape.mesh.material_ids[idx / 3];
            if (material_id != -1) {
                // OpenGL expects four floats, but tinyobjloader saves three.
                glm::vec4 ambient;
                ambient.r = materials[material_id].ambient[0];
                ambient.g = materials[material_id].ambient[1];
                ambient.b = materials[material_id].ambient[2];
                ambient.a = 1.0f;
                glMaterialfv(GL_FRONT, GL_AMBIENT, glm::value_ptr(ambient));

                glm::vec4 diffuse;
                diffuse.r = materials[material_id].diffuse[0];
                diffuse.g = materials[material_id].diffuse[1];
                diffuse.b = materials[material_id].diffuse[2];
                diffuse.a = 1.0f;
                glMaterialfv(GL_FRONT, GL_DIFFUSE, glm::value_ptr(diffuse));
            }

            if (!shape.mesh.texcoords.empty()) {
                glTexCoord2f(shape.mesh.texcoords[2 * idx],
                             shape.mesh.texcoords[2 * idx + 1]);
            }

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

    std::string mtl_path = "./";
    // Assume the MTL is located in the same directory as the OBJ.
    auto idx = filename.rfind('/');
    if (idx != std::string::npos) {
        // Keep the '/' in the path
        mtl_path = filename.substr(0, idx + 1);
    }

    std::cout << "[tinyobjloader] Loading              " << filename << "\n" //
              << "                Searching for MTL in " << mtl_path
              << std::endl;
    bool ok = tinyobj::LoadObj(
        shapes, materials, error, filename.c_str(), mtl_path.c_str());
    if (!ok) {
        std::cerr << "Error:  " << error << std::endl;
        abort();
    }

    size_t verts = 0;
    for (const auto &shape : shapes) {
        assert(shape.mesh.indices.size() % 3 == 0);   // Rendered Triangles
        assert(shape.mesh.positions.size() % 3 == 0); // Unique Triangles

        const auto indices   = shape.mesh.indices.size();
        const auto positions = shape.mesh.positions.size();

        assert(shape.mesh.material_ids.size() == indices / 3);
        assert(shape.mesh.texcoords.empty() ||
               shape.mesh.texcoords.size() == 2 * (positions / 3));
        assert(shape.mesh.normals.empty() ||
               shape.mesh.normals.size() == positions);

        std::cout << std::setfill(' ') << "\t" << std::left //
                  << std::setw(20) << shape.name            //
                  << std::setw(10) << std::right            //
                  << (shape.mesh.indices.size() / 3) << " vertices\n";
        verts += shape.mesh.indices.size();
    }
    std::cout << "\t" << std::string(39, '=') << "\n"                   //
              << "\t" << std::setw(30) << verts << " vertices\n"        //
              << "\t" << std::setw(30) << (verts / 3) << " triangles\n" //
              << std::endl;
}
