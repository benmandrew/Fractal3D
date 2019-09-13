#pragma once
#pragma once

#include <glad\glad.h>
#include <glfw\glfw3.h>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

typedef glm::vec3 vec3;
typedef glm::mat4 mat4;

UINT compileVertexShader(const char* vertexShaderSource);

UINT compileFragmentShader(const char* fragmentShaderSource);

void checkCompilationSuccess(UINT shader, std::string shaderText);

void checkLinkSuccess(UINT shaderProgram, std::string shaderText);

UINT attachShaders(UINT vertexShader, UINT fragmentShader);

UINT compileShaders(const char* vertexShaderSource, const char* fragmentShaderSource);


struct Shader {
	UINT ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, glm::vec2 value) const;
	void setVec3(const std::string& name, vec3 value) const;
	void setMat4(const std::string& name, mat4 value) const;
	void setMat3(const std::string& name, glm::mat3 value) const;
};