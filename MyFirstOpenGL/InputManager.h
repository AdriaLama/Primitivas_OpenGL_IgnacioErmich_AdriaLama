#pragma once
#include <GLFW/glfw3.h>

class InputManager
{
public:
    static InputManager* GetInstance();

    void Update(GLFWwindow* window);

    bool IsKeyPressed(int key) const;   

private:
    static InputManager* instance;

    bool currentKeys[GLFW_KEY_LAST] = {};
    bool previousKeys[GLFW_KEY_LAST] = {};
};