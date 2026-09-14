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
#include <time.h>

void HandleInput(GLFWwindow *win, double* posX, double* posY, bool* menuShow, int* pressState, bool isOnWindow, float* NuklearX, float* NuklearY, clock_t* lastTime)
{
    if (glfwGetKey(win, GLFW_KEY_M) == GLFW_PRESS && *pressState == GLFW_RELEASE) {
	clock_t currTime = clock();
	if ((((double)(currTime - *lastTime) / CLOCKS_PER_SEC) * 1000.0f) > 25)
	{
	    *menuShow = !(*menuShow);
	    *NuklearX = (float)(*posX);
	    *NuklearY = (float)(*posY);
	    *lastTime = currTime;
	}
    }
    
    glfwGetCursorPos(win, posX, posY);
    if (!isOnWindow)
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

	vec2 mousePosUpdate = {*posX, *posY};
	MouseUpdate(mousePosUpdate);
    }
}
