#pragma once
#include "GameObject.h"

class Scene
{
public:
    void Setup();
    void Update(float dt);

private:
    GameObject cube;
    GameObject ortho;
    GameObject pyramid;

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
