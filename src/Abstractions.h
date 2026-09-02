// Copyright (c) 2026 BYK. All Rights Reserved.
//
// This file is part of SCGL.
//
// SCGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, under version 2 of the License.
//
// SCGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with SCGL. If not, see <https://www.gnu.org/licenses/>.
//
// Maintainer: BYK <bykdev@proton.me>

#pragma once
#include <glad/gl.h>
#include <cglm/cglm.h>
#include <stdbool.h>
#include "Structs.h"

// OpenGL
void VertexBufferBind(GLuint bufferID)
{
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
}

void VertexBufferUnbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferCreate(GLuint* bufferID ,const void* data, GLuint size)
{
    glGenBuffers(1, bufferID);
    VertexBufferBind(*bufferID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBufferCleanup(GLuint bufferID)
{
    glDeleteBuffers(1, &bufferID);
}

void VertexAttribData(unsigned int index, unsigned int elementCount, GLenum type, bool normalize, unsigned int elementSize, const void* offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, elementCount, type, normalize, elementSize, offset);
}

void IndexBufferBind(GLuint bufferID)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
}

void IndexBufferUnbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBufferCreate(GLuint* bufferID ,const unsigned short* data, GLuint count)
{
    glGenBuffers(1, bufferID);
    IndexBufferBind(*bufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}

void IndexBufferCleanup(GLuint bufferID)
{
    glDeleteBuffers(1, &bufferID);
}

void UniformMat4f(const char* uniformName, GLuint progID, unsigned int count, GLboolean trnaspose, mat4 data)
{  
    GLint UniformLocation = glGetUniformLocation(progID, uniformName);
    if (UniformLocation == -1)
    {
	printf("Uniform \"%s\" not found!", uniformName);
	return;
    }

    glUniformMatrix4fv(UniformLocation, count, trnaspose, (const GLfloat *)data);
}

// CGLM

mat4* Mat4Rotate(mat4 matrixRotationTrnaslation, vec3 rotationAxis, vec3 rotationPivot, float angles)
{
    glm_rotate_at(matrixRotationTrnaslation, rotationPivot, glm_rad(angles), rotationAxis);
    return (mat4*)matrixRotationTrnaslation;
}

mat4* Mat4Translate(mat4 matrix, vec3 vector)
{
    glm_translate_to(matrix, vector, matrix);
    return (mat4*)matrix;
}

mat4* Mat4Identify(mat4 matToInit)
{
    glm_mat4_identity(matToInit);
}

mat4* Mat4Mul(mat4 m2, mat4 m1)
{ 
    glm_mat4_mul(m2, m1, m2);
    return (mat4*)m2;
}

void Mat4Copy(mat4 m1, mat4 m2)
{
    glm_mat4_copy(m1, m2);
}

void CameraInit(mat4 projMat)
{ 
    glm_perspective(glm_rad(60.0f), 1920.0f / 1080.0f, 0.1f, 10.f, projMat);
}

void CameraUpdate(Camera* cam, mat4 projMat)
{
    glm_perspective(glm_rad(cam->fov), ((float)cam->width) / (float)cam->height, cam->nearPlane, cam->farPlane, projMat);
}
