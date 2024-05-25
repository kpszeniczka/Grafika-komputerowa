#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

#include "plane.h"
#include "Cylinder.h"
#include "Texture.h"
#include "Camera.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "OBJLoader.h"

const unsigned int width = 800;
const unsigned int height = 800;

// Light movement parameters
float lightRadius = 10.0f;  // Promień orbity światła może być mniejszy, jeśli sześcian jest mniejszy
float lightHeight = 2.0f;  // Wysokość światła nad sześcianem
float lightSpeed = 0.5f;   // Szybkość obrotu światła (radiany na sekundę)
float lightAngle = 0.0f;   // Początkowy kąt w radianach

// Function to update light position
static void circularMotion(float deltaTime, glm::vec3& position) {
	lightAngle += lightSpeed * deltaTime;

	if (lightAngle > 2 * M_PI) {
		lightAngle -= 2 * M_PI;
	}

	position.x = lightRadius * cos(lightAngle);
	position.y = lightRadius * sin(lightAngle);
	position.z = lightHeight;
}

GLfloat lightVertices[] = {
	// Positions of the vertices for a cube
	-0.5f, -0.5f, -0.5f,  // 0
	 0.5f, -0.5f, -0.5f,  // 1
	 0.5f,  0.5f, -0.5f,  // 2
	-0.5f,  0.5f, -0.5f,  // 3
	-0.5f, -0.5f,  0.5f,  // 4
	 0.5f, -0.5f,  0.5f,  // 5
	 0.5f,  0.5f,  0.5f,  // 6
	-0.5f,  0.5f,  0.5f   // 7
};

GLuint lightIndices[] = {
	0, 1, 2, 0, 2, 3,  // Front face
	4, 5, 6, 4, 6, 7,  // Back face
	0, 4, 7, 0, 7, 3,  // Left face
	3, 7, 6, 3, 6, 2,  // Top face
	2, 6, 5, 2, 5, 1,  // Right face
	1, 5, 4, 1, 4, 0   // Bottom face
};


int main()
{
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 800, "dzialajprosze", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, width, width);

	Shader shaderProgram("res/shader/default.vert", "res/shader/default.frag");
	
	Shader lightShader("res/shader/light.vert", "res/shader/light.frag");
	VAO lightVAO;
	lightVAO.Bind();
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));
	lightVAO.LinkVBO(lightVBO, 0, 3, 3 * sizeof(float), 0);
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 8.0f));

	std::vector<Vertex> samolot = loadOBJ("res/model/samolot.obj");

	VAO VAO;
	VAO.Bind();
	VBO VBO(samolot);

	VAO.LinkVBO(VBO, 0, 3, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	VAO.LinkVBO(VBO, 2, 2, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	VAO.LinkVBO(VBO, 3, 3, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normals));

	VAO.Unbind();
	VBO.Unbind();

	glm::vec3 planePos = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::mat4 planeModel = glm::mat4(1.0f);
	planeModel = glm::translate(planeModel, planePos);

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 cubePos = glm::vec3(2.0f, 2.0f, 0.0f);
	glm::mat4 cubeModel = glm::mat4(1.0f);
	cubeModel = glm::translate(cubeModel, cubePos);
	
	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(planeModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), cubePos.x, cubePos.y, cubePos.z);

	Texture Dzialaj("res/textures/slime.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	Dzialaj.texUnit(shaderProgram, "tex0", 0);

	// Time tracking variables for light animation
	float lastFrameTime = glfwGetTime();
	float currentFrameTime;
	float deltaTime;

	float currentAngle = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		// Delta time
		currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		// Clear

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update camera

		camera.Inputs(window);
		shaderProgram.Activate();
		camera.Matrix(shaderProgram, "camMatrix");
		camera.updateMatrix(45.0f, 0.1f, 1000.0f);
		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");

		// Render plane
		shaderProgram.Activate();
		Dzialaj.Bind();
		VAO.Bind();

		circularMotion(deltaTime, planePos);
		planeModel = glm::translate(glm::mat4(1.0f), planePos);

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(planeModel));
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), cubePos.x, cubePos.y, cubePos.z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glDrawArrays(GL_TRIANGLES, 0, samolot.size());

		// Render light
		circularMotion(deltaTime, cubePos);
		lightShader.Activate();
		lightVAO.Bind();
		cubeModel = glm::translate(glm::mat4(1.0f), cubePos);
		cubeModel = glm::rotate(glm::mat4(1.0f), glm::radians(currentAngle), glm::vec3(1, 1, 1));
		currentAngle += 10 * deltaTime;

		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

		// Swap buffers

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO.Delete();
	VBO.Delete();
	shaderProgram.Delete();
	lightEBO.Delete();
	lightVBO.Delete();
	lightVAO.Delete();
	lightShader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}