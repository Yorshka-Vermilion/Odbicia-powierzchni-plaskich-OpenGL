#pragma once

#include<iostream>
#include<vector>

#include"Vertex.h"
#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>




class Mesh
{
public:
	Vertex* vertexArray;
	unsigned nrOfVertices;

	GLuint VAO;
	GLuint VBO;

	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 ModelMatrix;

	void initVAO()
	{
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), this->vertexArray, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}


	void updateModelMatrix()
	{
		this->ModelMatrix = glm::mat4(1.f);
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->origin);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->position - this->origin);
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
	}

public:
	Mesh(
		Vertex* vertexArray,
		const unsigned& nrOfVertices,
		GLuint* indexArray,
		const unsigned& nrOfIndices,
		glm::vec3 position = glm::vec3(0.f),
		glm::vec3 origin = glm::vec3(0.f),
		glm::vec3 rotation = glm::vec3(0.f),
		glm::vec3 scale = glm::vec3(1.f))
	{
		this->position = position;
		this->origin = origin;
		this->rotation = rotation;
		this->scale = scale;

		this->nrOfVertices = nrOfVertices;

		this->vertexArray = new Vertex[this->nrOfVertices];
		for (size_t i = 0; i < nrOfVertices; i++)
		{
			this->vertexArray[i] = vertexArray[i];
		}

		this->initVAO();
		this->updateModelMatrix();
	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);

		delete[] this->vertexArray;	
	}


	void rotate(const glm::vec3 rotation)
	{
		this->rotation += rotation;
	}


	void render(ShaderObj* shader)
	{
		this->updateModelMatrix();
		shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
		shader->Use();
		glBindVertexArray(this->VAO);

		glDrawArrays(GL_TRIANGLES, 0, this->nrOfVertices);

		glBindVertexArray(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


};
