#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:
	GLuint ID;
	VAO();

	void LinkVBO(VBO& VBO, GLuint layout, GLuint numComp, int k, int n);
	void Bind();
	void Unbind();
	void Delete();
};
#endif