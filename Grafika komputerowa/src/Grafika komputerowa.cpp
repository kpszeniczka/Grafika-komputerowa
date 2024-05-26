#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

#include "Texture.h"
#include "Camera.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "OBJLoader.h"
#include "Random.h"
#include "cloud.h"

#define LICZBACHMUREK 20

const unsigned int width = 800;
const unsigned int height = 800;

// Light movement parameters
float lightRadius = 10.0f;  // Promień orbity światła może być mniejszy, jeśli sześcian jest mniejszy
float lightHeight = 2.0f;  // Wysokość światła nad sześcianem
float lightSpeed = 0.5f;   // Szybkość obrotu światła (radiany na sekundę)
float lightAngle = 0.0f;   // Początkowy kąt w radianach

static void Motion(float deltaTime, glm::vec3& position) {
	position.x = position.x - deltaTime * 0.5;
	position.z = position.z - deltaTime * 0.5;
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

GLfloat floorVertices[] = {
	0.5f,  0.0f, 0.5f,    0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	-0.5f, 0.0f, 0.5f,	 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.5f,  0.0f, -0.5f,	 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, 0.0f, -0.5f,	 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
};

GLuint floorIndices[] = {
	2,1,0,
	1,2,3
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
	Shader cloudProgram("res/shader/cloud.vert", "res/shader/cloud.frag");
	Shader lightShader("res/shader/light.vert", "res/shader/light.frag");
	Shader floorShader("res/shader/floor.vert", "res/shader/floor.frag");

	VAO floorVAO;
	floorVAO.Bind();
	VBO floorVBO(floorVertices, sizeof(floorVertices));
	EBO floorEBO(floorIndices, sizeof(floorIndices));

	floorVAO.LinkVBO(floorVBO, 0, 3, 8 * sizeof(float), (void*) 0);
	floorVAO.LinkVBO(floorVBO, 3, 3, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	floorVAO.LinkVBO(floorVBO, 2, 2, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	floorVAO.Unbind();
	floorVBO.Unbind();
	floorEBO.Unbind();

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

	std::vector<Cloud> chmurki;
	
	chmurki.resize(LICZBACHMUREK);

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 cubePos = glm::vec3(10.0f, 22.5f, 0.0f);
	glm::mat4 cubeModel = glm::mat4(1.0f);
	cubeModel = glm::translate(cubeModel, cubePos);
	
	glm::vec3 floorPos = glm::vec3(0.0f, -5.0f, 0.0f);
	glm::mat4 floorModel = glm::mat4(1.0f);
	floorModel = glm::translate(floorModel, floorPos);
	floorModel = glm::scale(floorModel, glm::vec3(100.0f));

	floorShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(floorShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(floorModel));
	glUniform4f(glGetUniformLocation(floorShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(floorShader.ID, "lightPos"), cubePos.x, cubePos.y, cubePos.z);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(planeModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), cubePos.x, cubePos.y, cubePos.z);

	Texture Dzialaj("res/textures/slime.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	Dzialaj.texUnit(shaderProgram, "tex0", 0);
	Dzialaj.texUnit(floorShader, "tex0", 0);

	// Time tracking variables for light animation
	float lastFrameTime = glfwGetTime();
	float currentFrameTime;
	float deltaTime;

	float currentAngle = 0.0f;

	// Shadows
	Shader shadowProgram("res/shader/shadow.vert", "res/shader/shadow.frag");

	unsigned int shadowFrameID;
	glGenFramebuffers(1, &shadowFrameID);

	int shadowWidth = 2048, shadowHeight = 2048;
	unsigned int shadowTextureID;
	glGenTextures(1, &shadowTextureID);
	glBindTexture(GL_TEXTURE_2D, shadowTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTextureID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float aspect = (float)shadowWidth / (float)shadowHeight;
	float near = 18.0f;
	float far = floorPos.y;

	glm::mat4 perspectiveProjection = glm::perspective(glm::radians(45.0f), aspect, near, far);
	glm::mat4 lightView = glm::lookAt(cubePos, glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 lightProjection = perspectiveProjection * lightView;

	shadowProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shadowProgram.ID, "u_LightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
	

	while (!glfwWindowShouldClose(window))
	{
		// Delta time
		currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		// Clear

		glClearColor(0.32f, 0.67f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update camera

		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 1000.0f);

		shaderProgram.Activate();
		camera.Matrix(shaderProgram, "camMatrix");
		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		cloudProgram.Activate();
		camera.Matrix(cloudProgram, "camMatrix");
		floorShader.Activate();
		camera.Matrix(floorShader, "camMatrix");

		// Shadows

		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, shadowWidth, shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameID);
		glClear(GL_DEPTH_BUFFER_BIT);

		shadowProgram.Activate();
		planeVAO.Bind();
		glUniformMatrix4fv(glGetUniformLocation(shadowProgram.ID, "u_Model"), 1, GL_FALSE, glm::value_ptr(planeModel));
		glDrawArrays(GL_TRIANGLES, 0, samolot.size());

		engineVAO.Bind();
		glUniformMatrix4fv(glGetUniformLocation(shadowProgram.ID, "u_Model"), 1, GL_FALSE, glm::value_ptr(engineModel));
		glDrawArrays(GL_TRIANGLES, 0, smiglo.size());

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);

		// Render floor

		floorShader.Activate();
		glBindTexture(GL_TEXTURE_2D, shadowTextureID);
		glUniform1i(glGetUniformLocation(floorShader.ID, "shadowMap"), 0);
		//Dzialaj.Bind(0);
		floorVAO.Bind();

		glUniformMatrix4fv(glGetUniformLocation(floorShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(floorModel));
		glUniform3f(glGetUniformLocation(floorShader.ID, "lightPos"), cubePos.x, cubePos.y, cubePos.z);
		glUniform3f(glGetUniformLocation(floorShader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glDrawElements(GL_TRIANGLES, sizeof(floorIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

		floorVAO.Unbind();

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
		currentAngle += 360 * deltaTime;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(engineModel));
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), cubePos.x, cubePos.y, cubePos.z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glDrawArrays(GL_TRIANGLES, 0, smiglo.size());
		engineVAO.Unbind();

		// Render clouds

		for (auto& chmurka: chmurki)
			chmurka.draw(cloudProgram, deltaTime);
		

		// Render light
		//circularMotion(deltaTime, cubePos);
		lightShader.Activate();
		lightVAO.Bind();

		cubeModel = glm::translate(glm::mat4(1.0f), cubePos);

		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

		// Swap buffers

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}