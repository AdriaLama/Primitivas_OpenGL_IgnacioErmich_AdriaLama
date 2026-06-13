#include "Scene.h"
#include "RenderManager.h"
#include "Transform.h"
#include <cstdlib> 
#include <ctime>   


void Scene::InitSpawnPoints()
{
    spawnPoints = {
        glm::vec3(-1.f,  0.f,  1.f),
        glm::vec3(0.f,  0.f,  1.f),
        glm::vec3(1.f,  0.f,  1.f),
        glm::vec3(-1.f,  0.f,  0.f),
        glm::vec3(0.f,  0.f,  0.f),
        glm::vec3(1.f,  0.f,  0.f),
        glm::vec3(-1.f,  0.f, -1.f),
        glm::vec3(0.f,  0.f, -1.f),
        glm::vec3(1.f,  0.f, -1.f),
    };

    numSpawnPoints = spawnPoints.size();

    for (int i = numSpawnPoints - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        glm::vec3 temp = spawnPoints[i];
        spawnPoints[i] = spawnPoints[j];
        spawnPoints[j] = temp;
    }
}

void Scene::LoadModels()
{
    models.push_back(LoadOBJModel("Assets/Models/troll.obj"));
    models.push_back(LoadOBJModel("Assets/Models/cat.obj"));
    models.push_back(LoadOBJModel("Assets/Models/woodenTower.obj"));
    models.push_back(LoadOBJModel("Assets/Models/skull.obj"));
    models.push_back(LoadOBJModel("Assets/Models/sun.obj"));
    models.push_back(LoadOBJModel("Assets/Models/moon.obj"));

    models[0].textureID = RenderManager::GetInstance()->LoadTexture("Assets/Textures/troll.png");
    models[1].textureID = RenderManager::GetInstance()->LoadTexture("Assets/Textures/cat.jpg");
    models[2].textureID = RenderManager::GetInstance()->LoadTexture("Assets/Textures/woodenTower.jpg");
    models[3].textureID = RenderManager::GetInstance()->LoadTexture("Assets/Textures/skull.jpg");
    models[4].textureID = RenderManager::GetInstance()->LoadTexture("Assets/Textures/sun.jpg");
    models[5].textureID = RenderManager::GetInstance()->LoadTexture("Assets/Textures/moon.png");

    dayNightTextureID = RenderManager::GetInstance()->LoadTexture("Assets/Textures/DayNightCycle.png");
}

void Scene::SpawnObjects()
{
    int idx = 0;

    trolls.push_back({ &models[0], spawnPoints[idx++],  glm::vec3(0.f, 0.f, 0.f),   glm::vec3(0.f), glm::vec4(0.2f, 1.0f, 0.3f, 1.f) });
    trolls.push_back({ &models[0], spawnPoints[idx++], glm::vec3(0.f, 0.f, 0.f),    glm::vec3(0.f), glm::vec4(0.2f, 0.5f, 0.5f, 1.f) });
    trolls.push_back({ &models[0], spawnPoints[idx++],   glm::vec3(0.f, 0.f, 0.f),  glm::vec3(0.f), glm::vec4(0.2f, 1.f, 1.0f, 1.f) });

    cats.push_back({ &models[1], spawnPoints[idx++],  glm::vec3(-90.f, 0.f, 0.f),  glm::vec3(0.f, 0.f, 0.f), glm::vec4(1.f, 1.f, 1.f, 1.f) });

    woodenTowers.push_back({ &models[2], spawnPoints[idx++],  glm::vec3(0.f, 0.f, 0.f),  glm::vec3(0.f, 0.f, 0.f), glm::vec4(1.f, 1.f, 1.f, 1.f) });

    skulls.push_back({ &models[3], spawnPoints[idx++],  glm::vec3(-90.f, 0.f, 0.f),  glm::vec3(0.f, 0.f, 0.f), glm::vec4(1.f, 1.f, 1.f, 1.f) });
    skulls.push_back({ &models[3], spawnPoints[idx++],  glm::vec3(-90.f, 0.f, 0.f),  glm::vec3(0.f, 0.f, 0.f), glm::vec4(1.f, 1.f, 1.f, 1.f) });
    skulls.push_back({ &models[3], spawnPoints[idx++],  glm::vec3(-90.f, 0.f, 0.f),  glm::vec3(0.f, 0.f, 0.f), glm::vec4(1.f, 1.f, 1.f, 1.f) });

    // Sol y luna no ocupan spawn points, tienen posicion fija
    sun = { &models[4], glm::vec3(-4.f, 1.5f, 0.f),  glm::vec3(-0.f, 0.f, 0.f),  glm::vec3(0.0010f, 0.0010f, 0.0010f), glm::vec4(1.f, 1.f, 1.f, 1.f) };
    moon = { &models[5], glm::vec3(4.f, 1.5f, 0.f),  glm::vec3(-0.f, 0.f, 0.f),  glm::vec3(0.25f, 0.25f, 0.25f), glm::vec4(1.f, 1.f, 1.f, 1.f) };
}

void Scene::RandomizeObjects()
{
    for (RenderObject& obj : trolls) {
        obj.rotation.y = rand() % 361;
        obj.scale = glm::vec3(0.2f + (rand() % 100) / 100.f * (0.5f - 0.2f));
    }
    for (RenderObject& obj : cats) {
        obj.rotation.z = rand() % 361;
        obj.scale = glm::vec3(0.015f + (rand() % 100) / 100.f * (0.015f - 0.02f));
    }
    for (RenderObject& obj : woodenTowers) {
        obj.rotation.y = rand() % 361;
        obj.scale = glm::vec3(0.15f + (rand() % 100) / 100.f * (0.15f - 0.20f));
    }
    for (RenderObject& obj : skulls) {
        obj.rotation.z = rand() % 361;
        obj.scale = glm::vec3(0.01f + (rand() % 100) / 100.f * (0.01f - 0.015f));
    }
}

void Scene::Setup()
{
    srand(time(NULL));

    InitSpawnPoints();

    floor.position = glm::vec3(0.f, -0.5f, 0.f);
    floor.scale = glm::vec3(3.5f, 1.f, 3.5f);

    LoadModels();
    SpawnObjects();
    RandomizeObjects();
}

void Scene::Update(float dt)
{
    DayNightCycle(dt);
    Render();
    flashlight.Update(camera.GetCamPos(), camera.GetCamFront(), dt);
    camera.Update(dt);
}

void Scene::DayNightCycle(float dt)
{
    sunAngle += (360.f / CYCLE_DURATION) * dt;
    if (sunAngle >= 360.f) sunAngle -= 360.f;

    float rad = glm::radians(sunAngle);

    glm::vec3 sunPos = glm::vec3(cos(rad) * 10.f, sin(rad) * 10.f, 0.f);
    glm::vec3 sceneCenter = glm::vec3(0.f, 0.f, 0.f);

    // Calcular direccion del sol a partir de su vista
    glm::mat4 sunView = glm::lookAt(sunPos, sceneCenter, glm::vec3(0.f, 0.f, 1.f));
    sunDirection = glm::vec3(sunView * glm::vec4(0.f, 0.f, -1.f, 0.f));

    sun.position = sunPos;
    moon.position = -sunPos;// La luna esta siempre en el lado opuesto al sol

    dayNightTime += dt / CYCLE_DURATION;
    if (dayNightTime >= 1.f) 
        dayNightTime -= 1.f;

}

void Scene::RenderFloor(glm::mat4 projection, glm::mat4 view)
{
    RenderManager* RM = RenderManager::GetInstance();
    RM->SetColor(glm::vec4(0.92f, 0.75f, 0.45f, 1.f));
    glm::mat4 floorModel = GenerateTranslationMatrix(floor.position) * GenerateScaleMatrix(floor.scale);
    RM->DrawFloor(floorModel);
}

void Scene::RenderObjects(glm::mat4 projection, glm::mat4 view)
{
    RenderManager* RM = RenderManager::GetInstance();

    for (RenderObject& obj : trolls)
    {
        RM->SetColor(obj.color);
        glm::mat4 matrix = GenerateTranslationMatrix(obj.position)
            * GenerateRotationMatrix(glm::vec3(1.f, 0.f, 0.f), obj.rotation.x)
            * GenerateRotationMatrix(glm::vec3(0.f, 1.f, 0.f), obj.rotation.y)
            * GenerateRotationMatrix(glm::vec3(0.f, 0.f, 1.f), obj.rotation.z)
            * GenerateScaleMatrix(obj.scale);
        RM->DrawModel(*obj.model, matrix, projection, view);
    }

    for (RenderObject& obj : cats)
    {
        RM->SetColor(obj.color);
        glm::mat4 matrix = GenerateTranslationMatrix(obj.position)
            * GenerateRotationMatrix(glm::vec3(1.f, 0.f, 0.f), obj.rotation.x)
            * GenerateRotationMatrix(glm::vec3(0.f, 1.f, 0.f), obj.rotation.y)
            * GenerateRotationMatrix(glm::vec3(0.f, 0.f, 1.f), obj.rotation.z)
            * GenerateScaleMatrix(obj.scale);
        RM->DrawModel(*obj.model, matrix, projection, view);
    }

    for (RenderObject& obj : woodenTowers)
    {
        RM->SetColor(obj.color);
        glm::mat4 matrix = GenerateTranslationMatrix(obj.position)
            * GenerateRotationMatrix(glm::vec3(1.f, 0.f, 0.f), obj.rotation.x)
            * GenerateRotationMatrix(glm::vec3(0.f, 1.f, 0.f), obj.rotation.y)
            * GenerateRotationMatrix(glm::vec3(0.f, 0.f, 1.f), obj.rotation.z)
            * GenerateScaleMatrix(obj.scale);
        RM->DrawModel(*obj.model, matrix, projection, view);
    }

    for (RenderObject& obj : skulls)
    {
        RM->SetColor(obj.color);
        glm::mat4 matrix = GenerateTranslationMatrix(obj.position)
            * GenerateRotationMatrix(glm::vec3(1.f, 0.f, 0.f), obj.rotation.x)
            * GenerateRotationMatrix(glm::vec3(0.f, 1.f, 0.f), obj.rotation.y)
            * GenerateRotationMatrix(glm::vec3(0.f, 0.f, 1.f), obj.rotation.z)
            * GenerateScaleMatrix(obj.scale);
        RM->DrawModel(*obj.model, matrix, projection, view);
    }
}

void Scene::RenderSkyObjects(glm::mat4 projection, glm::mat4 view)
{
    RenderManager* RM = RenderManager::GetInstance();

    RM->SetColor(sun.color);
    glm::mat4 matrix = GenerateTranslationMatrix(sun.position) * GenerateScaleMatrix(sun.scale);
    RM->DrawModel(*sun.model, matrix, projection, view);

    RM->SetColor(moon.color);
    glm::mat4 moonMatrix = GenerateTranslationMatrix(moon.position) * GenerateScaleMatrix(moon.scale);
    RM->DrawModel(*moon.model, moonMatrix, projection, view);
}

void Scene::Render()
{
    RenderManager* RM = RenderManager::GetInstance();
    GLuint prog = RM->GetProgram();

    flashlight.SendToShader(prog);

    glUniform3f(glGetUniformLocation(prog, "sunDirection"), sun.position.x, sun.position.y, sun.position.z);
    glUniform1f(glGetUniformLocation(prog, "dayNightTime"), dayNightTime);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, dayNightTextureID);
    glUniform1i(glGetUniformLocation(prog, "dayNightSampler"), 1);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(prog, "textureSampler"), 0);

    glm::mat4 projection = camera.GetProjectionMatrix();
    glm::mat4 view = camera.GetViewMatrix();

    RenderFloor(projection, view);
    RenderObjects(projection, view);
    RenderSkyObjects(projection, view);

    glEnable(GL_CULL_FACE);
}