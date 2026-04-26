#pragma once
#include <glm.hpp>

struct GameObject
{
    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 rotation = glm::vec3(0.f);
    glm::vec3 scale = glm::vec3(1.f);
    glm::vec3 forward = glm::vec3(1.f, 0.f, 0.f);
    float fVelocity = 0.01f;
    float fAngularVel = 1.0f;
    float fScaleVel = 0.001f;
};
