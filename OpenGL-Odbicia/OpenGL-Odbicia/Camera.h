#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "ShaderObj.h"


enum direction { FORWARD = 0, RIGHT, BACK, LEFT, UP, DOWN };


class Camera
{
public:
	glm::vec3 position;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec2 windowSize;

	float speed = 1.f;
	float sensitivity = 15.f;
	
	ShaderObj mainShader;
	const char* uniformLocation;

	float pitch, yaw;

	Camera() {
	
	}

	Camera(glm::vec2 WindowSize, ShaderObj& shader, const char* uniform) {
		this->windowSize = WindowSize;
		this->position = glm::vec3(0.0f, 0.0f, 2.f);
		this->mainShader = shader;
		this->uniformLocation = uniform;
	}

	void UpdateMatrix() {

		mainShader.Use();

		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		view = glm::lookAt(position, position + orientation, up);
		projection = glm::perspective(glm::radians(45.0f), windowSize.x / windowSize.y, 1.f, 100.0f);
	
		glUniformMatrix4fv(glGetUniformLocation(mainShader.ID, uniformLocation), 1, GL_FALSE, glm::value_ptr(projection * view));
	}

	void Move(const int direction, const float& dt) {
		switch (direction) {
		case FORWARD:
			this->position += this->speed * this->orientation * dt;
			break;
		case RIGHT:
			this->position += this->speed * dt * glm::normalize(glm::cross(orientation, up));
			break;
		case BACK:
			this->position += this->speed * dt * (-orientation);
			break;
		case LEFT:
			this->position += this->speed * dt * (-glm::normalize(glm::cross(orientation, up)));
			break;
		case UP:
			this->position += this->speed * dt * up;
			break;
		case DOWN:
			this->position += this->speed * dt * -up;
			break;
		}
	}

	void Rotate(const glm::vec2 mouseRot, const float& dt) {
		glm::vec3 newOrint = glm::rotate(orientation, glm::radians(-(mouseRot.x * (sensitivity*dt))), glm::normalize(glm::cross(orientation, up)));
		
		if (!((glm::angle(newOrint, up) <= glm::radians(5.0f)) || (glm::angle(newOrint, -up) <= glm::radians(5.0f)))) {
			orientation = newOrint;
		}
		orientation = glm::rotate(orientation, glm::radians(-(mouseRot.y * (sensitivity*dt))), up);
	}
};

