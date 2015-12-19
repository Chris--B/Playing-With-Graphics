#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"

#define GLEW_STATIC
#include "GL/glew.h"
#include "GL/freeglut.h"

#include "btBulletCollisionCommon.h"

namespace {

glm::vec3 bt2glm(const btVector3 &bt_vec) {
    return glm::vec3(bt_vec.getX(), bt_vec.getY(), bt_vec.getZ());
}

btVector3 glm2bt(const glm::vec3 &glm_vec) {
    return btVector3(glm_vec.x, glm_vec.y, glm_vec.z);
}
}
