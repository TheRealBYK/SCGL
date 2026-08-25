find_package(OpenGL REQUIRED)

add_executable(SCGL "Engine.c" "${CMAKE_SOURCE_DIR}/dependencies/src/gl.c")

if(WIN32)
    target_link_directories(SCGL PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/lib/GLFW/")  
    target_link_libraries(SCGL PRIVATE glfw3 OpenGL::GL)
else()
    find_package(glfw3 REQUIRED)
    target_link_libraries(SCGL PRIVATE glfw OpenGL::GL m)
endif()

