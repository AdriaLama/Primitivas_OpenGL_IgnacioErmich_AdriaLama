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
    void InitSpawnPoints();
    void LoadModels();
    void SpawnObjects();
    void RandomizeObjects();
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
    glm::vec3 sunDirection;
    glm::vec3 ambientColor;
    float sunIntensity;
    float CYCLE_DURATION = 20.f;

    void Render();
    void DayNightCycle(float dt);
    void RenderFloor(glm::mat4 projection, glm::mat4 view);
    void RenderObjects(glm::mat4 projection, glm::mat4 view);
    void RenderSkyObjects(glm::mat4 projection, glm::mat4 view);
};
