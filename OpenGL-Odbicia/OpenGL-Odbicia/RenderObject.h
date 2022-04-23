#pragma once
#include <GL/glew.h>
#include <GL/GL.h>
#include <vector>
#include <glm/glm.hpp>
#include <stb.h>

#include "OBJLoader.h"
#include "ShaderObj.h"
#include "Mesh.h"
#include "Camera.h"

using namespace std;
class RenderObject
{
	// Textura
	GLuint texture;
	bool hasTexture = false;
	int width, height, colors;
	unsigned char* textureIMG;
	string defaultTextureFile = "Tekstury/skala.jpg";
	

	// Odbicia
	GLuint reflectionTexture;
	GLuint frameBuffer, depthBuffer;
	int widthFB = 1024;
	int heightFB = 1024;

public:
	Mesh* mesh;
	glm::vec3 position, rotation;
	bool reflective = false;
	bool horizontal = false;

	RenderObject() {

	};

	RenderObject(std::string filename, glm::vec3 position, glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f)) {
		//load_obj(filename.c_str());
		//bool loadout = loader.LoadFile(filename);
		this->horizontal = horizontal;
		this->position = position;
		this->rotation = rotation;
		std::vector<Vertex> mesh = loadOBJ(filename.c_str());
		this->mesh = new Mesh(mesh.data(), mesh.size(), NULL, 0, position,glm::vec3(0.f),rotation,scale);
	}

	~RenderObject() {
		if (NULL != texture) {
			glDeleteTextures(1, &texture);
		}
	}

	void bindReflections() {
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glViewport(0, 0, widthFB, heightFB);
	}

	void unbindReflections(glm::vec2 rozmiarOkna) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, rozmiarOkna.x, rozmiarOkna.y);
	}


	void initReflections(ShaderObj* shader = NULL) {
		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glGenTextures(1, &reflectionTexture);
		glBindTexture(GL_TEXTURE_2D, reflectionTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->widthFB, this->heightFB, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, reflectionTexture, 0);
		
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, widthFB, heightFB);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
		
		reflective = true;
		
	}


	void render(ShaderObj* shader, Camera *camera = NULL) {
		shader->Use();
		if (!this->hasTexture && !reflective) {
			SetTexture(defaultTextureFile);
		}

		glActiveTexture(GL_TEXTURE0);
		if (!reflective) {
			glBindTexture(GL_TEXTURE_2D, texture);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, reflectionTexture);
			shader->set1i(0, "reflectionTexture");
			shader->setMat4fv(camera->projection * camera->view, "cameraMatrix");
		}
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

