#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <glad\glad.h>
#include <glfw\glfw3.h>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>

struct Camera {
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;
	glm::mat4 view;
	glm::vec3 eAngles; // Pitch, yaw, roll (x, y, z)
	float speed = 1.0;

	Camera(glm::vec3 _position, glm::vec3 _eAngles);

	void processInput(GLFWwindow* window, float deltaTime);
	void updateView();

	void moveForward(float deltaTime);
	void moveBackward(float deltaTime);
	void moveLeft(float deltaTime);
	void moveRight(float deltaTime);
	void moveUp(float deltaTime);
	void moveDown(float deltaTime);
};