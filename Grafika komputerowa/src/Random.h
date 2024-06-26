#pragma once

#include <iostream>
#include <random>
#include <glm/glm.hpp>

class Random
{
public:
	static glm::vec3& GetPosition();
	static glm::vec3& GetScale();
	static glm::vec3& GetRotation();
	static float GetAngle();
	static float GetAlpha();
	static float GetSpeed();
private:
	Random();
	static float GenerateNumber(float min, float max);
};

