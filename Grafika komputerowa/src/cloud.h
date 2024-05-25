#pragma once

#include "OBJLoader.h"
#include "glm/glm.hpp"
#include "VAO.h"
#include "shaderClass.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Random.h"

class Cloud {
private:
	std::vector<Vertex> cloudVertices;
	VAO cloudVAO;
	VBO cloudVBO;

	glm::vec3 cloudPos;
	glm::vec3 cloudScale;
	glm::vec3 cloudRotation;
	float cloudAlpha;
	float cloudAngle;
	float cloudSpeed;
	glm::mat4 cloudModel = glm::mat4(1.0f);
public:
	Cloud();
	void draw(Shader &shader, float deltaTime);
	void move(float deltaTime, float speed);

};

