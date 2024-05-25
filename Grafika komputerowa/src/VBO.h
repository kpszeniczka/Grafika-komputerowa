#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>
#include"OBJLoader.h"

class VBO
{
public:
	GLuint ID;
	VBO(const std::vector<Vertex>& vertices);
	VBO(const float* vertices, GLsizeiptr size);
	~VBO();
	void Bind();
	void Unbind();
};

#endif