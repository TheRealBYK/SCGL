#pragma once

// Shape Structs.
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

// Camera Struct.
typedef struct
{
    float fov;
    int width, height;
    float nearPlane, farPlane;
} Camera;

// Shader Struct.
typedef struct {
    char vertexSource[1024];
    char fragmentSource[1024];
} ShaderVertFragSource;
