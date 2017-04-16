// *****************************************************************************************************************************
// gl_mesh.hpp
// OpenGL Rendering
// Mesh definitions
// Author: Cory Douthat
// Copyright (c) 2017 Cory Douthat, All Rights Reserved.
// *****************************************************************************************************************************

#ifndef GL_MESH_HPP
#define GL_MESH_HPP

#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <sstream>

#define GLEW_STATIC
#include <GL\glew.h>

#define GLFW_DLL
#define GLFW_INCLUDE_GLU
#include "GLFW\glfw3.h"

#include "file_help.hpp"

#include "linked_list.hpp"

#include "vec.hpp"
#include "mat.hpp"

// VERTEX
template <typename T = float>
struct Vertex
{
	Vec3<T> pos;	// Position
	Vec3<T> norm;	// Normal vector
	Vec2<T> uv;		// Texture coordinates
};

// TEXTURE
struct Texture
{
	GLuint id;
	std::string type;
	aiString path;	// Store the path of the texture for later comparison
};


// MESH CLASS
template <typename T = float>
class Mesh
{
private:
	GLuint VAO, VBO, EBO;	// OpenGL render buffer IDs

	void setupMesh();
public:
	std::vector<Vertex<T>> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	Mesh(std::vector <Vertex<T>> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	void draw(GLuint shader_id);
};

template <typename T = float>
Mesh<T>::Mesh(std::vector<Vertex<T>> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	
	this->setupMesh();
}

template <typename T = float>
void Mesh<T>::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex<T>), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<T>), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<T>), (GLvoid*)offsetof(Vertex<T>, norm));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex<T>), (GLvoid*)offsetof(Vertex<T>, uv));

	glBindVertexArray(0);
}

// TODO: use array im shader for texture uniforms instead of named?
template <typename T = float>
void Mesh<T>::draw(GLuint shader_id)
{
	GLuint diffuse_num = 0;
	GLuint specular_num = 0;
	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
										  // Retrieve texture number (the N in diffuse_textureN)
		std::stringstream ss;
		std::string number;
		std::string name = this->textures[i].type;
		if (name == "texture_diffuse")
			ss << "[" << ++diffuse_num << "]"; // Transfer GLuint to stream
		else if (name == "texture_specular")
			ss << "[" << ++specular_num << "]"; // Transfer GLuint to stream
		number = ss.str();

		glUniform1f(glGetUniformLocation(shader_id, ("material." + name + number).c_str()), float(i));

		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}

	// Set number of diffuse and specular textures
	glUniform1ui(glGetUniformLocation(shader_id, "material.num_tex_diffuse"), diffuse_num);
	glUniform1ui(glGetUniformLocation(shader_id, "material.num_tex_specular"), specular_num);

	glActiveTexture(GL_TEXTURE0);

	// Draw mesh
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

#endif
