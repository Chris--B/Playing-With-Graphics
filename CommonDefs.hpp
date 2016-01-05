#pragma once

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

#ifdef USE_OPENGL4
#include "OpenGL4Defs.hpp"
#endif

#ifdef USE_DIRECTX
#include "DirectXDefs.hpp"
#endif

template <typename T, typename U>
T as(U thing) {
    return static_cast<T>(thing);
}

namespace {
float getRand(float min, float max) {
    float unit = as<float>(rand()) / RAND_MAX;
    return (max - min) * unit + min;
}
}
