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

#include "cglm/types.h"
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

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#include "Abstractions.h"
#include "Input.h"
#include "ObjectData.h"
#include "Renderer.h"

GLuint programID;
GLuint vaoID;
GLuint numIndices;


int main(void) {
    struct nk_glfw glfw = {0};
    GLFWwindow *window;
    struct nk_context* ctx;
    struct nk_colorf bg;

    #ifdef INCLUDE_CONFIGURATOR
	static struct nk_color color_table[NK_COLOR_COUNT];
	memcpy(color_table, nk_default_color_style, sizeof(color_table));
    #endif
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
    RendererInit(&vaoID, &programID, &numIndices, DRAW_PYRAMID);

    ctx = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);

    printf("returned ctx: %p\n", (void *)ctx);
    printf("&glfw.ctx:   %p\n", (void *)&glfw.ctx);
    printf("ctx = %p\n", (void *)ctx);

    if (ctx == NULL) {
	fprintf(stderr, "nk_glfw3_init() FAILED\n");
	return -1;
    }
    {
	struct nk_font_atlas *atlas;
	nk_glfw3_font_stash_begin(&glfw, &atlas);
	nk_glfw3_font_stash_end(&glfw);
    }

    bg.r = 0.2f, bg.g = 0.2f, bg.b = 0.2f, bg.a = 1.0f;
  
    vec2 mousePosition;

    Camera camera = {60.0f, 0, 0, 0.1, 10.f};
    mat4 projectionMat;
    Mat4Identify(projectionMat);

    vec3 translationVec = {0.0f, 0.0f, -3.0f};
    vec3 rotationAxis = {1.0f, 0.0f, 0.0f};
    float AngleDegree = 54.0f;

    double mousePositionX, mousePositionY;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
	nk_glfw3_new_frame(&glfw);

	if (!nk_window_is_any_hovered(ctx))
	{
	    HandleInput(window, &mousePositionX, &mousePositionY);
	    printf("Handle\n");
	}
	else
	{
	    printf("unhandled\n");
	}

	if (nk_begin(ctx,
		     "Shape Control.",
		     nk_rect(mousePositionX, mousePositionY, 500, 230),
		     NK_WINDOW_BORDER |
		     NK_WINDOW_MOVABLE |
		     NK_WINDOW_SCALABLE |
		     NK_WINDOW_MINIMIZABLE |
		     NK_WINDOW_TITLE))
	{
	    nk_layout_row_dynamic(ctx, 100, 1);
	    if (nk_group_begin(ctx, "row1", NK_WINDOW_BORDER))
	    {
		nk_layout_row_dynamic(ctx, 10, 1);
		nk_label(ctx, "Translation.", NK_TEXT_LEFT);
		nk_layout_row_dynamic(ctx, 15, 3);
		nk_property_float(ctx, "X:", -10.0, &translationVec[0], 10.0f, 1.0f, 1);
		nk_property_float(ctx, "Y:", -10.0, &translationVec[1], 10.0f, 1.0f, 1);
		nk_property_float(ctx, "Z:", -10.0, &translationVec[2], 10.0f, 1.0f, 1);

		nk_group_end(ctx);
	    }
	    if (nk_group_begin(ctx, "row2", NK_WINDOW_BORDER))
	    {
		nk_layout_row_dynamic(ctx, 10, 1);
		nk_label(ctx, "Rotation.", NK_TEXT_LEFT);
		nk_layout_row_dynamic(ctx, 15, 3);
		nk_property_float(ctx, "X:", -10.0, &rotationAxis[0], 10.0f, 1.0f, 1);
		nk_property_float(ctx, "Y:", -10.0, &rotationAxis[1], 10.0f, 1.0f, 1);
		nk_property_float(ctx, "Z:", -10.0, &rotationAxis[2], 10.0f, 1.0f, 1);
		nk_layout_row_dynamic(ctx, 15, 1);
		nk_property_float(ctx, "Deg:", -360.0f, &AngleDegree, 360.0f, 1.0f, 100);

		nk_group_end(ctx);
	    }
	}
	nk_end(ctx);	

	glClearColor(bg.r, bg.g, bg.b, bg.a);
        RendererDraw(window, vaoID, &camera, projectionMat, programID, numIndices, translationVec, rotationAxis, AngleDegree);
	nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
    
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    
        /* Poll for and process events */
        glfwPollEvents();
    }
  
    ShaderUnbind();
  
    printf("Deleting program and exiting.\n");
    ShaderDelete(programID);
    printf("Exited correctly.\n");

    nk_glfw3_shutdown(&glfw);

    glfwTerminate();
    return 0;
}
