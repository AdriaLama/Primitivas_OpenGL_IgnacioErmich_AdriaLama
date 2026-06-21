#include "Flashlight.h"
#include "InputManager.h"
#include <gtc/type_ptr.hpp>

void Flashlight::Update(const glm::vec3& camPos, const glm::vec3& camFront, float dt)
{
    position = camPos;
    direction = camFront;

    InputManager* IM = InputManager::GetInstance();

    //On/Off de la camara

    static bool wasPressed = false;
    bool isPressed = IM->IsKeyPressed(GLFW_KEY_F); 

    if (isPressed && !wasPressed)
        isOn = !isOn;

    wasPressed = isPressed;
}


void Flashlight::SendToShader(GLuint program) const
{

    //Enviamos variables para la linterna al fragment

    glUniform1i(glGetUniformLocation(program, "flashlightOn"), isOn ? 1 : 0);

    glUniform3f(glGetUniformLocation(program, "flashlightPos"), position.x, position.y, position.z);

    glUniform3f(glGetUniformLocation(program, "flashlightDir"), direction.x, direction.y, direction.z);

    glUniform1f(glGetUniformLocation(program, "flashlightInnerCone"),cos(glm::radians(innerConeAngle)));

    glUniform1f(glGetUniformLocation(program, "flashlightOuterCone"),cos(glm::radians(outerConeAngle)));

    glUniform1f(glGetUniformLocation(program, "flashlightRange"), range);


}