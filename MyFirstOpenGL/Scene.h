#pragma once
#include "GameObject.h"
#include "Model.h"
#include <vector>
#include "Camera.h"
#include "Flashlight.h"

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
    std::vector<RenderObject> trolls;
    std::vector<RenderObject> rocks;
    std::vector<RenderObject> cats;
    std::vector<RenderObject> woodenTowers;
    std::vector<RenderObject> skulls;
    RenderObject sun;
    RenderObject moon;
    Camera camera;
    GameObject floor;
    Flashlight flashlight;
    float sunAngle = 0.f;
    glm::vec3 sunDirection = glm::vec3(0.f, 1.f, 0.f);
    glm::vec3 ambientColor = glm::vec3(0.15f, 0.15f, 0.2f);
    float sunIntensity = 1.0f;
    float CYCLE_DURATION = 20.f;

    void Render();
    void DayNightCycle(float dt);
};
