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

#include <stdio.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>
#include "Abstractions.h"
#include "Input.h"
#include "ObjectData.h"
#include "Renderer.h"

GLuint programID;
GLuint vaoID;
GLuint numIndices;


int main(void) {
    GLFWwindow *window;
  
    /* Initialize the library */
    if (!glfwInit())
      return -1;
  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "SCGL", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
  
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
  
    glfwSwapInterval(1);
  
    RendererInit(&vaoID, &programID, &numIndices, DRAW_CUBE);
  
    vec2 mousePosition;

    Camera camera = {60.0f, 0, 0, 0.1, 10.f};
    mat4 projectionMat;
    Mat4Identify(projectionMat);

    double mousePositionX, mousePositionY;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
	HandleInput(window, &mousePositionX, &mousePositionY);
        RendererDraw(window, vaoID, &camera, projectionMat, programID, numIndices);
    
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    
        /* Poll for and process events */
        glfwPollEvents();
    }
  
    ShaderUnbind();
  
    printf("Deleting program and exiting.\n");
    ShaderDelete(programID);
    printf("Exited correctly.\n");
  
    glfwTerminate();
    return 0;
}
