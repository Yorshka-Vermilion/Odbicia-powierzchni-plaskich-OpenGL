#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb.h>

#include "ShaderObj.h"
#include "Camera.h"

class Cubemap
{
	GLuint texture;
	float verts[24] = {
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f
	};
	unsigned int inds[48] = {
		1, 2, 6,
		6, 5, 1,
		// Left
		0, 4, 7,
		7, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4,
		// Bottom
		0, 3, 2,
		2, 1, 0,
		// Back
		0, 1, 5,
		5, 4, 0,
		// Front
		3, 7, 6,
		6, 2, 3
	}; 

	std::string images[6] = {
		"Cubemaps/negx.jpg",
		"Cubemaps/posx.jpg",
		"Cubemaps/posy.jpg",
		"Cubemaps/negy.jpg", 
		"Cubemaps/negz.jpg",
		"Cubemaps/posz.jpg"
	};
	unsigned int VAO, VBO, EBO;

public:
	Cubemap() {
	
	}
	Cubemap(ShaderObj* shader) {

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), &inds, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (int i = 0; i < 6; i++) {
			int width, height, colors;
			unsigned char* textureIMG = stbi_load(images[i].c_str(), &width, &height, &colors, 0);
			if (textureIMG != NULL) {
				stbi_set_flip_vertically_on_load(false);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureIMG);
			}
			else {
				std::cout << "Nie udalo sie zaladowac tekstury cubemapy: " << images[i] << std::endl;
			}
			stbi_image_free(textureIMG);
			
		}

		shader->Use();
		shader->set1i(0, "cubemap");
	}

	void render(Camera camera, glm::vec2 rozmiarOkna, ShaderObj* shader) {
		glDepthFunc(GL_LEQUAL);
		shader->Use();
		/*
		glm::mat4 view = camera.view;
		glm::mat4 proj = camera.projection;
		*/
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
		view = glm::mat4(glm::mat3(glm::lookAt(camera.position, camera.position + camera.orientation, camera.up)));
		proj = glm::perspective(glm::radians(45.0f), (float)rozmiarOkna.x / rozmiarOkna.y, 0.1f, 100.f);


		shader->setMat4fv(view, "view");
		shader->setMat4fv(proj, "projection");
		shader->Use();
		glCullFace(GL_FRONT);
		glBindVertexArray(this->VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		shader->Stop();
		glDepthFunc(GL_LESS);
		glCullFace(GL_BACK);
	}

};

