// *****************************************************************************************************************************
// gl_render.hpp
// OpenGL Rendering
// Graphics setup and helper functions
// Author: Cory Douthat
// Copyright (c) 2017 Cory Douthat, All Rights Reserved.
// *****************************************************************************************************************************

#ifndef GL_RENDER_HPP
#define GL_RENDER_HPP

#include <iostream>
#include <cstdio>
#include <vector>

#define GLEW_STATIC
#include <GL\glew.h>

#define GLFW_DLL
#define GLFW_INCLUDE_GLU
#include "GLFW\glfw3.h"

#include "file_help.hpp"

#include "linked_list.hpp"

#include "vec.hpp"
#include "mat.hpp"

#include "soil.h"

//#include "gl_shader.hpp"

void RendSetViewport(GLFWwindow* window);

// Initialize OpenGL render context
// Return: GLFW window id created
GLFWwindow* RendInit(const char *name, unsigned int width = 1024, unsigned int height = 768)
{
	GLFWwindow* window = nullptr;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// GLFW
	window = glfwCreateWindow(width, height, name, nullptr, nullptr);
	if (window == nullptr)
	{
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	// GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW/n");
		glfwTerminate();
		return nullptr;
	}

	// OpenGL Viewport
	RendSetViewport(window);

	// Configure OpenGL
	glEnable(GL_DEPTH_TEST);

	return window;
}

// Set OpenGL Viewport
void RendSetViewport(GLFWwindow* window)
{
	GLint width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);
}

// Load Texture
// Return: Texture ID created
GLuint RendLoadTexture(GLuint shd_prog, const char* path)
{
	// TODO: Switch to another image loader that does not invert y-axis.
	// Unofficial OpenGL SDK or DevIL

	GLuint tex_id;
	int width, height;
	unsigned char* image;

	// Load Image
	image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	if (image)
	{
		// Generate and Bind OpenGL Texture
		glGenTextures(1, &tex_id);
		glBindTexture(GL_TEXTURE_2D, tex_id);

		// TODO: Add arguments to this function if needed to define below parameters
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Cleanup
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		return tex_id;
	}
	else
	{
		fprintf(stderr, "Could not load texture image\n");
		
		return 0;
	}
}

#endif
