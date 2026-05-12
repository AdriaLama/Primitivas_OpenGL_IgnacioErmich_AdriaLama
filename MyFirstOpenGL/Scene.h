#pragma once
#include "GameObject.h"
#include "Model.h"
#include <vector>
#include "Camera.h"

struct RenderObject         
{
    Model* model;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec4 color;
};

class Scene
{
public:
    void Setup();
    void Update(float dt);

private:

    std::vector<Model> models;
   /* GameObject cube;
    GameObject ortho;
    GameObject pyramid;*/
    std::vector<RenderObject> trolls;
    std::vector<RenderObject> rocks;
    Camera camera;
    GameObject floor;

   /* bool showCube = true;
    bool showOrtho = true;
    bool showPyramid = true;
    bool bPaused = false;
    bool wireframe = false;*/

    // Estado anterior de teclas 
   /* bool b1WasPressed = false;
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
    void UpdatePyramid();*/
    void Render();
};
