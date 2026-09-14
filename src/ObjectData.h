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

#include "Abstractions.h"
#include "Structs.h"
#include "Shapes.h"
#include "cglm/types.h"

typedef enum {
    DRAW_TRIANGLE = 0,
    DRAW_CUBE = 1,
    DRAW_PYRAMID = 2,
    DRAW_CAR = 3,
}ShapeDraw;

void ObjectData(GLuint* numIndices, GLuint VAO, ShapeDraw shapeDraw, unsigned short numInstances, GLuint* transformMatID, GLuint* tintVecID) {
    ShapeData shape;
    switch (shapeDraw) {
	case DRAW_TRIANGLE:
	    CreateTriangle(&shape);
	    break;
	case DRAW_CUBE:
	    CreateCube(&shape);
	    break;
	case DRAW_PYRAMID:
	    CreatePyramid(&shape);
	    break;
	case DRAW_CAR:
	    CreateCar(&shape);
	    break;
	default:
	    printf("Invalid shape entered!\n");
    }
  
    glBindVertexArray(VAO);

    GLuint vertexID;
    VertexBufferCreate(&vertexID, shape.vertices, sizeof(Vertex) * shape.numVertices, GL_STATIC_DRAW, true);
  
    VertexAttribData(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 0));
    VertexAttribData(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));

    VertexBufferCreate(transformMatID, 0, sizeof(mat4) * numInstances, GL_DYNAMIC_DRAW, true);

    VertexAttribData(2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 0));
    VertexAttribData(3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 4));
    VertexAttribData(4, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 8));
    VertexAttribData(5, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 12));

    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    VertexBufferCreate(tintVecID, 0, sizeof(vec4) * numInstances, GL_DYNAMIC_DRAW, true);
    VertexAttribData(6, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), (void*)(sizeof(float) * 0));
    
    glVertexAttribDivisor(6, 1);
  
    GLuint indexID;
    IndexBufferCreate(&indexID, shape.indices, shape.numIndices * sizeof(unsigned short));
  
    *numIndices = shape.numIndices;
    printf("Indices: %d\n", *numIndices);
  
    free(shape.vertices);
    shape.vertices = NULL;
    free(shape.indices);
    shape.indices = NULL;

}
