#include "Random.h"

glm::vec3& Random::GetPosition()
{
    glm::vec3 temp{};

    temp.x = GenerateNumber(-20.f, 20.f);
    temp.y = GenerateNumber(-5.f, 5.f);
    temp.z = GenerateNumber(-20.f, 20.f);

    return temp;
}

glm::vec3& Random::GetScale()
{
    glm::vec3 temp{};

    temp.x = GenerateNumber(0.3f, 0.8f);
    temp.y = GenerateNumber(0.3f, 0.8f);
    temp.z = GenerateNumber(0.3f, 0.8f);

    return temp;
}

glm::vec3& Random::GetRotation()
{
    glm::vec3 temp{};

    temp.x = GenerateNumber(0.0f, 1.0f);
    temp.y = GenerateNumber(0.0f, 1.0f);
    temp.z = GenerateNumber(0.0f, 1.0f);

    return temp;
}

float Random::GetAngle()
{
    float temp = GenerateNumber(0.0f, 360.0f);
    return temp;
}

float Random::GetAlpha()
{
    float temp = GenerateNumber(0.6f, 1.0f);
    return temp;
}

float Random::GetSpeed() 
{
    float temp = GenerateNumber(0.01f, 0.2f);
    return temp;
}

float Random::GenerateNumber(float min, float max)
{
    float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return min + random * (max - min);
}


