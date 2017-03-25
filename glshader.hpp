// *****************************************************************************************************************************
// glshader.hpp
// OpenGL Rendering
// GLSL shader code strings
// Author: Cory Douthat
// Copyright (c) 2017 Cory Douthat, All Rights Reserved.
// *****************************************************************************************************************************

#ifndef GL_SHADER_HPP_
#define GL_SHADER_HPP_

#include <iostream>

#include "GL\glew.h"

#define GLFW_DLL
#define GLFW_INCLUDE_GLU
#include "GLFW\glfw3.h"


// Check shader compile
bool CheckShaderCompile(GLuint shader)
{
    GLint status;
    char buffer[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE)
        return true;
    else
    {
        std::cout << "Shader " << shader << " failed to compile." << std::endl;
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        std::cout << buffer << std::endl;
        return false;
    }
}

// Check shader program link
bool CheckShaderProgram(GLuint program)
{
    GLint status;
    char buffer[512];

    glGetProgramiv(program,GL_LINK_STATUS,&status);
    if (status == GL_TRUE)
        return true;
    else
    {
        std::cout << "Shader program " << program << " failed to link." << std::endl;
        glGetProgramInfoLog(program,512,NULL,buffer);
        std::cout << buffer << std::endl;
        return false;
    }
}

void UseShader(GLuint shader_prog_id)
{
    glUseProgram(shader_prog_id);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shader_prog_id,"pos");
    glVertexAttribPointer(posAttrib,2,GL_FLOAT,GL_FALSE,7 * sizeof(GLfloat),0);
    glEnableVertexAttribArray(posAttrib);

    GLint colAttrib = glGetAttribLocation(shader_prog_id,"color");
    glVertexAttribPointer(colAttrib,3,GL_FLOAT,GL_FALSE,7 * sizeof(GLfloat),(void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(colAttrib);

    GLint texAttrib = glGetAttribLocation(shader_prog_id,"texcoord");
    glVertexAttribPointer(texAttrib,2,GL_FLOAT,GL_FALSE,7 * sizeof(GLfloat),(void*)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(texAttrib);

    // TODO: TEMP Disable textures
    glUniform1ui(glGetUniformLocation(shader_prog_id,"use_textures"),0);

    return;
}

GLuint InitShaderProgram(const GLchar* vshd_src, const GLchar* fshd_src)
{
	GLuint vshd_id, fshd_id;
	GLuint shader_prog_id = 0;

    // Compile Vertex Shader
    // **************
    vshd_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshd_id, 1, &vshd_src, NULL);
    glCompileShader(vshd_id);
    if (!CheckShaderCompile(vshd_id))
        return 0;


    // Compile Fragment Shader
    // **************
    fshd_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshd_id, 1, &fshd_src, NULL);
    glCompileShader(fshd_id);
	if (!CheckShaderCompile(fshd_id))
		return 0;


    // Link Shader
    // **************
    shader_prog_id = glCreateProgram();
    glAttachShader(shader_prog_id, vshd_id);
    glAttachShader(shader_prog_id, fshd_id);
    glBindFragDataLocation(shader_prog_id,0,"color_out");
    glLinkProgram(shader_prog_id);
    if (!CheckShaderProgram(shader_prog_id))
        return 0;

	return shader_prog_id;
}

#endif
