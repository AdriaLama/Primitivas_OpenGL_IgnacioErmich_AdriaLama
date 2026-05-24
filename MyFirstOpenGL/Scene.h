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
    std::vector<glm::vec3> spawnPoints;
    int numSpawnPoints;
    int randomRotation;
    int randomScale;
    std::vector<RenderObject> trolls;
    std::vector<RenderObject> rocks;
    std::vector<RenderObject> cats;
    std::vector<RenderObject> woodenTowers;
    std::vector<RenderObject> skulls;
    Camera camera;
    GameObject floor;

    void Render();
};
