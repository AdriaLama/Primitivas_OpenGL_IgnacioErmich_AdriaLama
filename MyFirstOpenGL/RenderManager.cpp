#include "RenderManager.h"
#include <gtc/type_ptr.hpp>
#include <iostream>

RenderManager* RenderManager::instance = nullptr;

RenderManager* RenderManager::GetInstance()
{
    if (instance == nullptr)
        instance = new RenderManager();
    return instance;
}

void RenderManager::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    glUniform2f(glGetUniformLocation(GetInstance()->program, "windowSize"), width, height);
}

bool RenderManager::Init()
{
    srand(static_cast<unsigned int>(time(NULL)));

    glfwInit();
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Primitivas 3D", NULL, NULL);
    if (!window) return false;

    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return false;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    SetupShaders();
    SetupCubeBuffers();
    SetupOrthoBuffers();
    SetupPyramidBuffers();

    glUseProgram(program);
    glUniform2f(glGetUniformLocation(program, "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);

    return true;
}

void RenderManager::Shutdown()
{
    glUseProgram(0);
    glDeleteProgram(program);
    glfwTerminate();
}

bool RenderManager::WindowShouldClose() const
{
    return glfwWindowShouldClose(window);
}

void RenderManager::PollEvents()
{
    glfwPollEvents();
}

void RenderManager::ClearBuffers()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void RenderManager::SwapBuffers()
{
    glFlush();
    glfwSwapBuffers(window);
}

void RenderManager::SetWireframe(bool enabled)
{
    glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
}

void RenderManager::DrawCube(const glm::mat4& transform, bool visible)
{
    glm::mat4 model = glm::mat4(visible ? 1.0f : 0.f) * transform;
    glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(glGetUniformLocation(program, "objectID"), 0);
    glBindVertexArray(vaoCube);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
    glBindVertexArray(0);
}

void RenderManager::DrawOrtho(const glm::mat4& transform, bool visible)
{
    glm::mat4 model = glm::mat4(visible ? 1.0f : 0.f) * transform;
    glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(glGetUniformLocation(program, "objectID"), 1);
    glBindVertexArray(vaoOrtho);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
    glBindVertexArray(0);
}

void RenderManager::DrawPyramid(const glm::mat4& transform, bool visible, float tiempo)
{
    glm::mat4 model = glm::mat4(visible ? 1.0f : 0.f) * transform;
    glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(glGetUniformLocation(program, "objectID"), 2);
    glUniform1f(glGetUniformLocation(program, "tiempo"), tiempo);
    glBindVertexArray(vaoPyramid);
    glDrawArrays(GL_TRIANGLES, 0, 18);
    glBindVertexArray(0);
}

GLFWwindow* RenderManager::GetWindow() const { return window; }
GLuint RenderManager::GetProgram() const { return program; }


void RenderManager::SetupShaders()
{
    ShaderProgram sp;
    sp.vertexShader = LoadVertexShader("MyFirstVertexShader.glsl");
    sp.geometryShader = LoadGeometryShader("MyFirstGeometryShader.glsl");
    sp.fragmentShader = LoadFragmentShader("MyFirstFragmentShader.glsl");
    program = CreateProgram(sp);
}

void RenderManager::SetupCubeBuffers()
{
    GLfloat cubeVertices[] =
    {
        -0.5f, +0.5f, -0.5f,
        +0.5f, +0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        +0.5f, -0.5f, -0.5f,
        +0.5f, -0.5f, +0.5f,
        +0.5f, +0.5f, -0.5f,
        +0.5f, +0.5f, +0.5f,
        -0.5f, +0.5f, -0.5f,
        -0.5f, +0.5f, +0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, +0.5f,
        +0.5f, -0.5f, +0.5f,
        -0.5f, +0.5f, +0.5f,
        +0.5f, +0.5f, +0.5f,
    };

    glGenVertexArrays(1, &vaoCube);
    glBindVertexArray(vaoCube);
    glGenBuffers(1, &vboCube);
    glBindBuffer(GL_ARRAY_BUFFER, vboCube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RenderManager::SetupOrthoBuffers()
{
    GLfloat orthoVertices[] =
    {
        -0.5f, +0.5f, -0.5f,
        +0.5f, +0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        +0.5f, -0.5f, -0.5f,
        +0.5f, -0.5f, +0.5f,
        +0.5f, +0.5f, -0.5f,
        +0.5f, +0.5f, +0.5f,
        -0.5f, +0.5f, -0.5f,
        -0.5f, +0.5f, +0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, +0.5f,
        +0.5f, -0.5f, +0.5f,
        -0.5f, +0.5f, +0.5f,
        +0.5f, +0.5f, +0.5f,
    };

    glGenVertexArrays(1, &vaoOrtho);
    glBindVertexArray(vaoOrtho);
    glGenBuffers(1, &vboOrtho);
    glBindBuffer(GL_ARRAY_BUFFER, vboOrtho);
    glBufferData(GL_ARRAY_BUFFER, sizeof(orthoVertices), orthoVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RenderManager::SetupPyramidBuffers()
{
    GLfloat pyramidVertices[] =
    {
         0.0f, +0.5f,  0.0f,
        -0.5f, -0.5f, +0.5f,
        +0.5f, -0.5f, +0.5f,

         0.0f, +0.5f,  0.0f,
        +0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

         0.0f, +0.5f,  0.0f,
        +0.5f, -0.5f, +0.5f,
        +0.5f, -0.5f, -0.5f,

         0.0f, +0.5f,  0.0f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, +0.5f,

        -0.5f, -0.5f, -0.5f,
        +0.5f, -0.5f, -0.5f,
        +0.5f, -0.5f, +0.5f,
        -0.5f, -0.5f, -0.5f,
        +0.5f, -0.5f, +0.5f,
        -0.5f, -0.5f, +0.5f,
    };

    glGenVertexArrays(1, &vaoPyramid);
    glBindVertexArray(vaoPyramid);
    glGenBuffers(1, &vboPyramid);
    glBindBuffer(GL_ARRAY_BUFFER, vboPyramid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}