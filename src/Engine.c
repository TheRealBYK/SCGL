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

#include <stdio.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#include <nuklear/nuklear.h>
#include <nuklear/nuklear_glfw_gl3.h>
#include "nuklear/style.c"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#include "Abstractions.h"
#include "Input.h"
#include "ObjectData.h"
#include "Renderer.h"
#include "NuklearUI.h"

GLuint programID;
GLuint vaoID;
GLuint numIndices;


int main(void) {
    unsigned short instances = 8;
    struct nk_glfw glfw = {0};
    GLFWwindow *window;
    struct nk_context* ctx;
    struct nk_colorf bg;

    #ifdef INCLUDE_CONFIGURATOR
	static struct nk_color color_table[NK_COLOR_COUNT];
	memcpy(color_table, nk_default_color_style, sizeof(color_table));
    #endif

    if (instances > MAX_INSTANCES)
    {
	fprintf(stderr, "%d is over the allowed maximum amount of instances\nMax instances: %d\n", instances, MAX_INSTANCES);
	return -1;
    }

    /* Initialize the library */
    if (!glfwInit())
      return -1;
  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "SCGL", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
  
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
  
    glfwSwapInterval(1);
 

    if (!gladLoadGL(glfwGetProcAddress)) {
	fprintf(stderr, "Failed to initialize GLAD\n");
	return -1;
    }


    GLuint transformMatrixID;
    GLuint colorTintID;
    RendererInit(&vaoID, &programID, &numIndices, DRAW_CAR, instances, &transformMatrixID, &colorTintID);

    struct nk_font_atlas *atlas;

    if(NuklearInit(window, &glfw, &ctx, &bg, atlas) != 0) 
    {
	fprintf(stderr, "NuklearInit() FAILED\n");
	return -1;
    }
  
    vec2 mousePosition;

    Camera camera = {60.0f, 0, 0, 0.1, 10.f};
    mat4 projectionMat;
    Mat4Identify(projectionMat);

    vec3 translationVec[MAX_INSTANCES] = 
    {
	{+7.25f, +0.0f, -0.75f}, //  0
	{+7.2f, +0.0f, -1.75f},  //  1
	{+6.95f, +0.0f, -2.65f}, //  2
	{+6.5f, +0.0f, -3.55f},  //  3
	{+5.95f, +0.0f, -4.35f}, //  4
	{+5.2f, +0.0f, -5.05f},  //  5
	{+4.35f, +0.0f, -5.6f},  //  6
	{+3.4f, +0.0f, -6.05f},  //  7
	{+0.0f, +0.0f, -3.0f},   //  8
	{-0.9f, +0.0f, -3.0f},   //  9
	{-1.8f, +0.0f, -3.0f},   // 10
	{-2.7f, +0.0f, -3.0f},   // 11
	{-3.6f, +0.0f, -3.0f},   // 12
	{-4.5f, +0.0f, -3.0f},   // 13
	{-5.4f, +0.0f, -3.0f},   // 14
	{-6.3f, +0.0f, -3.0f},   // 15
    };
    vec3 rotationAxis[MAX_INSTANCES] = 
    {
	{+0.0f, +0.37f, +0.0f}, // 0
	{+0.0f, +0.37f, +0.0f}, // 1
	{+0.0f, +0.37f, +0.0f}, // 2
	{+0.0f, +0.37f, +0.0f}, // 3
	{+0.0f, +0.37f, +0.0f}, // 4
	{+0.0f, +0.37f, +0.0f}, // 5
	{+0.0f, +0.37f, +0.0f}, // 6
	{+0.0f, +0.37f, +0.0f}, // 7
	{+0.0f, +0.37f, +0.0f}, // 8
	{+0.0f, +0.37f, +0.0f}, // 9
	{+0.0f, +0.37f, +0.0f}, // 10
	{+0.0f, +0.37f, +0.0f}, // 11
	{+0.0f, +0.37f, +0.0f}, // 12
	{+0.0f, +0.37f, +0.0f}, // 13
	{+0.0f, +0.37f, +0.0f}, // 14
	{+0.0f, +0.37f, +0.0f}, // 15
    };
    float AngleDegree[MAX_INSTANCES] = {90.0f, 100.0f, 110.0f, 120.0f, 130.0f, 140.0f, 150.0f, 160.0f, 144.0f, 144.0f, 144.0f, 144.0f, 144.0f, 144.0f, 144.0f, 144.0f};
    vec4 colorTint[MAX_INSTANCES] = 
    {
	{+1.0f, +1.0f, +1.0f, +0.5f},    //  0 - White
	{+0.0f, +1.0f, +1.0f, +0.5f},    //  1 - Aquamarine
	{+1.0f, +0.0f, +1.0f, +0.5f},    //  2 - Magenta
	{+0.0f, +0.0f, +1.0f, +0.5f},    //  3 - Blue
	{+1.0f, +1.0f, +0.0f, +0.5f},    //  4 - Yellow
	{+0.0f, +1.0f, +0.0f, +0.5f},    //  5 - Lime
	{+1.0f, +0.0f, +0.0f, +0.5f},    //  6 - Red
	{+0.5f, +0.5f, +0.5f, +0.5f},    //  7 - Dark-Gray
	{+0.75f, +0.75f, +0.75f, +0.5f}, //  8 - Light-Gray
	{+0.0f, +0.5f, +0.5f, +0.5f},    //  9 - Cyan
	{+0.5f, +0.0f, +0.5f, +0.5f},    // 10 - Purple
	{+0.0f, +0.0f, +0.5f, +0.5f},    // 11 - Navy-Blue
	{+0.5f, +0.5f, +0.0f, +0.5f},    // 12 - Olive
	{+0.0f, +0.5f, +0.0f, +0.5f},    // 13 - Green
	{+0.5f, +0.0f, +0.0f, +0.5f},    // 14 - Maroon
	{+0.0f, +0.0f, +0.0f, +0.5f}     // 15 - Black
    };

    bool showMenu = false;
    int wasMenuOpen = 0;
    int buttonState = GLFW_RELEASE;

    double mousePositionX = 0.0f;
    double mousePositionY = 0.0f;
    float nuklearPositionX = 0.0f;
    float nuklearPositionY = 0.0f;
    clock_t pressTime = 0;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) { 
        /* Poll for and process events */
        glfwPollEvents();
	RendererClear();
        /* Render here */
	nk_glfw3_new_frame(&glfw);

	HandleInput(window, &mousePositionX, &mousePositionY, &showMenu, &buttonState, nk_window_is_any_hovered(ctx), &nuklearPositionX, &nuklearPositionY, &pressTime);
	
	
	
	if (showMenu && !wasMenuOpen)
	{
	    nuklearPositionX = (float)mousePositionX;
	    nuklearPositionY = (float)mousePositionY; 
	    nk_window_set_position(ctx, "Shape Control.", nk_vec2(nuklearPositionX / 1.5, nuklearPositionY - 100));
	}
	wasMenuOpen = showMenu;

	if (showMenu)
	{ 
	    NuklearWidget(ctx, instances, nuklearPositionX, nuklearPositionY, translationVec, rotationAxis, AngleDegree);

	    glClearColor(bg.r, bg.g, bg.b, bg.a);
	    RendererDraw(window, vaoID, &camera, projectionMat, programID, numIndices * 2, translationVec, rotationAxis, AngleDegree, instances, colorTint, transformMatrixID, colorTintID);
	    nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
	}
	else { 
	    glClearColor(bg.r, bg.g, bg.b, bg.a);
	    RendererDraw(window, vaoID, &camera, projectionMat, programID, numIndices * 2, translationVec, rotationAxis, AngleDegree, instances, colorTint, transformMatrixID, colorTintID);
	}
    
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }
  
    ShaderUnbind();
  
    printf("Deleting program and exiting.\n");
    ShaderDelete(programID);
    printf("Exited correctly.\n");

    nk_glfw3_shutdown(&glfw);

    glfwTerminate();
    return 0;
}
