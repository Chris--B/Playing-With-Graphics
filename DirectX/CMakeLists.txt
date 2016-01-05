# The new policy seems to have issues with"C:\Program Files (x86)\..."
# as a filename.
cmake_policy(SET CMP0053 OLD)
find_package(DirectX)

FILE(GLOB DIRECTX_HEADER_FILES "*.hpp")
FILE(GLOB DIRECTX_SOURCE_FILES "*.cpp")

add_executable(App-DirectX
               ${HEADER_FILES}
               ${SOURCE_FILES}
               ${TINYOBJ_FILES}

               ${DIRECTX_HEADER_FILES}
               ${DIRECTX_SOURCE_FILES})

target_link_libraries(App-DirectX
                      "${DirectX_D3D11_LIBRARY}")

# TODO: Both of these should be macros or something.
# TODO: Only do this for visual studio.
foreach(LIB ${BULLET_LIBRARIES})
    target_link_libraries(App-DirectX
                          debug     "${LIB}_Debug"
                          optimized "${LIB}")
endforeach()

if (USE_CLANGFORMAT)
  add_custom_command(TARGET App-DirectX
                     PRE_BUILD
                     COMMAND ${CLANG_FORMAT}
                        -sort-includes -i ${HEADER_FILES}
                                          ${SOURCE_FILES}
                                          ${DIRECTX_HEADER_FILES}
                                          ${DIRECTX_SOURCE_FILES})
endif()
