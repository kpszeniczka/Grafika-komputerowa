#ifndef PLANE_H
#define PLANE_H

#include "shaderClass.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

class Plane {
private:
	glm::vec3 Position;
	glm::vec2 Texture;
	float AngularSpeed;
	glm::vec3 m_RotationAxis;
public:
	void Draw(Shader& shader);
	glm::vec3& GetPosition() { return Position; };
	glm::vec2& GetTexture() { return Texture; };
};
#endif 