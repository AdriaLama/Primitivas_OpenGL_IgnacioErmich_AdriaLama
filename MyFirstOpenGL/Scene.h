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
    std::vector<RenderObject> trolls;
    std::vector<RenderObject> rocks;
    Camera camera;
    GameObject floor;

    void Render();
};
