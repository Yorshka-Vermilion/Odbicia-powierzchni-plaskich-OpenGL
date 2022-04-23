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
#include <glm/gtx/transform.hpp>

class CubemapRenderCamera
{
public:
	float near = 0.1f;
	float far = 200.f;
	unsigned int FOV = 90;
	int aspect = 1;

	glm::vec3 position;
	float pitch = 0;
	float yaw = 0;

	glm::mat4 proj, view, render;



	CubemapRenderCamera() {

	}

	CubemapRenderCamera(glm::vec3 position) {
		this->position = position;
		float scaley = (float)(1.f / glm::tan(glm::radians(FOV / 2.f)));
		float scalex = scaley / aspect;
		float frustum = far - near;

		proj[0][0] = scalex;
		proj[1][1] = scaley;
		proj[2][2] = -((far + near) / frustum);
		proj[2][3] = -1;
		proj[3][2] = -((2 * far * near) / frustum);
		proj[3][3] = 0;
	}

	void switchFaces(int face) {
		switch (face) {
		case 0:
			pitch = 0;
			yaw = 90;
			break;
		case 1:
			pitch = 0;
			yaw = -90;
			break;
		case 2:
			pitch = -90;
			yaw = 180;
			break;
		case 3:
			pitch = 90;
			yaw = 180;
			break;
		case 4:
			pitch = 0;
			yaw = 180;
			break;
		case 5:
			pitch = 0;
			yaw = 0;
			break;

		}
		updateMatrix();
	}

	void updateMatrix() {
		view = glm::mat4(1.f);
		view = glm::rotate(view, glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
		view = glm::rotate(view, glm::radians(pitch), glm::vec3(1.f, 0.f, 0.f));
		view = glm::rotate(view, glm::radians(yaw), glm::vec3(0.f, 1.f, 0.f));
		glm::vec3 negCamPos = glm::vec3(-position.x,-position.y,-position.z);
		view = glm::translate(view, negCamPos);
		render = proj * view;
	}

	void SetPosition(glm::vec3 position) {
		this->position = position;
	}

};

