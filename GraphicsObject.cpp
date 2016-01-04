#include "GraphicsObject.hpp"

#include <iomanip>
#include <iostream>

// Organize our uniform idicies.
// We hard code these into all of our shaders.
namespace Idx {

// Uniforms
enum {
    projection = 0,
    view       = 1,
    model      = 2,
    sunlight   = 3,
};

// Inputs
enum {
    vertex = 0,
    normal = 1,

    ambient = 10,
    diffuse = 11,
};
}


void GraphicsObject::draw(const glm::mat4x4 &projection,
                          const glm::mat4x4 &view,
                          const glm::mat4x4 &model) const {
    glChk();

    glUseProgram(shader);

    // TODO: These do not need to be set every frame!
    glUniformMatrix4fv(
        Idx::projection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(Idx::view, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(Idx::model, 1, GL_FALSE, glm::value_ptr(model));

    for (auto pair : vao_pairs) {
        glBindVertexArray(pair.first);
        glDrawElements(GL_TRIANGLES, pair.second, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
    glUseProgram(0);
}

void loadBufferObject(GLint idx, const std::vector<float> &data) {
    glChk();
    assert(data.size() % 3 == 0 && __FUNCTION__ " expects its data as vec3s.");
    
    if (data.empty()) {
        // The object we're trying to load doesn't have any data to load...
        // So fail silently, else OpenGL tries to read through a NULL pointer.
        return;
    }

    // We don't even need the buffer handle - the VAO does it all for us - so
    // this isn't returned.
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(data[0]) * data.size(),
                 &data[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glChk();

    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glChk();
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

    assert(shader != 0 &&
           "A shader was not attached before trying to load a file.");
    glUseProgram(shader);

    // TODO: This does not need to be set here.
    float sunlight[3] = { 2.0f, 20.0f, 1.0f };
    glUniform3fv(Idx::sunlight, 1, sunlight);

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
        loadBufferObject(Idx::vertex, shape.mesh.positions);


        // Normals
        loadBufferObject(Idx::normal, shape.mesh.normals);

        // Materials
        // tinyobjloader doesn't store these in an easy-to-use fashion,
        // so we make our own buffer.
        std::vector<float> diffuse;
        for (const auto &idx : shape.mesh.indices) {
            int material_id = shape.mesh.material_ids[idx / 3];
            diffuse.push_back(materials[material_id].diffuse[0]);
            diffuse.push_back(materials[material_id].diffuse[1]);
            diffuse.push_back(materials[material_id].diffuse[2]);
        }
        loadBufferObject(Idx::diffuse, diffuse);

        vao_pairs.emplace_back(vao, count);
    }

    std::cout << "\t" << std::string(39, '=') << "\n"                   //
              << "\t" << std::setw(30) << verts << " vertices\n"        //
              << "\t" << std::setw(30) << (verts / 3) << " triangles\n" //
              << std::endl;
}
