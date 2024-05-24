#include"VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO& VBO, GLuint layout, GLuint numComp, int k ,int n)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComp, GL_FLOAT, GL_FALSE, k * sizeof(float), (void*)(n * sizeof(float)));
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}