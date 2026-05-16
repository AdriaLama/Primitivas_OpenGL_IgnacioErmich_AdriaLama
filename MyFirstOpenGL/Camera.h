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
    glm::vec3 localvectorUp = glm::vec3(0.f, 1.f, 0.f);

    enum class CameraMode { Orbit, Key1, Key2, DollyZoom };
    CameraMode mode = CameraMode::Orbit;

    bool isPerspectiveActive = true;
    float fFov = 45.f;
    float fNear = 0.1f;
    float fFar = 100.f;

    glm::vec3 camPos = glm::vec3(0.f);
    glm::vec3 camTarget = glm::vec3(0.f);

    float dollyTime = 0.f;
    float dollyDuration = 3.f;   
    float dollyStartDist = 1.5f;
    float dollyEndDist = 4.0f;
    float dollyTargetHeight = 0.4f; 
};