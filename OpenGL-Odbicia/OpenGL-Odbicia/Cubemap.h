#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <stb.h>

#include "ShaderObj.h"
#include "Camera.h"
#include "CubemapRenderCamera.h"
#include "RenderObject.h"

#include <vector>

class Cubemap
{
public:
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
	unsigned int VAO, VBO, EBO,size;
	glm::mat4 view,proj;
	CubemapRenderCamera* renderCamera;
	GLuint frameBuffer, depthBuffer;

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

	Cubemap(glm::vec3 position, int size, CubemapRenderCamera* rCamera) {
		this->size = size;
		renderCamera = rCamera;
		renderCamera->SetPosition(position);
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), &inds, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		for (int i = 0; i < 6; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		
		

		
	}

	void refreshDynamicCubemap(std::vector<RenderObject*> *renderObjects, ShaderObj* shader, Cubemap* envCubemap, ShaderObj* cubemapShader, glm::vec2 windowSize) {
		
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glViewport(0, 0, size, size);
		
		
		

		for (int i = 0; i < 6; i++) {
			renderCamera->switchFaces(i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture, 0);
			glClearColor(0.f, 0.f, 0.1f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			 
			for (size_t j = 0; j < renderObjects->size(); j++) {
				if (!renderObjects->at(0)->reflective)
					renderObjects->at(0)->renderInCubemap(shader,renderCamera);
			}
			
			envCubemap->updateMatrix(*renderCamera);
			envCubemap->render(cubemapShader);
			
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, windowSize.x, windowSize.y);
	}

	void updateMatrix(Camera camera) {
		this->proj = camera.projection;
		this->view = glm::mat4(glm::mat3(camera.view[0], camera.view[1], camera.view[2]));
	}

	void updateMatrix(CubemapRenderCamera camera) {
		this->proj = camera.proj;
		this->view = glm::mat4(glm::mat3(camera.view[0], camera.view[1], camera.view[2]));
	}

	void render(ShaderObj* shader) {
		glDepthFunc(GL_LEQUAL);
		shader->Use();
		
		
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

	void remove() {
		
		glDeleteBuffers(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteFramebuffers(1, &frameBuffer);
		glDeleteRenderbuffers(1, &depthBuffer);
		glDeleteTextures(6, &texture);
	}

};

