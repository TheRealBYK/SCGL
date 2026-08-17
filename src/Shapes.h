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
		arr[i].z = +0.0f;
		arr[i].r = +0.0f;
		arr[i].g = +1.0f;
		arr[i].b = +0.0f;
		arr[i].a = +1.0f;
		break;
	    case 1:
		arr[i].x = +0.75f;
		arr[i].y = -0.75f;
		arr[i].z = +0.0f;
		arr[i].r = +1.0f;
		arr[i].g = +0.0f;
		arr[i].b = +0.0f;
		arr[i].a = +1.0f;
		break;
	    case 2:
		arr[i].x = -0.75f;
		arr[i].y = -0.75f;
		arr[i].z = +0.0f;
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
