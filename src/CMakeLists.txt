find_package(OpenGL REQUIRED)

add_executable(SCGL "Engine.c" "${CMAKE_SOURCE_DIR}/dependencies/src/gl.c")

if(WIN32)
    target_link_libraries(SCGL PRIVATE glfw OpenGL::GL "${CMAKE_SOURCE_DIR}/dependencies/lib/GLFW/glfw3dll.lib")
else()
    find_package(glfw3 REQUIRED)
    target_link_libraries(SCGL PRIVATE glfw OpenGL::GL m)
endif()

