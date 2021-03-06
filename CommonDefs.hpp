#pragma once

// glm
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"

// OpenGL
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

// Bullet Physics
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

// Common STL Headers
#include <iostream> // std::ostream for overloading operator<<

// We really, really, really want assert to do things. Always.
#ifdef NDEBUG
#define REPLACE_NDEBUG NDEBUG
#undef NDEBUG
#include <cassert>
#define NDEBUG REPLACE_NDEBUG
#undef REPLACE_NDEBUG
#else
#include <cassert>
#endif

// Convenient Functions
namespace {

glm::vec3 bt2glm(const btVector3 &bt_vec) {
    return glm::vec3(bt_vec.getX(), bt_vec.getY(), bt_vec.getZ());
}

btVector3 glm2bt(const glm::vec3 &glm_vec) {
    return btVector3(glm_vec.x, glm_vec.y, glm_vec.z);
}

std::ostream &operator<<(std::ostream &os, const glm::vec3 &vec) {
    return os << "glm::vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}

std::ostream &operator<<(std::ostream &os, const glm::vec2 &vec) {
    return os << "glm::vec2(" << vec.x << ", " << vec.y << ")";
}

template <typename T, typename U>
T as(U thing) {
    return static_cast<T>(thing);
}

float getRand(float min, float max) {
    float unit = as<float>(rand()) / RAND_MAX;
    return (max - min) * unit + min;
}
}
// OpenGL Error Checking
#define glChk() glChk_impl(__FILE__, __LINE__)

#define make_case(ERR)                                                         \
    case ERR:                                                                  \
        str = #ERR;                                                            \
        break;

static void glChk_impl(const char *file, int line) {
    const char *str = nullptr;
    switch (glGetError()) {
        make_case(GL_INVALID_ENUM);
        make_case(GL_INVALID_VALUE);
        make_case(GL_INVALID_OPERATION);
        make_case(GL_OUT_OF_MEMORY);
        make_case(GL_STACK_UNDERFLOW);
        make_case(GL_STACK_OVERFLOW);
    case GL_NO_ERROR:
        return;

    default:
        // TODO: Print the value.
        str = "Unknown error";
    }

    std::cerr << "[OpenGL] " << file << ":" << line << " " << str << std::endl;
    abort();
}
#undef make_case
