#include "Scene.h"
#include "RenderManager.h"
#include "Transform.h"


void Scene::Setup()
{
    
    floor.position = glm::vec3(0.f, -0.5f, 0.f);
    floor.scale = glm::vec3(3.5f, 1.f, 3.5f);

    models.push_back(LoadOBJModel("Assets/Models/troll.obj"));
    models.push_back(LoadOBJModel("Assets/Models/rock.obj"));
    models[0].textureID = RenderManager::GetInstance()->LoadTexture("Assets/Textures/troll.png");
    models[1].textureID = RenderManager::GetInstance()->LoadTexture("Assets/Textures/rock.png");

    trolls.push_back({ &models[0], glm::vec3(-0.8f, 0.f, 0.f), glm::vec3(0.f, 90.f, 0.f),  glm::vec3(0.3f), glm::vec4(0.2f, 1.0f, 0.3f, 1.f) });
    trolls.push_back({ &models[0], glm::vec3(0.f, 0.f, -0.75f),glm::vec3(0.f, 0.f, 0.f),  glm::vec3(0.3f), glm::vec4(0.2f, 0.5f, 0.5f, 1.f) });
    trolls.push_back({ &models[0], glm::vec3(0.8f, 0.f, 0.f),  glm::vec3(0.f, -90.f, 0.f), glm::vec3(0.3f), glm::vec4(0.2f, 1.f, 1.0f, 1.f) });

    rocks.push_back({ &models[1], glm::vec3(0.2f, 0.f,  0.4f),  glm::vec3(0.f, 45.f, 0.f), glm::vec3(0.4f, 0.2f, 0.2f), glm::vec4(1.f, 1.f, 1.f, 1.f) });
    rocks.push_back({ &models[1], glm::vec3(-0.2f, 0.f,  0.4f),  glm::vec3(0.f, -45.f, 0.f), glm::vec3(0.4f, 0.2f, 0.2f), glm::vec4(1.f, 1.f, 1.f, 1.f) });
    rocks.push_back({ &models[1], glm::vec3(-0.25f, 0.f,  -0.1f), glm::vec3(0.f, 45.f, 0.f), glm::vec3(0.4f, 0.2f, 0.2f), glm::vec4(1.f, 1.f, 1.f, 1.f) });
    rocks.push_back({ &models[1], glm::vec3(0.25f, 0.f,  -0.1f),  glm::vec3(0.f, -45.f, 0.f), glm::vec3(0.4f, 0.2f, 0.2f), glm::vec4(1.f, 1.f, 1.f, 1.f) });

    //Reutilizamos las rocas para hacer las nubes
    rocks.push_back({ &models[1], glm::vec3(0.25f, 1.f,  -1.f),  glm::vec3(-10.f, 0.f, 10.f), glm::vec3(0.6f, 0.2f, 0.2f), glm::vec4(0.7f, 0.7f, 1.f, 1.f) });
    rocks.push_back({ &models[1], glm::vec3(1.f, 1.25f,  1.f),  glm::vec3(15.f, 0.f, 15.f), glm::vec3(0.8f, 0.3f, 0.3f), glm::vec4(0.7f, 0.7f, 1.f, 1.f) });
    rocks.push_back({ &models[1], glm::vec3(-1.75f, 1.25f,  0.8f),  glm::vec3(12.f, 10.f, 15.f), glm::vec3(0.7f, 0.4f, 0.2f), glm::vec4(0.7f, 0.7f, 1.f, 1.f) });
 
}


void Scene::Update(float dt)
{
    Render();
    camera.Update(dt);
}

void Scene::Render()
{
    RenderManager* RM = RenderManager::GetInstance();
    glm::mat4 projection = camera.GetProjectionMatrix();
    glm::mat4 view = camera.GetViewMatrix();

    RM->SetColor(glm::vec4(0.92f, 0.75f, 0.45f, 1.f));
    glm::mat4 floorModel = GenerateTranslationMatrix(floor.position) * GenerateScaleMatrix(floor.scale);
    RM->DrawFloor(floorModel);

    for (RenderObject& obj : trolls)
    {
        RM->SetColor(obj.color);
        glm::mat4 matrix = GenerateTranslationMatrix(obj.position)* GenerateRotationMatrix(glm::vec3(1.f, 0.f, 0.f), obj.rotation.x) * GenerateRotationMatrix(glm::vec3(0.f, 1.f, 0.f), obj.rotation.y)* GenerateRotationMatrix(glm::vec3(0.f, 0.f, 1.f), obj.rotation.z) * GenerateScaleMatrix(obj.scale);
        RM->DrawModel(*obj.model, matrix, projection, view);
    }

    for (RenderObject& obj : rocks)
    {
        RM->SetColor(obj.color);
        glm::mat4 matrix = GenerateTranslationMatrix(obj.position) * GenerateRotationMatrix(glm::vec3(1.f, 0.f, 0.f), obj.rotation.x) * GenerateRotationMatrix(glm::vec3(0.f, 1.f, 0.f), obj.rotation.y)* GenerateRotationMatrix(glm::vec3(0.f, 0.f, 1.f), obj.rotation.z) * GenerateScaleMatrix(obj.scale);
        RM->DrawModel(*obj.model, matrix, projection, view);
    }
}