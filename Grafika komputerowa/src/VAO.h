#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:
	GLuint ID;
	VAO();
	~VAO();
	void LinkVBO(VBO& VBO, GLuint layout, GLuint numComp, GLsizei k, GLvoid* n);
	void Bind();
	void Unbind();
};
#endif