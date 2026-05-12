#include "Camera.h"
#include "RenderManager.h"

// Posiciones de los trolls¡
glm::vec3 TROLL_LEFT = glm::vec3(-0.8f, 0.f, 0.f);
glm::vec3 TROLL_MID = glm::vec3(0.f, 0.f, -0.75f);
glm::vec3 TROLL_RIGHT = glm::vec3(0.8f, 0.f, 0.f);

void Camera::Update(double dt)
{
    GLFWwindow* window = RenderManager::GetInstance()->GetWindow();

    bool k1 = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
    bool k2 = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
    bool k3 = glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS;

    // Tecla 1 → plano general del troll izquierdo
    if (k1 && !prevKey1)
    {
        mode = CameraMode::Key1;
        fFov = 60.f;
        camPos = glm::vec3(0.5f, 1.0f, 0.f);   // frente a su cara (lado +X)
        camTarget = glm::vec3(-0.8f, 0.4f, 0.f);  // apunta al troll izquierdo
    }

    // Troll derecho mira hacia -X → cámara a su izquierda (X negativo respecto a él)
    if (k2 && !prevKey2)
    {
        mode = CameraMode::Key2;
        fFov = 20.f;
        camPos = glm::vec3(0.1f, 0.8f, 0.f);   // frente a su cara (lado -X)
        camTarget = glm::vec3(0.8f, 0.4f, 0.f);   // apunta al troll derecho
    }

    // Tecla 3 → Dolly Zoom frente al troll del medio
    if (k3 && !prevKey3)
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

    prevKey1 = k1;
    prevKey2 = k2;
    prevKey3 = k3;
}

glm::mat4 Camera::GetViewMatrix()
{
    if (mode == CameraMode::Orbit)
    {
        // Orbita original — sin tocar
        const float radius = 3.f;
        float camX = sin(glfwGetTime()) * radius;
        float camY = 1.25f;
        float camZ = cos(glfwGetTime()) * radius;
        return glm::lookAt(glm::vec3(camX, camY, camZ),
            glm::vec3(0.f, 0.f, 0.f),
            localvectorUp);
    }
    else
    {
        return glm::lookAt(camPos, camTarget, localvectorUp);
    }
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(fFov),
        (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
        fNear, fFar);
}