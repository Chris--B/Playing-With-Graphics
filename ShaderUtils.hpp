#pragma once
#include "CommonDefs.hpp"

#include <string>


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

GLint loadCompileAndLink(const std::string &folder);

GLint loadAndCompileShader(const std::string &filename, GLenum type);
GLint compileShader(const std::string &source, GLenum type);

bool linkProgram(GLint program);
