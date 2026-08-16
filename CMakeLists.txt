cmake_minimum_required(VERSION 4.4)
project("BYK-Engine" C CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glad/include)
file(GLOB BUTTERFLIES_SOURCES_C ${CMAKE_CURRENT_SOURCE_DIR} *.c glad/src/gl.c)

add_subdirectory("./src")
