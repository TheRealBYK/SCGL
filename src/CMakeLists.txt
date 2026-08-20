find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(SCGL "Engine.c" "${CMAKE_SOURCE_DIR}/glad/src/gl.c")

target_link_libraries(SCGL PRIVATE glfw OpenGL::GL)
