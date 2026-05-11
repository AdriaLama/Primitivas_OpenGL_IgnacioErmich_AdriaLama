#pragma once
#include "GameObject.h"
#include "Model.h"
#include <vector>

class Scene
{
public:
    void Setup();
    void Update(float dt);

private:

    std::vector<Model> models;
    GameObject cube;
    GameObject ortho;
    GameObject pyramid;
    GLuint trollTexture;
    GLuint rockTexture;

    glm::vec3 trollPositions[3];
    float trollRotations[3];

    bool showCube = true;
    bool showOrtho = true;
    bool showPyramid = true;
    bool bPaused = false;
    bool wireframe = false;

    // Estado anterior de teclas 
    bool b1WasPressed = false;
    bool b2WasPressed = false;
    bool b3WasPressed = false;
    bool b4WasPressed = false;
    bool bSpaceWasPressed = false;
    bool bMWasPressed = false;
    bool bNWasPressed = false;

    float tiempo = 0.f;

    void HandleInput();
    void UpdateCube();
    void UpdateOrtho();
    void UpdatePyramid();
    void Render();
};
