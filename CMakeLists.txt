cmake_minimum_required(VERSION 2.8)

project(BulletTest)

# OpenGL
find_package(OpenGL)
if (OPENGL_FOUND)
    message("Found OpenGL: ${OPENGL_LIBRARIES}")
else (OPENGL_FOUND)
    message("Cannot find OpenGL")
    set(opengl_gl_library opengl32)
    set(opengl_glu_library glu32)
endif (OPENGL_FOUND)

# GLEW
find_package(GLEW REQUIRED)
include_directories("${GLEW_INCLUDE_DIR}")

# GLFW
if (DEFINED GLFW_INCLUDE_DIR AND
    DEFINED GLFW_LIBRARIES)
    message("Found GLFW")
    include_directories("${GLFW_INCLUDE_DIR}")
else()
    if (NOT DEFINED GLFW_INCLUDE_DIR)
        set(GLFW_INCLUDE_DIR "GLFW_INCLUDE_DIR-NOT-FOUND")
    endif()
    if (NOT DEFINED GLFW_LIBRARIES)
        set(GLFW_LIBRARIES "GLFW_LIBRARIES-NOT-FOUND")
    endif()
    message("Unable to find GLFW")
endif()
message("    GLFW_INCLUDE_DIR - ${GLFW_INCLUDE_DIR}")
message("    GLFW_LIBRARIES - ${GLFW_LIBRARIES}")

# GLM
if (DEFINED GLM_INCLUDE_DIR)
    message("Found glm: ${GLM_INCLUDE_DIR}")
    include_directories("${GLM_INCLUDE_DIR}")
else()
    message("Cannot find GLM - GLM_INCLUDE_DIR")
endif()

# tinyobjloader
include_directories("tinyobjloader")

# BulletPhysics
if (DEFINED BULLET_ROOT_DIR)
    include("${BULLET_ROOT_DIR}/lib/cmake/bullet/BulletConfig.cmake")
    include_directories("${BULLET_ROOT_DIR}/include/bullet")
    link_directories("${BULLET_ROOT_DIR}/lib")
else()
    message("Cannot find BulletPhysics' CMAKE dir - BULLET_ROOT_DIR")
endif()

# clang-format
find_program(CLANG_FORMAT
             NAMES
             clang-format
             clang-format-3.6
             clang-format-3.5
             clang-format-3.4
             )
message("ClangFormat: ${CLANG_FORMAT}")

FILE(GLOB HEADER_FILES "*.hpp")
FILE(GLOB SOURCE_FILES "*.cpp")

add_executable(Test
               ${HEADER_FILES}
               ${SOURCE_FILES}

               tinyobjloader/tiny_obj_loader.cc
               tinyobjloader/tiny_obj_loader.h
               )
target_link_libraries(Test
                      "${OPENGL_LIBRARIES}"
                      "${GLUT_LIBRARIES}"
                      "${GLFW_LIBRARIES}"
                      )

foreach(LIB ${BULLET_LIBRARIES})
    target_link_libraries(Test
        debug "${LIB}_Debug"
        optimized "${LIB}"
    )
endforeach()

add_custom_command(TARGET Test
                   PRE_BUILD
                   COMMAND ${CLANG_FORMAT} -i ${HEADER_FILES} ${SOURCE_FILES})
