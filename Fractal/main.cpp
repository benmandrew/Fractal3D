
#include <iostream>
#include <string>
#include <vector>

#include <glad\glad.h>
#include <glfw\glfw3.h>

#include "shaders.h"
#include "camera.h"

static int x = 960;
static int y = 540;
static bool animate = true;
static bool spaceHold = false;
static float time = 0.0;

static double MIN_DIST = 1.0;

static const float vertices[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	x = width;
	y = height;
}

void processInput(GLFWwindow* window, float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		MIN_DIST -= 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		MIN_DIST += 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spaceHold) {
		animate = !animate;
		spaceHold = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS) {
		spaceHold = false;
	}
}

UINT getVAO() {
	UINT VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	return VAO;
}

void render(Shader& shader, UINT VAO) {
	shader.use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

glm::mat4 getTransform() {
	float c = cos(time * 0.2);
	float s = sin(time * 0.2);
	return glm::mat4(
		c, 0, s, 0,
		0, 1, 0, 0,
		-s, 0, c, 0,
		0, 0, 0, 1
	);
}

glm::mat4 getFoldTransform() {
	float c = cos(-time * 0.15 + 0.1);
	float s = sin(-time * 0.15 + 0.1);
	return glm::mat4(
		c, 0, s, 0,
		0, 1, 0, 0,
		-s, 0, c, 0,
		0, 0, 0, 1
	);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(x, y, "OpenGLWindow", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialise GLAD\n";
		return -1;
	}
	glViewport(0, 0, x, y);
	Shader shader = Shader(
		"C:\\Users\\Mirrorworld\\Desktop\\Fractal\\Fractal\\vertex.shader",
		"C:\\Users\\Mirrorworld\\Desktop\\Fractal\\Fractal\\fragment.shader"
	);
	shader.use();
	UINT VAO = getVAO();
	Camera cam = Camera(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0));
	shader.setMat4("foldTransform", getFoldTransform());
	float start = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float now = glfwGetTime();
		float deltaTime = start - now;
		start = now;
		time += deltaTime;

		processInput(window, deltaTime);
		cam.processInput(window, deltaTime);

		shader.setVec2("windowSize", glm::vec2(x, y));
		shader.setFloat("MIN_DIST", MIN_DIST);
		std::cout << MIN_DIST << "- (" << cam.view[3][0] << ", " << cam.view[3][1]<< "," << cam.view[3][2] << ")\n";

		cam.updateView();
		shader.setMat4("transform", cam.view);

		//if (animate) {
		//	time += deltaTime;
		//	shader.setMat4("transform", getTransform());
		//	shader.setMat4("foldTransform", getFoldTransform());
		//}
		shader.setFloat("time", glfwGetTime());

		render(shader, VAO);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}