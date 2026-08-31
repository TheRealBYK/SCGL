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

typedef struct
{
    float x, y, z;
    float r, g, b, a;
}Vertex;

typedef struct
{
    Vertex* vertices;
    int numVertices;
    unsigned short* indices;
    int numIndices;
}ShapeData;

int ArrayAllocation(int vertexCount, int indexCount, ShapeData* data)
{
    data->vertices = (Vertex*)calloc(vertexCount, sizeof(Vertex));
    if (data->vertices == NULL)
    {
	printf("%s : %s :Memory initialization failed!", __FILE__, __LINE__);
	return 1;
    }
    data->indices = (unsigned short*)calloc(indexCount, sizeof(unsigned short));
    if (data->indices == NULL)
    {
	printf("%s : %s :Memory initialization failed!", __FILE__, __LINE__);
	return 1;
    }
    return 0;
}

void CreateTriangle(ShapeData* inData)
{
    Vertex verts[] = {
	{+0.0f, +0.75f, -1.0f,
	+0.0f, +1.0f, +0.0f, +1.0f},
	{+0.75f, -0.75f, -1.0f,
	+1.0f, +0.0f, +0.0f,+1.0f},	
	{-0.75f, -0.75f, -1.0f,
	+0.0f, +0.0f, +1.0f, +1.0f}
    };

    unsigned short indices[] = {0,  1,  2};


    inData->numVertices = sizeof(verts) / sizeof(Vertex);
    inData->numIndices = sizeof(indices) / sizeof(unsigned short);

    ArrayAllocation(inData->numVertices, inData->numIndices, inData);

    memcpy(inData->vertices, verts, sizeof(verts));
    memcpy(inData->indices, indices, sizeof(indices));
}

void CreateCube(ShapeData* inData)
{
    Vertex verts[24] = {
	{-1.0f, +1.0f, +1.0f, // 0
        +1.0f, +0.0f, +0.0f, +1.0}, // Colour
        {+1.0f, +1.0f, +1.0f, // 1
        +0.0f, +1.0f, +0.0f, +1.0}, // Colour
        {+1.0f, +1.0f, -1.0f, // 2
        +0.0f, +0.0f, +1.0f, +1.0}, // Colour
	{-1.0f, +1.0f, -1.0f, // 3
        +1.0f, +1.0f, +1.0f, +1.0}, // Colour

        {-1.0f, +1.0f, -1.0f, // 4
        +1.0f, +0.0f, +1.0f, +1.0}, // Colour
	{+1.0f, +1.0f, -1.0f, // 5
        +0.0f, +0.5f, +0.2f, +1.0}, // Colour
	{+1.0f, -1.0f, -1.0f, // 6
        +0.8f, +0.6f, +0.4f, +1.0}, // Colour
	{-1.0f, -1.0f, -1.0f, // 7
        +0.3f, +1.0f, +0.5f, +1.0}, // Colour

        {+1.0f, +1.0f, -1.0f, // 8
        +0.2f, +0.5f, +0.2f, +1.0}, // Colour
        {+1.0f, +1.0f, +1.0f, // 9
        +0.9f, +0.3f, +0.7f, +1.0}, // Colour
	{+1.0f, -1.0f, +1.0f, // 10
        +0.3f, +0.7f, +0.5f, +1.0}, // Colour
	{+1.0f, -1.0f, -1.0f, // 11
        +0.5f, +0.7f, +0.5f, +1.0}, // Colour

	{-1.0f, +1.0f, +1.0f, // 12
        +0.7f, +0.8f, +0.2f, +1.0}, // Colour
	{-1.0f, +1.0f, -1.0f, // 13
        +0.5f, +0.7f, +0.3f, +1.0}, // Colour
	{-1.0f, -1.0f, -1.0f, // 14
        +0.4f, +0.7f, +0.7f, +1.0}, // Colour
	{-1.0f, -1.0f, +1.0f, // 15
        +0.2f, +0.5f, +1.0f, +1.0}, // Colour

	{+1.0f, +1.0f, +1.0f, // 16
        +0.6f, +1.0f, +0.7f, +1.0}, // Colour
	{-1.0f, +1.0f, +1.0f, // 17
        +0.6f, +0.4f, +0.8f, +1.0}, // Colour
	{-1.0f, -1.0f, +1.0f, // 18
        +0.2f, +0.8f, +0.7f, +1.0}, // Colour
	{+1.0f, -1.0f, +1.0f, // 19
        +0.2f, +0.7f, +1.0f, +1.0}, // Colour

        {+1.0f, -1.0f, -1.0f, // 20
        +0.8f, +0.3f, +0.7f, +1.0}, // Colour
	{-1.0f, -1.0f, -1.0f, // 21
        +0.8f, +0.9f, +0.5f, +1.0}, // Colour
	{-1.0f, -1.0f, +1.0f, // 22
        +0.5f, +0.8f, +0.5f, +1.0}, // Colour
	{+1.0f, -1.0f, +1.0f, // 23
        +0.9f, +1.0f, +0.2f, +1.0}, // Colour
    };

    unsigned short indices[] = {
	    0,  1,  2, 0,  2,  3,  // Top
            4,  5,  6,  4,  6,  7, // Front
            8,  9, 10,  8, 10, 11, // Right
           12, 13, 14, 12, 14, 15, // Left
           16, 17, 18, 16, 18, 19, // Back
           20, 22, 21, 20, 23, 22, // Bottom
    };


    inData->numVertices = sizeof(verts) / sizeof(Vertex);
    inData->numIndices = sizeof(indices) / sizeof(unsigned short);

    ArrayAllocation(inData->numVertices, inData->numIndices, inData);

    memcpy(inData->vertices, verts, sizeof(verts));
    memcpy(inData->indices, indices, sizeof(indices));
}

void CreateCar(ShapeData* inData)
{
    Vertex verts[58] = {
	{+0.25f, -0.0625625f, -0.75f,     // 0
	+0.8f, +1.0f, +0.8f, +1.0f},
	{-0.25f, -0.0625625f, -0.75f,     // 1
	+0.0f, +1.0f, +0.0f, +1.0f},
	{-0.25f, +0.125f, -0.75f,     // 2
	+0.8f, +1.0f, +0.8f, +1.0f},
	{+0.25f, +0.125f, -0.75f,     // 3
	+0.0f, +1.0f, +0.0f, +1.0f},

	{+0.25f, +0.125f, -0.75f,     // 4
	+0.8f, +1.0f, +0.8f, +1.0f},
	{-0.25f, +0.125f, -0.75f,     // 5
	+0.8f, +1.0f, +0.8f, +1.0f},
	{-0.25f, +0.125f, -0.45f,     // 6
	+0.2f, +1.0f, +0.1f, +1.0f},
	{+0.25f, +0.125f, -0.45f,     // 7
	+0.2f, +1.0f, +0.1f, +1.0f},

	{+0.25f, -0.0625625f, -0.45f,     // 8
	+0.2f, +1.0f, +0.3f, +1.0f},
	{+0.25f, -0.0625625f, -0.75f,     // 9
	+0.2f, +1.0f, +0.3f, +1.0f},
	{+0.25f, +0.125f, -0.75f,     // 10
	+0.7f, +1.0f, +0.7f, +1.0f},
	{+0.25f, +0.125f, -0.45f,     // 11
	+0.7f, +1.0f, +0.7f, +1.0f},

	{-0.25f, -0.0625f, -0.75f,     // 12
	+0.4f, +1.0f, +0.3f, +1.0f},
	{-0.25f, -0.0625f, -0.45f,     // 13
	+0.4f, +1.0f, +0.3f, +1.0f},
	{-0.25f, +0.125f, -0.45f,     // 14
	+0.7f, +1.0f, +0.7f, +1.0f},
	{-0.25f, +0.125f, -0.75f,     // 15
	+0.7f, +1.0f, +0.7f, +1.0f},

	{+0.25f, +0.125f, -0.45f,     // 16
	+0.7f, +0.7f, +0.7f, +1.0f},
	{-0.25f, +0.125f, -0.45f,     // 17
	+0.3f, +0.6f, +0.9f, +1.0f},
	{-0.25f, +0.375f, -0.25f,     // 18
	+0.7f, +0.7f, +0.7f, +1.0f},
	{+0.25f, +0.375f, -0.25f,     // 19
	+0.3f, +0.6f, +0.9f, +1.0f},

	{-0.25f, +0.125f, +0.0f,      // 20
	+0.4f, +1.0f, +1.0f, +1.0f},
	{-0.25f, +0.125f, -0.45f,     // 21
	+0.4f, +1.0f, +1.0f, +1.0f},
	{-0.25f, +0.375f, -0.25f,     // 22
	+0.4f, +0.7f, +0.5f, +1.0f},
	{-0.25f, +0.375f, +0.15f,     // 23
	+0.4f, +1.0f, +0.5f, +1.0f},

	{+0.25f, +0.125f, +0.0f,      // 24
	+0.4f, +1.0f, +1.0f, +1.0f},
	{+0.25f, +0.125f, -0.45f,     // 25
	+0.4f, +1.0f, +1.0f, +1.0f},
	{+0.25f, +0.375f, -0.25f,     // 26
	+0.4f, +1.0f, +0.5f, +1.0f},
	{+0.25f, +0.375f, +0.15f,     // 27
	+0.4f, +1.0f, +0.5f, +1.0f},

	{-0.25f, +0.125f, +0.25f,     // 28
	+0.4f, +1.0f, +1.0f, +1.0f},
	{-0.25f, +0.125f, +0.0f,      // 29
	+0.4f, +1.0f, +1.0f, +1.0f},
	{-0.25f, +0.375f, +0.15f,     // 30
	+0.4f, +0.7f, +1.0f, +1.0f},

	{+0.25f, +0.125f, +0.25f,     // 31
	+0.4f, +1.0f, +1.0f, +1.0f},
	{+0.25f, +0.125f, +0.0f,      // 32
	+0.4f, +1.0f, +1.0f, +1.0f},
	{+0.25f, +0.375f, +0.15f,     // 33
	+0.4f, +0.7f, +1.0f, +1.0f},

	{+0.25f, +0.375f, -0.25f,     // 34
	+0.6f, +1.0f, +0.5f, +1.0f},
	{-0.25f, +0.375f, -0.25f,     // 35
	+0.6f, +1.0f, +0.5f, +1.0f},
	{-0.25f, +0.375f, +0.15f,     // 36
	+0.6f, +1.0f, +0.5f, +1.0f},
	{+0.25f, +0.375f, +0.15f,     // 37
	+0.6f, +1.0f, +0.5f, +1.0f},

	{+0.25f, +0.375f, +0.15f,     // 38
	+0.7f, +0.7f, +0.7f, +1.0f},
	{-0.25f, +0.375f, +0.15f,     // 39
	+0.3f, +0.6f, +0.9f, +1.0f},
	{-0.25f, +0.125f, +0.25f,     // 40
	+0.7f, +0.7f, +0.7f, +1.0f},
	{+0.25f, +0.125f, +0.25f,     // 41
	+0.3f, +0.6f, +0.9f, +1.0f},

	{+0.25f, -0.0625f, +0.25f,     // 42
	+0.6f, +1.0f, +0.7f, +1.0f},
	{+0.25f, -0.0625f, -0.45f,     // 43
	+0.6f, +1.0f, +0.7f, +1.0f},
	{+0.25f, +0.125f, -0.45f,     // 44
	+0.75f, +1.0f, +0.75f, +1.0f},
	{+0.25f, +0.125f, +0.25f,     // 45
	+0.75f, +1.0f, +0.75f, +1.0f},

	{-0.25f, -0.0625f, +0.25f,     // 46
	+0.6f, +1.0f, +0.7f, +1.0f},
	{-0.25f, -0.0625f, -0.45f,     // 47
	+0.6f, +1.0f, +0.7f, +1.0f},
	{-0.25f, +0.125f, -0.45f,     // 48
	+0.75f, +1.0f, +0.75f, +1.0f},
	{-0.25f, +0.125f, +0.25f,     // 49
	+0.75f, +1.0f, +0.75f, +1.0f},

	{-0.25f, -0.0625f, +0.25f,     // 50
	+0.8f, +1.0f, +0.7f, +1.0f},
	{+0.25f, -0.0625f, +0.25f,     // 51
	+0.9f, +1.0f, +0.9f, +1.0f},
	{+0.25f, +0.125f, +0.25f,     // 52
	+0.8f, +1.0f, +0.7f, +1.0f},
	{-0.25f, +0.125f, +0.25f,     // 53
	+0.9f, +1.0f, +0.9f, +1.0f},

	{-0.25f, -0.0625f, +0.25f,     // 54
	+1.0f, +1.0f, +1.0f, +1.0f},
	{+0.25f, -0.0625f, +0.25f,     // 55
	+1.0f, +1.0f, +1.0f, +1.0f},
	{+0.25f, -0.0625f, -0.75f,     // 56
	+1.0f, +1.0f, +1.0f, +1.0f},
	{-0.25f, -0.0625f, -0.75f,     // 57
	+1.0f, +1.0f, +1.0f, +1.0f},
    };

    unsigned short indices[] = {
	    0,  1,  2, 0,  2,  3,
            4,  5,  6,  4,  6,  7,
            8,  9, 10,  8, 10, 11,
           12, 13, 14, 12, 14, 15,
           16, 17, 18, 16, 18, 19,
           20, 21, 22, 20, 22, 23,
	   24, 25, 26, 24, 26, 27,
	   28, 29, 30, 31, 32, 33,
	   34, 35, 36, 34, 36, 37,
	   38, 39, 40, 38, 40, 41,
	   42, 43, 44, 42, 44, 45,
	   46, 47, 48, 46, 48, 49,
	   50, 51, 52, 50, 52, 53,
	   54, 55, 56, 54, 56, 57,
    };


    inData->numVertices = sizeof(verts) / sizeof(Vertex);
    inData->numIndices = sizeof(indices) / sizeof(unsigned short);

    ArrayAllocation(inData->numVertices, inData->numIndices, inData);

    memcpy(inData->vertices, verts, sizeof(verts));
    memcpy(inData->indices, indices, sizeof(indices));
}
