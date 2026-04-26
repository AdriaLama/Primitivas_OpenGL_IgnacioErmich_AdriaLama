#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <vector>

#include "Shader.h"

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

    void DrawCube(const glm::mat4& transform, bool visible);
    void DrawOrtho(const glm::mat4& transform, bool visible);
    void DrawPyramid(const glm::mat4& transform, bool visible, float tiempo);

    void SetWireframe(bool enabled);

    GLFWwindow* GetWindow() const;
    GLuint GetProgram() const;

private:
    RenderManager() = default;
    static RenderManager* instance;

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* window = nullptr;
    GLuint program = 0;

    GLuint vaoCube = 0, vboCube = 0;
    GLuint vaoOrtho = 0, vboOrtho = 0;
    GLuint vaoPyramid = 0, vboPyramid = 0;

    void SetupCubeBuffers();
    void SetupOrthoBuffers();
    void SetupPyramidBuffers();
    void SetupShaders();
};
