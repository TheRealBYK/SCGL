#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

void SendDataToOpenGL();
ShaderVertFragSource* LoadShaderCode(ShaderVertFragSource* shaderStrBuf, const char* fileName, ShaderType type);
GLuint CompileShader(GLuint type, const char* source);
GLuint CreateShader(const char* vertexShader, const char* fragmentShader);
void InstallShaders();
void InitializeGL();

GLuint programID;

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

    InitializeGL();

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

void SendDataToOpenGL()
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

    GLushort indices[] = {0,  1,  2,};

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
   
    glClear(GL_COLOR_BUFFER_BIT);
    SendDataToOpenGL();
    InstallShaders();
}
