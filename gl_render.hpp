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

//#include "soil.h"
#include "lodepng.h"
#include "lodepng.cpp"

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
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES,16);    //Anti-Aliasing

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
GLuint RendLoadTexture(const char* path)
{
	GLuint tex_id;
	unsigned int width, height;
	std::vector<unsigned char> image;
	unsigned int error;

	// Load Image
	error = lodepng::decode(image, width, height, path);
	//if (image)
	if (error == 0)
	{
		// Generate and Bind OpenGL Texture
		glGenTextures(1, &tex_id);
		glBindTexture(GL_TEXTURE_2D, tex_id);

		// TODO: Add arguments to this function if needed to define below parameters
		// lodepng appears to always produce a 4-byte RGBA output?
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Parameters
		// TODO: Are these always applicable?
		// TODO: Check if texture is square and disable MIPMAP if not (also anything else to consider?)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Cleanup
		//SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		return tex_id;
	}
	else
	{
		//fprintf(stderr, "Could not load texture image\n");
		std::cout << "lodepng error " << error << ": " << lodepng_error_text(error) << std::endl;
		return 0;
	}
}

#endif
