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
#include <stdio.h>
#include "Abstractions.h"
#include "Camera.h"
#include "Shader.h"
#include "ObjectData.h"
#include "cglm/mat4.h"
#include "cglm/types.h"

#define MAX_INSTANCES 16

void RendererInit(GLuint* VAO, GLuint* progID, GLuint* numIndices, ShapeDraw shapeDraw, unsigned short numInstances, GLuint* transformMatrixID, GLuint* shaderTintID)
{
    int version = gladLoadGL(glfwGetProcAddress);
    printf("GLAD: OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version),
    GLAD_VERSION_MINOR(version));
    printf("%s\n", glGetString(GL_VERSION));
    
    glEnable(GL_DEPTH_TEST);
    glGenVertexArrays(1, VAO);
    ObjectData(numIndices, *VAO, shapeDraw, numInstances, transformMatrixID, shaderTintID);
    InstallShaders(progID, SCGL_NONE, "res/shaders/Combo.glsl", "");
}

void RendererClear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererDraw(GLFWwindow *win, GLuint VAO, Camera* cam, mat4 projMat, GLuint progID, GLuint numIndices, float translationVector[][3], float rotAxis[][3], float* deg, unsigned short numInstances, float shaderColor[][4], GLuint transformMatrixID, GLuint shaderTintID)
{
    GET_WINDOW_SIZE(win, cam->width, cam->height);

    glUseProgram(progID);
    glBindVertexArray(VAO);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
    

    mat4 fullTranforms[numInstances];
    vec4 shaderTints[numInstances];

    for (int i = 0; i < numInstances; i++) { 
	CameraUpdate(cam, projMat);

	mat4 worldToViewMatrix;
	GetWorldToViewMatrix(worldToViewMatrix);
	
	mat4 VarMatrix = GLM_MAT4_IDENTITY_INIT;
	mat4* rotationMat = Mat4Rotate(VarMatrix, rotAxis[i], translationVector[i], deg[i]);
	mat4* translationMat = Mat4Translate(*rotationMat, translationVector[i]);
	Mat4Copy(*Mat4Mul(worldToViewMatrix, *translationMat), VarMatrix);
	mat4* fullMatrix = Mat4Mul(projMat, VarMatrix);
	Mat4Copy(*fullMatrix, fullTranforms[i]);
	Vec4Copy(shaderColor[i], shaderTints[i]);
    }

    VertexBufferBind(transformMatrixID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fullTranforms), fullTranforms, GL_DYNAMIC_DRAW);

    VertexBufferUnbind();
    
    VertexBufferBind(shaderTintID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shaderTints), shaderTints, GL_DYNAMIC_DRAW);

    glViewport(0, 0, cam->width, cam->height);
    glDrawElementsInstanced(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0, numInstances);
}
