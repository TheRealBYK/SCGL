cmake_minimum_required(VERSION 4.4)
project("SCGL" C CXX)

include_directories(${CMAKE_SOURCE_DIR}/glad/include)
file(GLOB BUTTERFLIES_SOURCES_C ${CMAKE_SOURCE_DIR} *.c glad/src/gl.c)

if (WIN32)
    include(FetchContent)
    FetchContent_Declare(
	GLFW
	URL "https://github.com/glfw/glfw/releases/download/3.5.1/glfw-3.5.1.bin.WIN64.zip"
    )
    # set(ZIP_FILE_PATH "${CMAKE_SOURCE_DIR}/glfw-3.5.1.bin.WIN64.zip")
    # file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/GLFW")
    # set(EXTRACT_DIR "${CMAKE_SOURCE_DIR}/GLFW")
    # file(ARCHIVE_EXTRACT INPUT "${ZIP_FILE_PATH}" DESTINATION "${EXTRACT_DIR}")
    FetchContent_MakeAvailable(	GLFW)
endif()

add_subdirectory("${CMAKE_SOURCE_DIR}/src")
