// Copyright (c) 2026 BYK. All Rights Reserved.
//
// This file is part of SCGL.
//
// SCGL is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, under version 2 of the License.
//
// SCGL is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// SCGL. If not, see <https://www.gnu.org/licenses/>.
//
// Maintainer: BYK <bykdev@proton.me>

#pragma once

#include "Camera.h"
#include <GLFW/glfw3.h>


void HandleInput(GLFWwindow *win, double* posX, double* posY)
{
    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
      MoveForeward();
    } else if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
      MoveBackward();
    } else if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
      StrafeLeft();
    } else if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
      StrafeRight();
    } else if (glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS) {
      MoveUp();
    } else if (glfwGetKey(win, GLFW_KEY_F) == GLFW_PRESS) {
      MoveDown();
    }

    glfwGetCursorPos(win, posX, posY);
    vec2 mousePosUpdate = {(float)*posX, (float)*posY};
    MouseUpdate(mousePosUpdate);
}
