#pragma once
#include <GL/glew.h>
#include <GL/GL.h>
#include <vector>
#include <glm/glm.hpp>

#include "OBJLoader.h"
#include "Mesh.h"

using namespace std;
class RenderObject
{
	Mesh* mesh;
	std::vector<Vertex> wooo;

public:
	RenderObject() {

	};

	RenderObject(std::string filename) {
		//load_obj(filename.c_str());
		//bool loadout = loader.LoadFile(filename);
		std::vector<Vertex> mesh = loadOBJ(filename.c_str());
		this->mesh = new Mesh(mesh.data(), mesh.size(), NULL, 0, glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f));
	}

	void render() {
		this->mesh->render();
	}

	void init() {
	
	}
};

