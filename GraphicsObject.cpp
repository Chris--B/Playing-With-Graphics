#include "GraphicsObject.hpp"

#include <iomanip>
#include <iostream>

void GraphicsObject::draw() const {
    if (shapes.empty()) {
        return;
    }

    for (auto pair : vao_pairs) {
        glBindVertexArray(pair.first);
        glDrawElements(GL_TRIANGLES, pair.second, GL_UNSIGNED_INT, nullptr);
    }
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

    glChk();
    size_t verts = 0;
    for (const auto &shape : shapes) {
        // Sanity checks.
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

        verts += indices;

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glChk();

        // Indices
        GLuint  ibo;
        GLsizei count = as<GLsizei>(indices);
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(shape.mesh.indices[0]) * count,
                     &shape.mesh.indices[0],
                     GL_STATIC_DRAW);
        glChk();

        // Vertices
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(shape.mesh.positions[0]) * positions,
                     &shape.mesh.positions[0],
                     GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glChk();

        // Normals
        GLuint normals;
        GLint  normalIdx = 2; // glGetAttribLocation(0, // TODO: load a program!
                              //                "gl_Normal");
        glChk();

        glGenBuffers(1, &normals);
        glBindBuffer(GL_ARRAY_BUFFER, normals);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(shape.mesh.normals[0]) * positions,
                     &shape.mesh.normals[0],
                     GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(normalIdx, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glChk();

        vao_pairs.emplace_back(vao, count);
    }

    std::cout << "\t" << std::string(39, '=') << "\n"                   //
              << "\t" << std::setw(30) << verts << " vertices\n"        //
              << "\t" << std::setw(30) << (verts / 3) << " triangles\n" //
              << std::endl;
}
