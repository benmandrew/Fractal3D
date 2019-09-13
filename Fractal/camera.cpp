
#include "camera.h"

Camera::Camera(glm::vec3 _position, glm::vec3 _eAngles)
	: position(_position), eAngles(_eAngles) {

}

void Camera::processInput(GLFWwindow* window, float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { // Forward
		moveForward(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { // Backward
		moveBackward(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { // Right
		moveRight(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { // Left
		moveLeft(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) { // Up
		moveUp(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) { // Down
		moveDown(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { // Roll anticlockwise

	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { // Roll clockwise

	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { // Pitch up

	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { // Pitch down

	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) { // Yaw right

	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) { // Yaw left

	}
}

void Camera::updateView() {
	glm::mat4 matRoll = glm::rotate(glm::mat4(1.0f), eAngles.z, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 matPitch = glm::rotate(glm::mat4(1.0f), eAngles.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 matYaw = glm::rotate(glm::mat4(1.0f), eAngles.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotate = matRoll * matPitch * matYaw;
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), -position);
	view = rotate * translate;
	right = view[0];
	up = view[1];
	forward = view[2];
}

void Camera::moveForward(float deltaTime) {
	position += forward * deltaTime * speed;
}
void Camera::moveBackward(float deltaTime) {
	position -= forward * deltaTime * speed;
}
void Camera::moveRight(float deltaTime) {
	position += right * deltaTime * speed;
}
void Camera::moveLeft(float deltaTime) {
	position -= right * deltaTime * speed;
}
void Camera::moveUp(float deltaTime) {
	position += up * deltaTime * speed;
}
void Camera::moveDown(float deltaTime) {
	position -= up * deltaTime * speed;
}