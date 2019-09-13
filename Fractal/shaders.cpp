#include "shaders.h"

UINT compileVertexShader(const char* vertexShaderSource) {
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	return vertexShader;
}

UINT compileFragmentShader(const char* fragmentShaderSource) {
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	return fragmentShader;
}

void checkCompilationSuccess(UINT shader, std::string shaderText) {
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << shaderText << " compilation failed\n" << infoLog << "\n";
	}
}

void checkLinkSuccess(UINT shaderProgram, std::string shaderText) {
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << shaderText << " compilation failed\n" << infoLog << "\n";
	}
}

UINT attachShaders(UINT vertexShader, UINT fragmentShader) {
	UINT shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	return shaderProgram;
}

UINT compileShaders(const char* vertexShaderSource, const char* fragmentShaderSource) {
	UINT vertexShader = compileVertexShader(vertexShaderSource);
	checkCompilationSuccess(vertexShader, "Vertex shader");
	UINT fragmentShader = compileFragmentShader(fragmentShaderSource);
	checkCompilationSuccess(fragmentShader, "Fragment shader");

	UINT shaderProgram = attachShaders(vertexShader, fragmentShader);
	checkLinkSuccess(shaderProgram, "ShaderProgram");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

std::string readShaderFromFile(const char* path) {
	std::string code;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(path);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		code = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Shader file not successfully read" << std::endl;
	}
	return code;
}

//---------------------------------------------

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vShaderCode = readShaderFromFile(vertexPath);
	std::string fShaderCode = readShaderFromFile(fragmentPath);
	ID = compileShaders(vShaderCode.c_str(), fShaderCode.c_str());
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec2(const std::string& name, glm::vec2 value) const {
	glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
}
void Shader::setVec3(const std::string& name, vec3 value) const {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}
void Shader::setMat4(const std::string& name, mat4 value) const
{
	UINT loc = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setMat3(const std::string& name, glm::mat3 value) const {
	UINT loc = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}