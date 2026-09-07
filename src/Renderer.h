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

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Abstractions.h"
#include "Camera.h"
#include "Shader.h"
#include "ObjectData.h"

void RendererInit(GLuint* VAO, GLuint* progID, GLuint* numIndices, ShapeDraw shapeDraw)
{
    int version = gladLoadGL(glfwGetProcAddress);
    printf("GLAD: OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version),
    GLAD_VERSION_MINOR(version));
    printf("%s\n", glGetString(GL_VERSION));
    
    glEnable(GL_DEPTH_TEST);
    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);
    ObjectData(numIndices, shapeDraw);
    InstallShaders(progID, SCGL_NONE, "res/shaders/Combo.glsl", "");

}

void RendererClear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererDraw(GLFWwindow *win, GLuint VAO, Camera* cam, mat4 projMat, GLuint progID, GLuint numIndices)
{
    GET_WINDOW_SIZE(win, cam->width, cam->height);

    CameraUpdate(cam, projMat);
    mat4 VarMatrix = GLM_MAT4_IDENTITY_INIT;
    mat4* rotationMat = Mat4Rotate(VarMatrix, (vec3){1.0f, 0.0f, 0.0f}, (vec3){0.0f, 0.0f, -3.0f}, 0.0f);
    mat4* translationMat = Mat4Translate(*rotationMat, (vec3){0.0f, 0.0f, -3.0f});
    mat4 worldToViewMatrix;
    GetWorldToViewMatrix(worldToViewMatrix);
    Mat4Copy(*Mat4Mul(worldToViewMatrix, *translationMat), VarMatrix);
    mat4* fullMatrix = Mat4Mul(projMat, VarMatrix);

    UniformMat4f("u_FullMatrix", progID, 1, GL_FALSE, *fullMatrix);
    
    glBindVertexArray(VAO);

    RendererClear();
    glViewport(0, 0, cam->width, cam->height);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
}
