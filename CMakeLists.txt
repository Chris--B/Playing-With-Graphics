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

# GLUT
find_package(GLUT REQUIRED)
include_directories("${GLUT_INCLUDE_DIR}")

# GLEW
find_package(GLEW REQUIRED)
include_directories("${GLEW_INCLUDE_DIR}")

# GLM
if (DEFINED GLM_INCLUDE_DIR)
    message("Found glm: ${GLM_INCLUDE_DIR}")
    include_directories("${GLM_INCLUDE_DIR}")
else()
    message("Cannot find GLM - GLM_INCLUDE_DIR")
endif()

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
               )
target_link_libraries(Test
                      "${OPENGL_LIBRARIES}"
                      "${GLUT_LIBRARIES}"
                      "${GLEW_LIBRARIES}"
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
