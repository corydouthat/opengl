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

#include "glshader.hpp"

void GLFWErrorHandler(int, const char*);
char *getFileContents(const char*, unsigned int*);
void ResizeWindow(GLFWwindow*, GLsizei, GLsizei);

class RenderContext
{
private:
    GLFWwindow *window = nullptr;	// GLFW Window
    Mat4f model, projection, view;	// model, projection, and view matrices (TODO: UNNECESSARY?)
    std::vector<GLuint> shd_progs;	// List of shader program IDs
    int i_cur_shd_prog = -1;		// Local index of current shader program

public:
	~RenderContext();

    bool init(unsigned int width = 1024, unsigned int height = 768);
	void setWindowRefreshCallback(GLFWwindowrefreshfun r);
	bool checkWindow() { return !glfwWindowShouldClose(window);};
    int addShaderProgram(const char* vshd_path, const char* fshd_path);
    void useShaderProgram(unsigned int i);
	Mat4f calcProjPersp(float fov, float clip_near, float clip_far);
    void swapBuffers() {glfwSwapBuffers(window);};

    // Get and Set Functions
	GLFWwindow* getWindow() { return window; };
    Mat4f getModelMat() {return model;};
	GLint getCurShaderProgram();
    void setModelMat(Mat4f model_in);
    void setModelMat(Vec2f pos,float rot = 0.0f);
    //void setProjection();
    //void setView(Vec2f pos,float zoom);
};


// Render Context IMPLEMENTATION

// Initial Window and OpenGL parameters
bool RenderContext::init(unsigned int width, unsigned int height)
{
    // GLFW
    glfwInit();

    // GLFW Window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE,GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES,16);    //Anti-Aliasing
    window = glfwCreateWindow(width,height,"Space",nullptr,nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(GLFWErrorHandler);
    glfwSetWindowSizeCallback(window,ResizeWindow);

    // GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW." << std::endl;
        // TODO: Trying this out. Might switch everything to stderr
        fprintf(stderr, "Failed to initialize GLEW.\n");
        return false;
    }

    // OpenGL
    glViewport(0,0,width,height);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	// TODO: Update m, v, and p matrices if applicable. May not be appropriate here.
    //if (cur_shd_prog >= 0)
    //{
    //    // Projection
    //    setProjection();

    //    // View
    //    setView(Vec2f(0.0f, 0.0f),1.0f);

    //    // Model
    //    setModelMat(Vec2f(0.0f,0.0f),0.0f);
    //}

    // GLFW Options
    // glfwSwapInterval(0);    // Disable VSYNC

    return true;
}

// Set window refresh callback function
void RenderContext::setWindowRefreshCallback(GLFWwindowrefreshfun r)
{
	// Assign RenderContext pointer to window user pointer
	glfwSetWindowUserPointer(window, this);

	// Set callback
	glfwSetWindowRefreshCallback(window, r);
}

// Add shader program
// Return: index of shader program in RenderContext list
int RenderContext::addShaderProgram(const char* vshd_path, const char* fshd_path)
{
    GLuint shader_prog_id = 0;
	unsigned int *vshd_size = 0;
	unsigned int *fshd_size = 0;	// Not used

	// Load text files
	char *vshd_src = getFileContents(vshd_path, vshd_size);
    if (!vshd_src)
    {
        std::cout << "Could not load vertex shader at " << vshd_path << std::endl;
        return -1;
    }
	char *fshd_src = getFileContents(fshd_path, fshd_size);
    if (!fshd_src)
    {
        std::cout << "Could not load fragment shader at " << vshd_path << std::endl;
        return -1;
    }

    // Initialize Shader program
	shader_prog_id = InitShaderProgram(vshd_src, fshd_src);
    if (!shader_prog_id)
        return false;

    // Save to shd_progs
    shd_progs.push_back(shader_prog_id);

    // If no current shader, use this one
    if (i_cur_shd_prog < 0)
        useShaderProgram(shd_progs.size() - 1);

    // TODO: Init proj, model mat, view??

	return shd_progs.size() - 1;

}

// Use shader program
void RenderContext::useShaderProgram(unsigned int i)
{
    if (i < shd_progs.size() && i != i_cur_shd_prog)
    {
        UseShader(shd_progs[i]);
        i_cur_shd_prog = i;
    }

    return;
}

// Calc projection matrix
Mat4f RenderContext::calcProjPersp(float fov, float clip_near, float clip_far)
{
	if (window)
	{
		int width, height;

		glfwGetWindowSize(window, &width, &height);

		return Mat4f::projPerspective(fov, float(width) / float(height), clip_near, clip_far);
	}
	else
		return Mat4f::ident();
}

GLint RenderContext::getCurShaderProgram() 
{ 
	if (i_cur_shd_prog >= 0)
		return shd_progs[i_cur_shd_prog];
	else
		return NULL; 
}

//// Set projection matrix
//void RenderContext::setProjection()
//{
//    if (window)
//    {
//        int width, height;
//
//        glfwGetWindowSize(window, &width, &height);
//
//        projection = Mat4f::projOrtho(-(width / 2.0f),(width / 2.0f),-(height / 2.0f),(height / 2.0f),0,1);
//        if (shader_program >= 0)
//        {
//            GLint uni_proj = glGetUniformLocation(shader_program,"proj");
//            if (uni_proj >= 0)
//                glUniformMatrix4fv(uni_proj,1,GL_FALSE,projection.getData());
//        }
//    }
//}

//// Set view matrix
//// Inputs:  pos = position vector for location of camera
////          zoom = zoom value where 1.0f = normal (100%) zoom
//void RenderContext::setView(Vec3f pos,float zoom)
//{
//	GLuint shader = getCurShaderProgram();
//
//    view = Mat4f::transl(-pos);
//    if (glIsProgram(shader) == GL_TRUE)
//    {
//        GLint uni_view = glGetUniformLocation(shader,"view");
//        if (uni_view >= 0)
//            glUniformMatrix4fv(uni_view,1,GL_FALSE,view.getData());
//    }
//}

// Set model matrix
// Inputs:  model_in = 4x4 model matrix input
void RenderContext::setModelMat(Mat4f model_in)
{
	GLuint shader = getCurShaderProgram();

    model = model_in;
    if (glIsProgram(shader) == GL_TRUE)
    {
        GLint uni_model = glGetUniformLocation(shader,"model");
        if (uni_model >= 0)
            glUniformMatrix4fv(uni_model,1,GL_FALSE,model.getData());
    }
}

// Set model matrix
// Inputs:  pos = position vector for object location
//          rot = rotation of object around z axis
void RenderContext::setModelMat(Vec2f pos,float rot)
{
	GLuint shader = getCurShaderProgram();

    model = Mat4f::transf(Mat3f::rot(rot,Vec3f(0.0f,0.0f,1.0f)),Vec3f(pos,0.0f));
    if (glIsProgram(shader) == GL_TRUE)
    {
        GLint uni_model = glGetUniformLocation(shader,"model");
        if (uni_model >= 0)
            glUniformMatrix4fv(uni_model,1,GL_FALSE,model.getData());
    }
}

RenderContext::~RenderContext()
{
    glfwTerminate();
}

// END RenderContext IMPLEMENTATION

void GLFWErrorHandler(int error,const char *description)
{
    std::cout << "GLFW error: " << error << ", " << description << std::endl;
}

// Resize window
void ResizeWindow(GLFWwindow* window, GLsizei width, GLsizei height)
{
	if (height == 0)
		height = 1;

	//Set GL viewport
	glViewport(0, 0, width, height);

	// TODO: Re-calculate projection matrix if it is cached
	// setProjection();
}

#endif
