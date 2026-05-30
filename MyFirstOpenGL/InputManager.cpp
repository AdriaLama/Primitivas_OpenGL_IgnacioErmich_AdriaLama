#include "InputManager.h"

InputManager* InputManager::instance = nullptr;

InputManager* InputManager::GetInstance()
{
    if (instance == nullptr)
        instance = new InputManager();
    return instance;
}

void InputManager::Update(GLFWwindow* window)
{
    for (int key = 0; key < GLFW_KEY_LAST; key++)
        currentKeys[key] = glfwGetKey(window, key) == GLFW_PRESS;

    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    mouseX = static_cast<float>(mx);
    mouseY = static_cast<float>(my);
}

bool InputManager::IsKeyPressed(int key) const
{
    return currentKeys[key];
}