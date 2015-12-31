#pragma once
#include "CommonDefs.hpp"

#include <string>

GLint loadAndCompileShader(const std::string &filename, GLenum type);
GLint compileShader(const std::string &source, GLenum type);

bool linkProgram(GLint program);
