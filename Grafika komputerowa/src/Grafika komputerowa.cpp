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

static void Motion(float deltaTime, glm::vec3& position) {
	position.x = position.x + deltaTime * 0.1;
	position.z = position.z + deltaTime * 0.1;
}

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

	VAO planeVAO;
	planeVAO.Bind();
	VBO planeVBO(samolot);

	planeVAO.LinkVBO(planeVBO, 0, 3, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	planeVAO.LinkVBO(planeVBO, 2, 2, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	planeVAO.LinkVBO(planeVBO, 3, 3, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normals));

	planeVAO.Unbind();
	planeVBO.Unbind();

	glm::vec3 planePos = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::mat4 planeModel = glm::mat4(1.0f);
	planeModel = glm::translate(planeModel, planePos);

	std::vector<Vertex> smiglo = loadOBJ("res/model/smiglo.obj");

	VAO engineVAO;
	engineVAO.Bind();
	VBO engineVBO(smiglo);

	engineVAO.LinkVBO(engineVBO, 0, 3, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	engineVAO.LinkVBO(engineVBO, 2, 2, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	engineVAO.LinkVBO(engineVBO, 3, 3, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normals));

	engineVAO.Unbind();
	engineVBO.Unbind();

	glm::vec3 enginePos = glm::vec3(planePos.x, planePos.y, planePos.z-0.1f);

	glm::mat4 engineModel = glm::mat4(1.0f);
	engineModel = glm::translate(engineModel, enginePos);

	std::vector<Vertex> cloud = loadOBJ("res/model/chmurka.obj");

	VAO cloudVAO;
	cloudVAO.Bind();
	VBO cloudVBO(cloud);

	cloudVAO.LinkVBO(cloudVBO, 0, 3, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	cloudVAO.LinkVBO(cloudVBO, 2, 2, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	cloudVAO.LinkVBO(cloudVBO, 3, 3, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normals));

	cloudVAO.Unbind();
	cloudVBO.Unbind();

	glm::vec3 cloudPos1 = glm::vec3(1.0f, 0.0f, -4.0f);
	glm::vec3 cloudPos2 = glm::vec3(-2.0f, 0.0f, 3.0f);
	glm::vec3 cloudPos3 = glm::vec3(3.0f, 0.0f, -2.0f);
	glm::vec3 cloudPos4 = glm::vec3(-4.0f, 0.0f, 1.0f);


	glm::mat4 cloudModel1 = glm::translate(glm::mat4(1.0f), cloudPos1);
	glm::mat4 cloudModel2 = glm::translate(glm::mat4(1.0f), cloudPos2);
	glm::mat4 cloudModel3 = glm::translate(glm::mat4(1.0f), cloudPos3);
	glm::mat4 cloudModel4 = glm::translate(glm::mat4(1.0f), cloudPos4);

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
		planeVAO.Bind();

		planeModel = glm::translate(glm::mat4(1.0f), planePos);
		//planeModel = glm::rotate(planeModel, glm::radians(currentAngle), glm::vec3(0, 0, 1));
		//currentAngle += 10 * deltaTime;
		
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(planeModel));
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), cubePos.x, cubePos.y, cubePos.z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glDrawArrays(GL_TRIANGLES, 0, samolot.size());
		planeVAO.Unbind();
		// Render engine
		shaderProgram.Activate();
		Dzialaj.Bind();
		engineVAO.Bind();

		engineModel = glm::translate(glm::mat4(1.0f), enginePos);
		engineModel = glm::rotate(engineModel, glm::radians(currentAngle), glm::vec3(0, 0, 1));
		currentAngle += 180 * deltaTime;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(engineModel));
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), cubePos.x, cubePos.y, cubePos.z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glDrawArrays(GL_TRIANGLES, 0, smiglo.size());
		engineVAO.Unbind();
		// Render clouds

		shaderProgram.Activate();
		Dzialaj.Bind();
		cloudVAO.Bind();

		Motion(deltaTime, cloudPos1);
		cloudModel1 = glm::translate(glm::mat4(1.0f), cloudPos1);
		cloudModel1 = glm::scale(cloudModel1, glm::vec3(0.3));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(cloudModel1));
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), cubePos.x, cubePos.y, cubePos.z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glDrawArrays(GL_TRIANGLES, 0, cloud.size());

		Motion(deltaTime, cloudPos2);
		cloudModel2 = glm::translate(glm::mat4(1.0f), cloudPos2);
		cloudModel2 = glm::scale(cloudModel2, glm::vec3(0.4));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(cloudModel2));
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), cubePos.x, cubePos.y, cubePos.z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glDrawArrays(GL_TRIANGLES, 0, cloud.size());

		Motion(deltaTime, cloudPos3);
		cloudModel3 = glm::translate(glm::mat4(1.0f), cloudPos3);
		cloudModel3 = glm::scale(cloudModel3, glm::vec3(0.5));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(cloudModel3));
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), cubePos.x, cubePos.y, cubePos.z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glDrawArrays(GL_TRIANGLES, 0, cloud.size());

		Motion(deltaTime, cloudPos4);
		cloudModel4 = glm::translate(glm::mat4(1.0f), cloudPos4);
		cloudModel4 = glm::scale(cloudModel4, glm::vec3(0.3));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(cloudModel4));
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), cubePos.x, cubePos.y, cubePos.z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glDrawArrays(GL_TRIANGLES, 0, cloud.size());

		// Render light
		circularMotion(deltaTime, cubePos);
		lightShader.Activate();
		lightVAO.Bind();

		cubeModel = glm::translate(glm::mat4(1.0f), cubePos);

		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

		// Swap buffers

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	shaderProgram.Delete();
	lightShader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}