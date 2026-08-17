#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "glad/gl.h"
#include <GLFW/glfw3.h>
#include <nuklear.h>
#include "Shapes.h"

void sendDataToOpenGL();
const char* LoadShaderCode(char* shaderStrBuf, const char* fileName);
void installShaders();
bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType);
bool checkShaderStatus(GLuint shaderID);
bool checkProgramStatus(GLuint progID);
GLuint programID;

void initializeGL();

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    initializeGL();

    GLsizei width, height;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
    
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

void sendDataToOpenGL()
{
    Vertex verts[3];
    createTriangle(verts);

    GLuint vertexID;
    glGenBuffers(1, &vertexID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char *)(sizeof(float) * 3));

    GLushort indices[] = {0,1,2,};

    GLuint indexID;
    glGenBuffers(1, &indexID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

const char* LoadShaderCode(char* shaderStrBuf, const char* fileName)
{
    char buffer[256] = {0};
    printf("Opening file: %s\n", fileName);
    FILE* pFile = fopen(fileName, "r");

    if (pFile == NULL){
	printf("Error Opening file!");
	return NULL;
    }

    unsigned int strRemainder = sizeof(buffer) - strlen(buffer) - 1;

    while (fgets(buffer, sizeof(buffer), pFile) != NULL) {
	strncat(shaderStrBuf, buffer, strRemainder);
	strRemainder = sizeof(buffer) - strlen(buffer) - 1;
    }
    fclose(pFile);
    printf("Closing file: %s\n", fileName);
    const char* interShader = shaderStrBuf;
    return interShader;
}

void installShaders()
{
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLchar adapter[2][1024] = {{0}};
    
    const GLchar* vertexCode = LoadShaderCode(adapter[0], "src/shaders/VertexShader.glsl");
    glShaderSource(vertexShaderID, 1, &vertexCode, 0);
    const GLchar* fragmentCode = LoadShaderCode(adapter[1], "src/shaders/FragmentShader.glsl");
    glShaderSource(fragmentShaderID, 1, &fragmentCode, 0);

    glCompileShader(vertexShaderID);
    glCompileShader(fragmentShaderID);

    if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID)) return;

    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    if (!checkProgramStatus(programID)) return;

    printf("Deleting vertex shader.\n");
    glDeleteShader(vertexShaderID);
    printf("Deleting fragment shader.\n");
    glDeleteShader(fragmentShaderID);

    glUseProgram(programID);
}

bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType)
{
    GLint status;
    objectPropertyGetterFunc(objectID, statusType, &status);
    
    if (status != GL_TRUE)
    {
	GLint infoLogLength;
	objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
	GLchar buffer[infoLogLength];

	memset(buffer, 0, infoLogLength);

	GLsizei bufferSize;
	getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);

	printf("OpenGL: %s\n", buffer);

	return false;
    }
    return true;
}

bool checkShaderStatus(GLuint shaderID)
{
    return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint progID)
{
    return checkStatus(progID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

void initializeGL()
{
    int version = gladLoadGL(glfwGetProcAddress);
    printf("GLAD: OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    printf("%s\n", glGetString(GL_VERSION));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_DEPTH_TEST);
    sendDataToOpenGL();
    installShaders();
}
