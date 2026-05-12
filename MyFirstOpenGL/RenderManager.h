#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <vector>
#include <string>

#include "Shader.h"
#include "Model.h"

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

class RenderManager
{
public:
    static RenderManager* GetInstance();

    bool Init();
    void Shutdown();

    bool WindowShouldClose() const;
    void PollEvents();
    void ClearBuffers();
    void SwapBuffers();

    void DrawFloor(const glm::mat4& transform);

    void DrawModel(const Model& model, const glm::mat4& transform, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);

    GLuint LoadTexture(const std::string& filePath);

    void SetWireframe(bool enabled);

    GLFWwindow* GetWindow() const;
    GLuint GetProgram() const;

    void SetColor(const glm::vec4& color);

private:
    RenderManager() = default;
    static RenderManager* instance;

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* window = nullptr;
    GLuint program = 0;
    GLuint texture = 0;   
    GLuint vaoFloor = 0, vboFloor = 0;

    void SetupShaders();
    void SetupFloorBuffers();
};