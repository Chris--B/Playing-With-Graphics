cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

project(BulletTest)

option(USE_OPENGL4 "Build a binary using OpenGL4." ON)
option(USE_DIRECTX "Build a binary using DirectX." OFF)
option(USE_CLANGFORMAT "Run ClangFormat on code when building." ON)

if (NOT USE_OPENGL4 AND NOT USE_DIRECTX)
    message(FATAL_ERROR "You must choose at least one graphical backend.")
endif()

# tinyobjloader
include_directories("tinyobjloader")
# TODO: Manage header impl files like this better.
set(TINYOBJ_FILES "${CMAKE_SOURCE_DIR}/tinyobjloader/tiny_obj_loader.cc")

# stb
include_directories("stb")

# BulletPhysics
if (DEFINED BULLET_ROOT_DIR)
    include("${BULLET_ROOT_DIR}/lib/cmake/bullet/BulletConfig.cmake")
    include_directories("${BULLET_ROOT_DIR}/include/bullet")
    link_directories("${BULLET_ROOT_DIR}/lib")
else()
    message("Cannot find BulletPhysics' CMAKE dir - BULLET_ROOT_DIR")
endif()

# clang-format
if (USE_CLANGFORMAT)
    find_program(CLANG_FORMAT
                 NAMES
                 clang-format
                 clang-format-3.8
                 clang-format-3.7)
    if (CLANG_FORMAT MATCHES "CLANG_FORMAT-NOTFOUND")
        message(SEND_ERROR "Unable to find ClangFormat")
    endif()
    message("ClangFormat: ${CLANG_FORMAT}")
endif()

FILE(GLOB HEADER_FILES "*.hpp")
FILE(GLOB SOURCE_FILES "*.cpp")

include_directories("${CMAKE_SOURCE_DIR}")

if (USE_OPENGL4)
    add_subdirectory(OpenGL4)
endif()

if (USE_DIRECTX)
    add_subdirectory(DirectX)
endif()
