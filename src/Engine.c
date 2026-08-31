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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glad/gl.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>
#include <GLFW/glfw3.h>
#include "Shapes.h"
#include "Camera.h"
#include "Abstractions.h"

#define GET_WINDOW_SIZE(win, w, h) glfwGetFramebufferSize(win, &w, &h)

typedef enum {
    SCGL_NONE = -1,
    SCGL_VERTEX = 0,
    SCGL_FRAGMENT = 1,
}ShaderType;

typedef struct {
    char vertexSource[1024];
    char fragmentSource[1024];
}ShaderVertFragSource;

void SendDataToOpenGL();
ShaderVertFragSource* LoadShaderCode(ShaderVertFragSource* shaderStrBuf, const char* fileName, ShaderType type);
GLuint CompileShader(GLuint type, const char* source);
GLuint CreateShader(const char* vertexShader, const char* fragmentShader);
void InstallShaders();
void InitializeGL();
void PaintGL(GLFWwindow* win);
void MouseMoveEvent(GLFWwindow* win, double xpos, double ypos);

GLuint programID;
GLuint vaoID;
GLuint numIndices;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "SCGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    glfwSwapInterval(1);

    InitializeGL();

    vec2 mousePosition;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
	    MoveForeward();
	}
	else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
	    MoveBackward();
	}
	else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
	    StrafeLeft();
	}
	else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
	    StrafeRight();
	}
	else if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
	    MoveUp();
	}
	else if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
	    MoveDown();
	}
        /* Render here */
	double mousePositionX, mousePositionY;
	glfwGetCursorPos(window, &mousePositionX, &mousePositionY);
	vec2 mousePosUpdate = {(float)mousePositionX, (float)mousePositionY};
	MouseUpdate(mousePosUpdate);
	PaintGL(window);
    
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glUseProgram(0);

    printf("Deleting program and exiting.\n");    
    glDeleteProgram(programID);
    printf("Exited correctly.\n");

    glfwTerminate();
    return 0;
}

void SendDataToOpenGL()
{
    ShapeData shape;
    CreateCube(&shape);

    GLuint vertexID;
    VertexBufferCreate(&vertexID, shape.vertices, sizeof(Vertex) * shape.numVertices);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char *)(sizeof(float) * 3));

    GLuint indexID;
    IndexBufferCreate(&indexID, shape.indices, shape.numIndices * sizeof(unsigned short));

    numIndices = shape.numIndices;
    printf("Indices: %d\n", numIndices);

    free(shape.vertices);
    shape.vertices = NULL;
    free(shape.indices);
    shape.indices = NULL;
}

ShaderVertFragSource* LoadShaderCode(ShaderVertFragSource* shaderStrBuf, const char* fileName, ShaderType type)
{
    if (type != SCGL_NONE && type != SCGL_VERTEX && type != SCGL_FRAGMENT)
    {
        printf("Incorrect type detected!\n");
	return shaderStrBuf;
    }
    memset(shaderStrBuf->vertexSource, 0, sizeof(shaderStrBuf->vertexSource));
    memset(shaderStrBuf->fragmentSource, 0, sizeof(shaderStrBuf->fragmentSource));
    char buffer[256] = {0};
    printf("Opening file: %s\n", fileName);
    FILE* pFile = fopen(fileName, "r");

    if (pFile == NULL){
	printf("Error Opening file!");
	return shaderStrBuf;
    }

    unsigned int strRemainder = sizeof(buffer) - strlen(buffer) - 1;

    if (type == SCGL_NONE)
    {
	char shaderBuffers[2][1024] = {{0}};
	ShaderType sType = type;
	while (fgets(buffer, sizeof(buffer), pFile) != NULL) {
	    if (strncmp(buffer, "#shader", (size_t)7) == 0)
	    {
		if (strncmp(&buffer[8], "vertex", (size_t)6) == 0)
		{
		    sType = SCGL_VERTEX;
		}
		else if (strncmp(&buffer[8], "fragment", (size_t)8) == 0)
		{ 
		    sType = SCGL_FRAGMENT;
		}
	    }
	    else 
	    {
		strncat(shaderBuffers[sType], buffer, strRemainder);
		strRemainder = sizeof(buffer) - strlen(buffer) - 1;
	    }
	}
	strncpy(shaderStrBuf->vertexSource, shaderBuffers[0], strlen(shaderBuffers[0]) - 1);
	strncpy(shaderStrBuf->fragmentSource, shaderBuffers[1], strlen(shaderBuffers[1]) - 1);
	shaderStrBuf->vertexSource[strlen(shaderStrBuf->vertexSource) - 1] = '\0';
    }
    else
    {
	char shaderBuf[1024] = {0};
	while (fgets(buffer, sizeof(buffer), pFile) != NULL) {
	    strncat(shaderBuf, buffer, strRemainder);
	    strRemainder = sizeof(shaderBuf) - strlen(shaderBuf) - 1;
	}
	if (type == SCGL_VERTEX)
	{
	    strncpy(shaderStrBuf->vertexSource, shaderBuf, strlen(shaderBuf));
	    strncpy(shaderStrBuf->fragmentSource, " \0", sizeof(" \0"));
	}
	else if (type == SCGL_FRAGMENT)
	{
	    strncpy(shaderStrBuf->fragmentSource, shaderBuf, sizeof(shaderBuf));
	    strncpy(shaderStrBuf->vertexSource, " \0", sizeof(" \0"));
	}
    }
    fclose(pFile);
    printf("Closing file: %s\n", fileName);
    return shaderStrBuf;
}

GLuint CompileShader(GLuint type, const char* source)
{
    GLuint id = glCreateShader(type); 
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    int status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
	int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        printf("Failed to compile %s shader", ((type == GL_VERTEX_SHADER) ? "vertex": "fragment"));
        printf("%s\n", message);

        glDeleteShader(id);
        return 0;
    }

    return id;
}
GLuint CreateShader(const char* vertexShader, const char* fragmentShader)
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    printf("Deleting vertex shader.\n");
    glDeleteShader(vs);
    printf("Deleting fragment shader.\n");
    glDeleteShader(fs);

    return program;
}

void InstallShaders()
{
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    ShaderVertFragSource shaderSource;
    LoadShaderCode(&shaderSource, "res/shaders/Combo.glsl", SCGL_NONE);

    const GLchar* vertStr = shaderSource.vertexSource;
    const GLchar* fragStr = shaderSource.fragmentSource;

    printf("VERT:\n%s\n", shaderSource.vertexSource);
    printf("FRAG:\n%s\n", shaderSource.fragmentSource);


    programID = CreateShader(vertStr, fragStr);

    glUseProgram(programID);
}

void InitializeGL()
{
    int version = gladLoadGL(glfwGetProcAddress);
    printf("GLAD: OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    printf("%s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST);
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    SendDataToOpenGL();
    InstallShaders();
}

void PaintGL(GLFWwindow* win)
{
    int width, height;
    GET_WINDOW_SIZE(win, width, height);
    
    mat4 projectionMat;
    glm_perspective(glm_rad(60.0f), ((float)width) / (float)height, 0.1f, 10.0f, projectionMat);
    mat4 rotationMat;
    cglm_rotate((mat4){{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}, (vec3){0.0f, 1.0f, 0.0f}, (vec3){0.0f, 0.0f, -3.0f}, 180.0f, rotationMat);
    mat4 translationMat;
    glm_translate_to(rotationMat, (vec3){0.0f, 0.0f, -3.0f}, translationMat);
    mat4 worldToViewMatrix;
    mat4 intermediateMatrix;
    GetWorldToViewMatrix(worldToViewMatrix);
    glm_mat4_mul(worldToViewMatrix, translationMat, intermediateMatrix);
    mat4 fullMatrix;
    glm_mat4_mul(projectionMat, intermediateMatrix, fullMatrix);

    GLint fullMatrixUniformLocation = glGetUniformLocation(programID, "fullMatrix");
    glUniformMatrix4fv(fullMatrixUniformLocation, 1, GL_FALSE, (const GLfloat *)fullMatrix);

    glBindVertexArray(vaoID);
    // IndexBufferBind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
}
