# OpenGL
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    message("Found OpenGL: ${OPENGL_LIBRARIES}")
else()
    message("Cannot find OpenGL")
    set(opengl_gl_library opengl32)
    set(opengl_glu_library glu32)
endif()

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
message("    GLFW_LIBRARIES   - ${GLFW_LIBRARIES}")

# GLM
if (DEFINED GLM_INCLUDE_DIR)
    message("Found glm: ${GLM_INCLUDE_DIR}")
    include_directories("${GLM_INCLUDE_DIR}")
else()
    message("Cannot find GLM - GLM_INCLUDE_DIR")
endif()

FILE(GLOB OPENGL4_HEADER_FILES "*.hpp")
FILE(GLOB OPENGL4_SOURCE_FILES "*.cpp")

add_definitions("-DUSE_OPENGL4")

add_executable(App-OpenGL4
               ${HEADER_FILES}
               ${SOURCE_FILES}
               ${TINYOBJ_FILES}

               ${OPENGL4_HEADER_FILES}
               ${OPENGL4_SOURCE_FILES})

target_link_libraries(App-OpenGL4
                      "${OPENGL_LIBRARIES}"
                      "${GLEW_LIBRARIES}"
                      "${GLFW_LIBRARIES}")

# TODO: Both of these should be macros or something.
# TODO: Only do this for visual studio.
foreach(LIB ${BULLET_LIBRARIES})
  target_link_libraries(App-OpenGL4
                        debug     "${LIB}_Debug"
                        optimized "${LIB}")
endforeach()

if (USE_CLANGFORMAT)
  add_custom_command(TARGET App-OpenGL4
                     PRE_BUILD
                     COMMAND ${CLANG_FORMAT}
                        -sort-includes -i ${HEADER_FILES}
                                          ${SOURCE_FILES}
                                          ${OPENGL4_HEADER_FILES}
                                          ${OPENGL4_SOURCE_FILES})
endif()
