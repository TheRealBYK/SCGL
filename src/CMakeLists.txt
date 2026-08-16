find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(BYK-Engine "Engine.c" "../glad/src/gl.c" "../glad/src/egl.c")

target_link_libraries(BYK-Engine PRIVATE glfw OpenGL::GL)
