#pragma once
#include <GL/glew.h>
#include <GL/GL.h>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "OBJ_Loader.h"

using namespace std;
class RenderObject
{
	GLuint VertexArrayID;
	std::vector<GLfloat> vertexBufferData;
	GLuint vertexBuffer;
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec3> normals;
	std::vector<GLushort> elements;
	objl::Loader loader;


public:

	RenderObject() {
		GLfloat tmp[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
		};
		for (size_t i = 0; i < 9; i++) {
			vertexBufferData.push_back(tmp[i]);
		}
		this->VertexArrayID = 0;
		this->vertexBuffer = 0;
	}

	RenderObject(float* tmp, unsigned int size) {
		for (size_t i = 0; i < size; i++) {
			vertexBufferData.push_back(tmp[i]);
		}
		this->VertexArrayID = 0;
		this->vertexBuffer = 0;
	}

	RenderObject(std::string filename) {
		//load_obj(filename.c_str());
		bool loadout = loader.LoadFile(filename);
		if (!loadout) {
			std::cout << "DSDS";
		}
		this->VertexArrayID = 0;
		this->vertexBuffer = 0;
	}

	void render() {

		
		glBindVertexArray(this->VertexArrayID);


		// Generate 1 buffer, put the resulting identifier in vertexbuffer
		glGenBuffers(1, &vertexBuffer);
		// The following commands will talk about our 'vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		// Give our vertices to OpenGL.
		//glBufferData(GL_ARRAY_BUFFER, vertexBufferData.size() * sizeof(GLfloat), vertexBufferData.data(), GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, loader.LoadedMeshes[0].Vertices.size() * sizeof(objl::Vector3), loader.LoadedMeshes[0].Vertices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_TRUE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, loader.LoadedMeshes[0].Vertices.size()); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		/*

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, loader.LoadedMeshes[0].Vertices.data());


		// Draw the primitives via index-array
		glDrawElements(GL_TRIANGLES, loader.LoadedMeshes[0].Indices.size(), GL_UNSIGNED_BYTE, loader.LoadedMeshes[0].Indices.data());

		glDisableClientState(GL_VERTEX_ARRAY);
		*/
		

	}

	void init() {
	
	}

	void load_obj(const char* filename)
	{
		ifstream in(filename, ios::in);
		if (!in)
		{
			cerr << "Cannot open " << filename << endl; exit(1);
		}

		string line;
		while (getline(in, line))
		{
			if (line.substr(0, 2) == "v ")
			{
				istringstream s(line.substr(2));
				glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
				vertices.push_back(v);
			}
			else if (line.substr(0, 2) == "f ")
			{
				istringstream s(line.substr(2));
				GLushort a, b, c;
				s >> a; s >> b; s >> c;
				a--; b--; c--;
				elements.push_back(a); elements.push_back(b); elements.push_back(c);
			}
			/* anything else is ignored */
		}

		normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
		for (int i = 0; i < elements.size(); i += 3)
		{
			GLushort ia = elements[i];
			GLushort ib = elements[i + 1];
			GLushort ic = elements[i + 2];
			
			glm::vec3 normal = glm::normalize(glm::cross(
				glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
				glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
			normals[ia] = normals[ib] = normals[ic] = normal;
			
		}
	}
};

