#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "glad/gl.h"
#include <GLFW/glfw3.h>
#include "Shapes.h"

typedef enum {
    SCGL_NONE = -1,
    SCGL_VERTEX = 0,
    SCGL_FRAGMENT = 1,
}ShaderType;

typedef struct {
    char vertexSource[1024];
    char fragmentSource[1024];
}ShaderVertFragSource;

void sendDataToOpenGL();
ShaderVertFragSource* LoadShaderCode(ShaderVertFragSource* shaderStrBuf, const char* fileName, ShaderType type);
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
	ShaderType sType;
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
	    // shaderStrBuf->vertexSource[strlen(shaderStrBuf->vertexSource) - 1] = '\0';
	}
	else if (type == SCGL_FRAGMENT)
	{
	    strncpy(shaderStrBuf->fragmentSource, shaderBuf, sizeof(shaderBuf));
	    strncpy(shaderStrBuf->vertexSource, " \0", sizeof(" \0"));
	    // shaderStrBuf->fragmentSource[strlen(shaderStrBuf->fragmentSource) - 1] = '\0';
	}
    }
    fclose(pFile);
    printf("Closing file: %s\n", fileName);
    return shaderStrBuf;
}

void installShaders()
{
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLchar adapter[2][1024] = {{0}};
   
    ShaderVertFragSource shaderSource;
    LoadShaderCode(&shaderSource, "res/shaders/Combo.glsl", SCGL_NONE);

    printf("VERT:\n%s\n", shaderSource.vertexSource);
    const GLchar* vertStr = shaderSource.vertexSource;
    const GLchar* vertShader = vertStr;
    glShaderSource(vertexShaderID, 1, &vertShader, 0);

    printf("FRAG:\n%s\n", shaderSource.fragmentSource);
    const GLchar* fragStr = shaderSource.fragmentSource;
    const GLchar* fragShader = fragStr;
    glShaderSource(fragmentShaderID, 1, &fragShader, 0);

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
