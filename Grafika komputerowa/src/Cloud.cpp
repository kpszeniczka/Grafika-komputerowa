#include "cloud.h"


Cloud::Cloud() {
	static std::vector<Vertex> x = loadOBJ("res/model/chmurka.obj");
	cloudVertices = x;
	cloudVAO.Bind();
	cloudVBO.Init(cloudVertices);
	cloudVAO.LinkVBO(cloudVBO, 0, 3, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	cloudVAO.LinkVBO(cloudVBO, 2, 2, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	cloudVAO.LinkVBO(cloudVBO, 3, 3, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normals));

	cloudVAO.Unbind();
	cloudVBO.Unbind();
	cloudPos = Random::GetPosition();
	cloudScale = Random::GetScale();
	cloudRotation = Random::GetRotation();
	cloudAlpha = Random::GetAlpha();
	cloudAngle = Random::GetAngle();
	cloudSpeed = Random::GetSpeed();
}

void Cloud::draw(Shader& shader, float deltaTime)
{
	shader.Activate();
	cloudVAO.Bind();
	move(deltaTime, cloudSpeed);
	glm::mat4 cloudModel = glm::translate(glm::mat4(1.0f), cloudPos);
	cloudModel = glm::rotate(cloudModel, glm::radians(cloudAngle), cloudRotation);
	cloudModel = glm::scale(cloudModel, cloudScale);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(cloudModel));
	glUniform1f(glGetUniformLocation(shader.ID, "alpha"), cloudAlpha);
	glDrawArrays(GL_TRIANGLES, 0, cloudVertices.size());
	cloudVAO.Unbind();
}

void Cloud::move(float deltaTime, float speed) {
	cloudPos.x -= (deltaTime * speed);
	cloudPos.z -= (deltaTime * speed);
}