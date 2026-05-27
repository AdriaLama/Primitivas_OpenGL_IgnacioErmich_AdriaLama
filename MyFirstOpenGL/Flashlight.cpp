#include "Flashlight.h"
#include "InputManager.h"
#include <gtc/type_ptr.hpp>

void Flashlight::Update(const glm::vec3& camPos, const glm::vec3& camFront, float dt)
{
    position = camPos;
    direction = camFront;

    InputManager* IM = InputManager::GetInstance();

    if (IM->IsKeyPressed(GLFW_KEY_F))
    {
        isOn = !isOn;
    }
}


void Flashlight::SendToShader(GLuint program) const
{
    glUniform1i(glGetUniformLocation(program, "flashlightOn"), isOn ? 1 : 0);

    glUniform3f(glGetUniformLocation(program, "flashlightPos"), position.x, position.y, position.z);

    glUniform3f(glGetUniformLocation(program, "flashlightDir"), direction.x, direction.y, direction.z);

    glUniform1f(glGetUniformLocation(program, "flashlightInnerCone"),cos(glm::radians(innerConeAngle)));

    glUniform1f(glGetUniformLocation(program, "flashlightOuterCone"),cos(glm::radians(outerConeAngle)));

    glUniform1f(glGetUniformLocation(program, "flashlightRange"), range);

    glUniform1f(glGetUniformLocation(program, "flashlightIntensity"), intensity);

}