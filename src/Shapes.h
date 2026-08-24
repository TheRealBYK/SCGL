#pragma once

typedef struct
{
    float x, y, z;
    float r, g, b, a;
}Vertex;

Vertex* createTriangle(Vertex arr[3])
{
    for (int i = 0; i < 3; i++)
    {
	switch (i) {
	    case 0:
		arr[i].x = +0.0f;
		arr[i].y = +0.75f;
		arr[i].z = -1.0f;
		arr[i].r = +0.0f;
		arr[i].g = +1.0f;
		arr[i].b = +0.0f;
		arr[i].a = +1.0f;
		break;
	    case 1:
		arr[i].x = +0.75f;
		arr[i].y = -0.75f;
		arr[i].z = -1.0f;
		arr[i].r = +1.0f;
		arr[i].g = +0.0f;
		arr[i].b = +0.0f;
		arr[i].a = +1.0f;
		break;
	    case 2:
		arr[i].x = -0.75f;
		arr[i].y = -0.75f;
		arr[i].z = -1.0f;
		arr[i].r = +0.0f;
		arr[i].g = +0.0f;
		arr[i].b = +1.0f;
		arr[i].a = +1.0f;
		break;
	    default:	
	        break;
	}
    }
    
    return arr;
}

Vertex* createCube(Vertex inArr[24])
{
    Vertex arr[24] = {
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
    
    memcpy(inArr, arr, sizeof(arr));

    return inArr;
}
