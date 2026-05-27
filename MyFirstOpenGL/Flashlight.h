#pragma once
#pragma once
#include <GL/glew.h>
#include <glm.hpp>

class Flashlight
{
public:

    void Update(const glm::vec3& camPos, const glm::vec3& camFront, float dt);
    void SendToShader(GLuint program) const;

private:

    // Posicion y direccion
    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);

    // Angulos de los conos en radianes
    float innerConeAngle = 12.5f;
    float outerConeAngle = 20.0f;

    // Alcance maximo de la luz
    float range = 10.f;

    //Intensidad de la luz
    float intensity = 12.5f;

    // Estado on/off
    bool isOn = false;

    bool fKeyPressed = false;
};