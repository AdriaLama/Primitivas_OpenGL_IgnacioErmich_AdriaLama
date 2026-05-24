#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

class Camera
{
public:
    void Update(double dt);
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();

private:
    // Posicion y orientacion
    glm::vec3 camPos = glm::vec3(0.f, 1.f, 3.f);
    glm::vec3 camFront = glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 camUp = glm::vec3(0.f, 1.f, 0.f);

    // Angulos (grados)
    float yaw = -90.f;  // empieza mirando hacia -Z
    float pitch = 0.f;

    // Sensibilidad del raton y velocidad de movimiento
    float mouseSensitivity = 0.1f;
    float moveSpeed = 3.f;

    // Proyeccion
    float fFov = 45.f;
    float fNear = 0.1f;
    float fFar = 100.f;

    // Evita el salto de camara al capturar el raton por primera vez
    bool  firstMouse = true;
    float lastMouseX = 0.f;
    float lastMouseY = 0.f;
};