#include "ShaderUtils.hpp"

#include <fstream>
#include <sstream>

#define error() error_impl(__FILE__, __LINE__)
std::ostream &error_impl(const char *file, int line) {
    return std::cerr << "[Shaders] " << file << ":" << line << " ";
}

GLint loadCompileAndLink(const std::string &folder) {
    GLint vert = loadAndCompileShader(folder + "/vert.glsl", GL_VERTEX_SHADER);
    GLint frag =
        loadAndCompileShader(folder + "/frag.glsl", GL_FRAGMENT_SHADER);

    GLint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    bool ok = linkProgram(program);
    assert(ok && "Failed to load, compile, and link shader program.");

    return program;
}

GLint loadAndCompileShader(const std::string &filename, GLenum type) {
    std::stringstream ss;
    // Make sure the file is closed before we try to compile, so that
    // the file handle isn't held open if we call abort() and need to debug.
    {
        std::ifstream file(filename);
        ss << file.rdbuf();
    }
    return compileShader(ss.str(), type);
}

GLint compileShader(const std::string &source, GLenum type) {
    GLint       shader = glCreateShader(type);
    const char *cstr   = source.c_str();
    glShaderSource(shader, 1, &cstr, nullptr);
    glChk();

    glCompileShader(shader);
    glChk();

    GLint compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        std::string log(length, '-');
        glGetShaderInfoLog(shader, length, &length, &log[0]);

        error() << "Shader compilation went south:\n" << log << std::endl;
        assert(length == log.size());

        glDeleteShader(shader);
        shader = 0;
    }
    assert(shader != 0);

    return shader;
}

bool linkProgram(GLint program) {
    glLinkProgram(program);

    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        GLint length = 0;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);

        std::string log(length, '-');
        glGetProgramInfoLog(program, length, &length, &log[0]);

        error() << "Shader linking went south:\n" << log << std::endl;
        assert(length == log.size());

        return false;
    }
    return true;
}
