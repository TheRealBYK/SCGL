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
#include <stdlib.h>
#include <string.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>
#include "Abstractions.h"
#include "Camera.h"
#include "Shapes.h"

#define GET_WINDOW_SIZE(win, w, h) glfwGetFramebufferSize(win, &w, &h)

typedef enum {
    SCGL_NONE = -1,
    SCGL_VERTEX = 0,
    SCGL_FRAGMENT = 1,
} ShaderType;

void SendDataToOpenGL();
ShaderVertFragSource *LoadShaderCode(ShaderVertFragSource *shaderStrBuf, const char *fileName, ShaderType type);
GLuint CompileShader(GLuint type, const char *source);
GLuint CreateShader(const char *vertexShader, const char *fragmentShader);
void InstallShaders();
void InitializeGL();
void PaintGL(GLFWwindow *win, Camera* cam, mat4 projMat);
void MouseMoveEvent(GLFWwindow *win, double xpos, double ypos);

GLuint programID;
GLuint vaoID;
GLuint numIndices;


int main(void) {
    GLFWwindow *window;
  
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
  
    InitializeGL();
  
    vec2 mousePosition;

    Camera camera = {60.0f, 0, 0, 0.1, 10.f};
    mat4 projectionMat = GLM_MAT4_IDENTITY_INIT;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
          MoveForeward();
        } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
          MoveBackward();
        } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
          StrafeLeft();
        } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
          StrafeRight();
        } else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
          MoveUp();
        } else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
          MoveDown();
        }
        /* Render here */
        double mousePositionX, mousePositionY;
        glfwGetCursorPos(window, &mousePositionX, &mousePositionY);
        vec2 mousePosUpdate = {(float)mousePositionX, (float)mousePositionY};
        MouseUpdate(mousePosUpdate);
        PaintGL(window, &camera, projectionMat);
    
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

void SendDataToOpenGL() {
    ShapeData shape;
    CreateCar(&shape);
  
    GLuint vertexID;
    VertexBufferCreate(&vertexID, shape.vertices,
                       sizeof(Vertex) * shape.numVertices);
  
    VertexAttribData(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    VertexAttribData(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                     (char *)(sizeof(float) * 3));
  
    GLuint indexID;
    IndexBufferCreate(&indexID, shape.indices,
                      shape.numIndices * sizeof(unsigned short));
  
    numIndices = shape.numIndices;
    printf("Indices: %d\n", numIndices);
  
    free(shape.vertices);
    shape.vertices = NULL;
    free(shape.indices);
    shape.indices = NULL;
}

ShaderVertFragSource *LoadShaderCode(ShaderVertFragSource *shaderStrBuf,
                                     const char *fileName, ShaderType type) {
    if (type != SCGL_NONE && type != SCGL_VERTEX && type != SCGL_FRAGMENT) {
      printf("Incorrect type detected!\n");
      return shaderStrBuf;
    }
    memset(shaderStrBuf->vertexSource, 0, sizeof(shaderStrBuf->vertexSource));
    memset(shaderStrBuf->fragmentSource, 0, sizeof(shaderStrBuf->fragmentSource));
    char buffer[256] = {0};
    printf("Opening file: %s\n", fileName);
    FILE *pFile = fopen(fileName, "r");
  
    if (pFile == NULL) {
      printf("Error Opening file!");
      return shaderStrBuf;
    }
  
    unsigned int strRemainder = sizeof(buffer) - strlen(buffer) - 1;
  
    if (type == SCGL_NONE) {
    char shaderBuffers[2][1024] = {{0}};
    ShaderType sType = type;
    while (fgets(buffer, sizeof(buffer), pFile) != NULL) 
    {
        if (strncmp(buffer, "#shader", (size_t)7) == 0) {
            if (strncmp(&buffer[8], "vertex", (size_t)6) == 0) {
                sType = SCGL_VERTEX;
            } 
	    else if (strncmp(&buffer[8], "fragment", (size_t)8) == 0) {
                sType = SCGL_FRAGMENT;
            }
	}
	else {
            strncat(shaderBuffers[sType], buffer, strRemainder);
            strRemainder = sizeof(buffer) - strlen(buffer) - 1;
        }
    }
    strncpy(shaderStrBuf->vertexSource, shaderBuffers[0], strlen(shaderBuffers[0]) - 1);
    strncpy(shaderStrBuf->fragmentSource, shaderBuffers[1], strlen(shaderBuffers[1]) - 1);
    shaderStrBuf->vertexSource[strlen(shaderStrBuf->vertexSource) - 1] = '\0';
    } 
    else {
	char shaderBuf[1024] = {0};
        while (fgets(buffer, sizeof(buffer), pFile) != NULL) 
	{
	    strncat(shaderBuf, buffer, strRemainder);
            strRemainder = sizeof(shaderBuf) - strlen(shaderBuf) - 1;
        }
        if (type == SCGL_VERTEX) {
            strncpy(shaderStrBuf->vertexSource, shaderBuf, strlen(shaderBuf));
            strncpy(shaderStrBuf->fragmentSource, " \0", sizeof(" \0"));
        } 
	else if (type == SCGL_FRAGMENT) {
            strncpy(shaderStrBuf->fragmentSource, shaderBuf, sizeof(shaderBuf));
            strncpy(shaderStrBuf->vertexSource, " \0", sizeof(" \0"));
        }
    }
    fclose(pFile);
    printf("Closing file: %s\n", fileName);
    return shaderStrBuf;
}

GLuint CompileShader(GLuint type, const char *source) {
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);
    
    int status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
	int length;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
	char *message = (char *)alloca(length * sizeof(char));
	glGetShaderInfoLog(id, length, &length, message);
	
	printf("Failed to compile %s shader",
	((type == GL_VERTEX_SHADER) ? "vertex" : "fragment"));
	printf("%s\n", message);
	
	glDeleteShader(id);
	return 0;
    }
    
    return id;
}
GLuint CreateShader(const char *vertexShader, const char *fragmentShader) {
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

void InstallShaders() {
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    ShaderVertFragSource shaderSource;
    LoadShaderCode(&shaderSource, "res/shaders/Combo.glsl", SCGL_NONE);
    
    const GLchar *vertStr = shaderSource.vertexSource;
    const GLchar *fragStr = shaderSource.fragmentSource;
    
    printf("VERT:\n%s\n", shaderSource.vertexSource);
    printf("FRAG:\n%s\n", shaderSource.fragmentSource);
    
    programID = CreateShader(vertStr, fragStr);
    
    glUseProgram(programID);
}

void InitializeGL() {
    int version = gladLoadGL(glfwGetProcAddress);
    printf("GLAD: OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version),
    GLAD_VERSION_MINOR(version));
    printf("%s\n", glGetString(GL_VERSION));
    
    glEnable(GL_DEPTH_TEST);
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    SendDataToOpenGL();
    InstallShaders();
}

void PaintGL(GLFWwindow *win, Camera* cam, mat4 projMat) {
    GET_WINDOW_SIZE(win, cam->width, cam->height);

    CameraUpdate(cam, projMat);
    mat4 VarMatrix = GLM_MAT4_IDENTITY_INIT;
    mat4* rotationMat = Mat4Rotate(VarMatrix, (vec3){1.0f, 0.0f, 0.0f}, (vec3){0.0f, 0.0f, -3.0f}, 0.0f);
    mat4* translationMat = Mat4Translate(*rotationMat, (vec3){0.0f, 0.0f, -3.0f});
    mat4 worldToViewMatrix;
    GetWorldToViewMatrix(worldToViewMatrix);
    Mat4Copy(*Mat4Mul(worldToViewMatrix, *translationMat), VarMatrix);
    mat4* fullMatrix = Mat4Mul(projMat, VarMatrix);

    UniformMat4f("fullMatrix", programID, 1, GL_FALSE, *fullMatrix);
    
    glBindVertexArray(vaoID);
    // IndexBufferBind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, cam->width, cam->height);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
}
