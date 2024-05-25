#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normals;
};

static std::vector<Vertex> loadOBJ(const char* file_name) {
	std::vector<glm::fvec3> vertexPosition;
	std::vector<glm::fvec2> vertexTexcoord;
	std::vector<glm::fvec3> vertexNormals;

	std::vector<GLuint> vertexPositionIndicies;
	std::vector<GLuint> vertexTexcoordIndicies;
	std::vector<GLuint> vertexNormalsIndicies;

	std::vector<Vertex> vertices;
	std::stringstream ss;
	std::ifstream inFile(file_name);
	std::string line = "";
	std::string prefix = "";
	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	GLint temp_glint = 0;

	if (!inFile.is_open()) {
		throw "error";
	}

	while (std::getline(inFile, line)) {
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "v") {
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertexPosition.push_back(temp_vec3);
		}
		else if (prefix == "vt") {
			ss >> temp_vec2.x >> temp_vec2.y;
			vertexTexcoord.push_back(temp_vec2);
		}
		else if (prefix == "vn") {
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertexNormals.push_back(temp_vec3);
		}
		else if (prefix == "f") {
			int counter = 0;
			while (ss >> temp_glint) {
				if (counter == 0) {
					vertexPositionIndicies.push_back(temp_glint);
				}
				else if (counter == 1) {
					vertexTexcoordIndicies.push_back(temp_glint);
				}
				else if (counter == 2) {
					vertexNormalsIndicies.push_back(temp_glint);
				}
				if (ss.peek() == '/') {
					++counter;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ') {
					++counter;
					ss.ignore(1, ' ');
				}
				if (counter > 2) {
					counter = 0;
				}
			}
		}
		else {

		}
	}
	vertices.resize(vertexPositionIndicies.size(), Vertex());

	for (size_t i = 0; i < vertexPositionIndicies.size(); i++)
	{
		vertices[i].position = vertexPosition[vertexPositionIndicies[i] - 1];
		vertices[i].texcoord = vertexTexcoord[vertexTexcoordIndicies[i] - 1];
		vertices[i].normals = vertexNormals[vertexNormalsIndicies[i] - 1];
	}
	return vertices;
}