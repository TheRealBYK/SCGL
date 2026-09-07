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

#pragma once

#include <string.h>
#include "Abstractions.h"

typedef enum {
    SCGL_NONE = -1,
    SCGL_VERTEX = 0,
    SCGL_FRAGMENT = 1,
} ShaderType;

ShaderVertFragSource *LoadShaderCode(ShaderVertFragSource *shaderStrBuf, const char *fileName, ShaderType type)
{
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

void ShaderDelete(GLuint shaderID)
{
    glDeleteProgram(shaderID);
}

void ShaderBind(GLuint shaderID)
{
    glUseProgram(shaderID);
}

void ShaderUnbind()
{
    glUseProgram(0);
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

void InstallShaders(GLuint* progID, ShaderType type, const char* vertPath, const char* fragPath) {
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    ShaderVertFragSource shaderSource;

    switch (type)
    {
	case SCGL_NONE:
	    if (vertPath[0] == '\0')
	    {
		printf("Error: No file-path provided!\n");
	    }
	    else
	    {		
		LoadShaderCode(&shaderSource, vertPath, type);
	    }
	    break;
	case SCGL_VERTEX:
	    if (vertPath[0] == '\0')
	    {
		printf("Error: No file-path provided!\n");
	    }
	    else
	    {		
		LoadShaderCode(&shaderSource, vertPath, type);
	    }
	    break;
	case SCGL_FRAGMENT: 
	    if (fragPath[0] == '\0')
	    {
		printf("Error: No file-path provided!\n");
	    }
	    else
	    {		
		LoadShaderCode(&shaderSource, fragPath, type);
	    }
	    break;
    }

    
    const GLchar *vertStr = shaderSource.vertexSource;
    const GLchar *fragStr = shaderSource.fragmentSource;
    
    printf("VERT:\n%s\n", shaderSource.vertexSource);
    printf("FRAG:\n%s\n", shaderSource.fragmentSource);
    
    *progID = CreateShader(vertStr, fragStr);
    
    ShaderBind(*progID);
}
