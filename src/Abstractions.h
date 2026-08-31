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

void cglm_rotate(mat4 matrixRotationTrnaslation, vec3 rotationAxis, vec3 rotationPivot, float angles, mat4 outputMatrix)
{
    glm_rotate_at(matrixRotationTrnaslation, rotationPivot, glm_rad(angles), rotationAxis);
    glm_mat4_copy(matrixRotationTrnaslation, outputMatrix);
}
