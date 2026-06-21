#include "Camera.h"
#include "RenderManager.h"
#include "InputManager.h"

void Camera::Update(double dt)
{
    InputManager* IM = InputManager::GetInstance();
    float deltaTime = static_cast<float>(dt);
    float mouseX = IM->GetMouseX();
    float mouseY = IM->GetMouseY();

    if (firstMouse)
    {
        lastMouseX = mouseX;
        lastMouseY = mouseY;
        firstMouse = false;
    }

    float offsetX = (mouseX - lastMouseX) * mouseSensitivity;
    float offsetY = (lastMouseY - mouseY) * mouseSensitivity; 

    lastMouseX = mouseX;
    lastMouseY = mouseY;

    //Calculo de la posicion de la camara

    yaw += offsetX;
    pitch += offsetY;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camFront = glm::normalize(front);

   
    glm::vec3 right = glm::normalize(glm::cross(camFront, camUp));

    //Utilizamos Input Manager para el movimiento de la camara
    if (IM->currentKeys[GLFW_KEY_W])
        camPos += camFront * moveSpeed * deltaTime;

    if (IM->currentKeys[GLFW_KEY_S])
        camPos -= camFront * moveSpeed * deltaTime;

    if (IM->currentKeys[GLFW_KEY_A])
        camPos -= right * moveSpeed * deltaTime;

    if (IM->currentKeys[GLFW_KEY_D])
        camPos += right * moveSpeed * deltaTime;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(camPos, camPos + camFront, camUp);
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(fFov),(float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,fNear,fFar);
}