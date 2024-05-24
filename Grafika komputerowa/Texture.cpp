#include"Texture.h"
Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum
	pixelType)
{
	type = texType;
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);
	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(texType, ID);
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(texType, 0, GL_RGB, widthImg, heightImg, 0, format, pixelType,bytes);
	glGenerateMipmap(texType);
	stbi_image_free(bytes);
	glBindTexture(texType, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}
void Texture::Bind()
{
	glBindTexture(type, ID);
}
void Texture::Unbind()
{
	glBindTexture(type, 0);
}
void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}