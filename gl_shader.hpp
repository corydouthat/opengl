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

	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_TRUE)
		return true;
	else
	{
		std::cout << "Shader program " << program << " failed to link." << std::endl;
		glGetProgramInfoLog(program, 512, NULL, buffer);
		std::cout << buffer << std::endl;
		return false;
	}
}

// User Shader Program
void UseShaderProgram(GLuint shader_prog_id)
{
	glUseProgram(shader_prog_id);

	//// Specify the layout of the vertex data
	//GLint posAttrib = glGetAttribLocation(shader_prog_id, "pos");
	//glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
	//glEnableVertexAttribArray(posAttrib);

	//GLint colAttrib = glGetAttribLocation(shader_prog_id, "color");
	//glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(colAttrib);

	//GLint texAttrib = glGetAttribLocation(shader_prog_id, "texcoord");
	//glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(texAttrib);

	return;
}

// Build Shader Program from vertex shader and fragment shader source
// Return: Shader Program ID created
GLuint BuildShaderProgram(const GLchar* vshd_src, const GLchar* fshd_src)
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
	glLinkProgram(shader_prog_id);
	if (!CheckShaderProgram(shader_prog_id))
		return 0;

	glDeleteShader(vshd_id);
	glDeleteShader(fshd_id);

	return shader_prog_id;
}

// Create Shader Program by loading vector and fragment shader source
// Return: Shader Program ID created
GLuint MakeShaderProgram(const char* vshd_path, const char* fshd_path)
{
	GLuint shader_prog_id = 0;
	unsigned int *vshd_size = 0;	// Not Used
	unsigned int *fshd_size = 0;	// Not used

	// Load text files
	char *vshd_src = getFileContents(vshd_path, vshd_size);
	if (!vshd_src)
	{
		std::cout << "Could not load vertex shader at " << vshd_path << std::endl;
		return 0;
	}
	char *fshd_src = getFileContents(fshd_path, fshd_size);
	if (!fshd_src)
	{
		std::cout << "Could not load fragment shader at " << vshd_path << std::endl;
		return 0;
	}

	// Build Shader program
	shader_prog_id = BuildShaderProgram(vshd_src, fshd_src);
	if (!shader_prog_id)
		return 0;

	return shader_prog_id;
}

#endif
