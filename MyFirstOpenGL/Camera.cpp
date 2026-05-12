#include "Camera.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "TimeManager.h"


glm::vec3 TROLL_MID = glm::vec3(0.f, 0.f, -0.75f);

void Camera::Update(double dt)
{
    InputManager* IM = InputManager::GetInstance();

    bool k1 = IM->IsKeyPressed(GLFW_KEY_1);
    bool k2 = IM->IsKeyPressed(GLFW_KEY_2);
    bool k3 = IM->IsKeyPressed(GLFW_KEY_3);

    // 1- Plano general del troll izquierdo
    if (k1)
    {
        if (mode == CameraMode::Key1)
        {
            mode = CameraMode::Orbit;
            fFov = 45.f;
        }
        else
        {
            mode = CameraMode::Key1;
            fFov = 60.f;
            camPos = glm::vec3(0.5f, 1.0f, 0.f);
            camTarget = glm::vec3(-0.8f, 0.4f, 0.f);
        }
    }
    // 2- Plano Detalle troll derecha
    if (k2)
    {
        if (mode == CameraMode::Key2)
        {
            mode = CameraMode::Orbit;
            fFov = 45.f;
        }
        else
        {
            mode = CameraMode::Key2;
            fFov = 20.f;
            camPos = glm::vec3(0.1f, 0.8f, 0.f);
            camTarget = glm::vec3(0.8f, 0.4f, 0.f);
        }
    }

    // 3- Dolly Zoom frente al troll del medio
    if (k3)
    {
        mode = CameraMode::DollyZoom;
        dollyTime = 0.f;
        fFov = 25.f;
    }

    // Actualizar Dolly Zoom
    if (mode == CameraMode::DollyZoom)
    {
        dollyTime += static_cast<float>(dt);
        float t = dollyTime / dollyDuration;

        if (t >= 1.f)
        {
            mode = CameraMode::Orbit;
            fFov = 45.f;
        }
        else
        {
            // Distancia inicial y la constante que mantiene el troll del mismo tamaño
            float startDist = 1.5f;
            float startFov = 25.f;

            // Constante que preserva el tamaño del troll en pantalla
            float k = startDist * glm::tan(glm::radians(startFov / 2.f));

            // Nueva distancia interpolada
            float dist = glm::mix(startDist, 6.0f, t);

            // FOV calculado matemáticamente para mantener el troll igual
            fFov = glm::degrees(2.f * glm::atan(k / dist));
            camPos = TROLL_MID + glm::vec3(0.f, 0.5f, dist);
            camTarget = TROLL_MID + glm::vec3(0.f, 0.4f, 0.f);
        }
    }
}

glm::mat4 Camera::GetViewMatrix()
{
    if (mode == CameraMode::Orbit)
    {
        const float radius = 3.f;
        float t = TimeManager::GetInstance()->GetCurrentTime();
        float camX = sin(t) * radius;
        float camY = 1.25f;
        float camZ = cos(t) * radius;
        return glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.f, 0.f, 0.f), localvectorUp);
    }
    else
    {
        return glm::lookAt(camPos, camTarget, localvectorUp);
    }
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(fFov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, fNear, fFar);
}