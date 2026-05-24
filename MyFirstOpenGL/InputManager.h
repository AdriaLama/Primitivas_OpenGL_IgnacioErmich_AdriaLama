#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class InputManager
{
public:
    static InputManager* GetInstance();

    void Update(GLFWwindow* window);
    bool IsKeyPressed(int key) const;

    float GetMouseX() const { return mouseX; }
    float GetMouseY() const { return mouseY; }

    bool currentKeys[GLFW_KEY_LAST] = {};
    bool previousKeys[GLFW_KEY_LAST] = {};

private:
    InputManager() = default;
    static InputManager* instance;

    float mouseX = 0.f;
    float mouseY = 0.f;
};