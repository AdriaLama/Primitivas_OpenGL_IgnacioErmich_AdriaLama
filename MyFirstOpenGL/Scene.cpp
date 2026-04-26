#include "Scene.h"
#include "RenderManager.h"
#include "Transform.h"
#include <GLFW/glfw3.h>

void Scene::Setup()
{
    cube.position = glm::vec3(-0.55f, 0.f, 0.f);
    ortho.position = glm::vec3(0.0f, 0.f, 0.f);
    pyramid.position = glm::vec3(0.55f, 0.f, 0.f);

    cube.forward = glm::vec3(0.f, 1.f, 0.f);
    pyramid.forward = glm::vec3(0.f, 1.f, 0.f);

    cube.scale = glm::vec3(0.3f, 0.3f, 0.3f);
    ortho.scale = glm::vec3(0.15f, 0.27f, 0.15f);
    pyramid.scale = glm::vec3(0.3f, 0.3f, 0.3f);
}

void Scene::Update(float dt)
{
    tiempo = static_cast<float>(glfwGetTime());

    HandleInput();

    if (bPaused) return;

    UpdateCube();
    UpdateOrtho();
    UpdatePyramid();
    Render();
}

// Gestion de los inputs

void Scene::HandleInput()
{
    GLFWwindow* window = RenderManager::GetInstance()->GetWindow();

    // Tecla 1 - wireframe
    bool b1IsPressed = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
    if (b1IsPressed && !b1WasPressed)
    {
        wireframe = !wireframe;
        RenderManager::GetInstance()->SetWireframe(wireframe);
    }
    b1WasPressed = b1IsPressed;

    // Espacio - pausa
    bool bSpaceIsPressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    if (bSpaceIsPressed && !bSpaceWasPressed)
        bPaused = !bPaused;
    bSpaceWasPressed = bSpaceIsPressed;

    // M - acelerar
    bool bMIsPressed = glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS;
    if (bMIsPressed && !bMWasPressed)
    {
        cube.fVelocity += cube.fVelocity * 0.10f;
        cube.fAngularVel += cube.fAngularVel * 0.10f;
        ortho.fScaleVel += ortho.fScaleVel * 0.10f;
        ortho.fAngularVel += ortho.fAngularVel * 0.10f;
        pyramid.fVelocity += pyramid.fVelocity * 0.10f;
        pyramid.fAngularVel += pyramid.fAngularVel * 0.10f;
    }
    bMWasPressed = bMIsPressed;

    // N - desacelerar
    bool bNIsPressed = glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS;
    if (bNIsPressed && !bNWasPressed)
    {
        cube.fVelocity -= cube.fVelocity * 0.10f;
        cube.fAngularVel -= cube.fAngularVel * 0.10f;
        ortho.fScaleVel -= ortho.fScaleVel * 0.10f;
        ortho.fAngularVel -= ortho.fAngularVel * 0.10f;
        pyramid.fVelocity -= pyramid.fVelocity * 0.10f;
        pyramid.fAngularVel -= pyramid.fAngularVel * 0.10f;
    }
    bNWasPressed = bNIsPressed;

    // Tecla 2 - toggle cubo
    bool b2IsPressed = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
    if (b2IsPressed && !b2WasPressed) showCube = !showCube;
    b2WasPressed = b2IsPressed;

    // Tecla 3 - toggle ortoedro
    bool b3IsPressed = glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS;
    if (b3IsPressed && !b3WasPressed) showOrtho = !showOrtho;
    b3WasPressed = b3IsPressed;

    // Tecla 4 - toggle piramide
    bool b4IsPressed = glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS;
    if (b4IsPressed && !b4WasPressed) showPyramid = !showPyramid;
    b4WasPressed = b4IsPressed;
}


//Update posiciones y comportamiento cubo
void Scene::UpdateCube()
{
    cube.position = cube.position + cube.forward * cube.fVelocity;
    cube.rotation = cube.rotation + glm::vec3(0.f, 1.f, 0.f) * cube.fAngularVel;

    if (cube.position.y >= 0.7f || cube.position.y <= -0.7f)
        cube.forward = cube.forward * -1.f;
}
//Update posiciones y comportamiento ortoedro
void Scene::UpdateOrtho()
{
    ortho.rotation = ortho.rotation + glm::vec3(0.f, 0.f, 1.f) * ortho.fAngularVel;
    ortho.scale = ortho.scale + glm::vec3(1.f, 0.f, 0.f) * ortho.fScaleVel;

    if (ortho.scale.x >= 0.3f || ortho.scale.x <= 0.05f)
        ortho.fScaleVel = ortho.fScaleVel * -1.f;
}
//Update posiciones y comportamiento piramide
void Scene::UpdatePyramid()
{
    pyramid.position = pyramid.position + pyramid.forward * pyramid.fVelocity;
    pyramid.rotation = pyramid.rotation + glm::vec3(1.f, 1.f, 0.f) * pyramid.fAngularVel;

    if (pyramid.position.y >= 0.7f || pyramid.position.y <= -0.7f)
        pyramid.forward = pyramid.forward * -1.f;
}

void Scene::Render()
{
    RenderManager* RM = RenderManager::GetInstance();

    // Cubo
    glm::mat4 cubeModel = GenerateTranslationMatrix(cube.position) * GenerateRotationMatrix(glm::vec3(0.f, 1.f, 0.f), cube.rotation.y) * GenerateScaleMatrix(cube.scale);
    RM->DrawCube(cubeModel, showCube);

    // Ortoedro
    glm::mat4 orthoModel = GenerateTranslationMatrix(ortho.position) * GenerateRotationMatrix(glm::vec3(0.f, 0.f, 1.f), ortho.rotation.z) * GenerateScaleMatrix(ortho.scale);
    RM->DrawOrtho(orthoModel, showOrtho);

    // Piramide
    glm::mat4 pyramidModel = GenerateTranslationMatrix(pyramid.position) * GenerateRotationMatrix(glm::vec3(1.f, 0.f, 0.f), pyramid.rotation.x) * GenerateRotationMatrix(glm::vec3(0.f, 1.f, 0.f), pyramid.rotation.y)* GenerateScaleMatrix(pyramid.scale);
    RM->DrawPyramid(pyramidModel, showPyramid, tiempo);
}