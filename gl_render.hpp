// *****************************************************************************************************************************
// glrender.hpp
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

#include "linkedlist.hpp"

#include "vec.hpp"
#include "mat.hpp"

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

	return window;
}

// Set OpenGL Viewport
void RendSetViewport(GLFWwindow* window)
{
	GLint width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);
}

#endif
