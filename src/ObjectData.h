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

typedef enum {
    DRAW_TRIANGLE = 0,
    DRAW_CUBE = 1,
    DRAW_PYRAMID = 2,
    DRAW_CAR = 3,
}ShapeDraw;

void ObjectData(GLuint* numIndices, ShapeDraw shapeDraw) {
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
  
    GLuint vertexID;
    VertexBufferCreate(&vertexID, shape.vertices, sizeof(Vertex) * shape.numVertices);
  
    VertexAttribData(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    VertexAttribData(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char *)(sizeof(float) * 3));
  
    GLuint indexID;
    IndexBufferCreate(&indexID, shape.indices, shape.numIndices * sizeof(unsigned short));
  
    *numIndices = shape.numIndices;
    printf("Indices: %d\n", *numIndices);
  
    free(shape.vertices);
    shape.vertices = NULL;
    free(shape.indices);
    shape.indices = NULL;
}
