#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H
#include<glad/glad.h>
#include<stb/stb_image.h>
#include"shaderClass.h"
class Texture
{
public:
	GLuint ID;
	GLenum type;
	Texture(const char* image, GLenum texType, int slot, GLenum format, GLenum
		pixelType);
	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind(int slot = 0);
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	~Texture();
};
#endif