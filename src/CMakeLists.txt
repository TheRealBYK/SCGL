# Copyright (c) 2026 BYK. All Rights Reserved.
#
# This file is part of SCGL.
#
# SCGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, under version 2 of the License.
#
# SCGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with SCGL. If not, see <https://www.gnu.org/licenses/>.
#
# Maintainer: BYK <bykdev@proton.me>

find_package(OpenGL REQUIRED)

add_executable(SCGL "Engine.c" "${CMAKE_SOURCE_DIR}/dependencies/src/gl.c")

if(WIN32)
    target_link_directories(SCGL PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/lib/GLFW/")  
    target_link_libraries(SCGL PRIVATE glfw3 OpenGL::GL)
else()
    find_package(glfw3 REQUIRED)
    target_link_libraries(SCGL PRIVATE glfw OpenGL::GL m)
endif()

