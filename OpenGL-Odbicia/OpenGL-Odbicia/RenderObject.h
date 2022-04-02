#pragma once
#include <GL/glew.h>
#include <GL/GL.h>
#include <vector>
#include <glm/glm.hpp>
#include <stb.h>

#include "OBJLoader.h"
#include "ShaderObj.h"
#include "Mesh.h"


using namespace std;
class RenderObject
{
	Mesh* mesh;
	std::vector<Vertex> wooo;

	// Textura
	GLuint texture;
	bool hasTexture = false;
	int width, height, colors;
	unsigned char* textureIMG;
	string defaultTextureFile = "Tekstury/skala.jpg";

public:
	RenderObject() {

	};

	RenderObject(std::string filename, glm::vec3 position) {
		//load_obj(filename.c_str());
		//bool loadout = loader.LoadFile(filename);
		std::vector<Vertex> mesh = loadOBJ(filename.c_str());
		this->mesh = new Mesh(mesh.data(), mesh.size(), NULL, 0, position,
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f));

	}

	~RenderObject() {
		if (NULL != texture) {
			glDeleteTextures(1, &texture);
		}
	}

	void render(ShaderObj* shader) {
		shader->Use();
		if (!this->hasTexture) {
			SetTexture(defaultTextureFile);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		//shader->set1i(texture, "tex0");
		
		this->mesh->render(shader);
		glBindTexture(GL_TEXTURE_2D, 0);
		shader->Stop();
	}

	void SetTexture(std::string filename) {
		this->textureIMG = stbi_load(filename.c_str(), &width, &height, &colors, 0);
		glGenTextures(1, &this->texture);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureIMG);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		stbi_image_free(textureIMG);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(0);
		this->hasTexture = true;
	}

	void init() {
	
	}
};

