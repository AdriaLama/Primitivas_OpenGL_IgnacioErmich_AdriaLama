#include "Scene.h"
#include "RenderManager.h"
#include "Transform.h"
#include <cstdlib> 
#include <ctime>   

void Scene::Setup()
{
    srand(time(NULL));
    
    randomRotation = rand() & 361;

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

    // Suelo
    floor.position = glm::vec3(0.f, -0.5f, 0.f);
    floor.scale = glm::vec3(3.5f, 1.f, 3.5f);

    // Carga los modelos OBJ una sola vez y reutiliza en múltiples instancias
    models.push_back(LoadOBJModel("Assets/Models/troll.obj"));
    models.push_back(LoadOBJModel("Assets/Models/cat.obj"));
    models.push_back(LoadOBJModel("Assets/Models/woodenTower.obj"));
    models.push_back(LoadOBJModel("Assets/Models/skull.obj"));

    // Cargar las texturas correspondientes a cada modelo
    models[0].textureID = RenderManager::GetInstance()->LoadTexture("Assets/Textures/troll.png");
    models[1].textureID = RenderManager::GetInstance()->LoadTexture("Assets/Textures/cat.jpg");
    models[2].textureID = RenderManager::GetInstance()->LoadTexture("Assets/Textures/woodenTower.jpg");
    models[3].textureID = RenderManager::GetInstance()->LoadTexture("Assets/Textures/skull.jpg");

    int idx = 0;

    trolls.push_back({ &models[0], spawnPoints[idx++],  glm::vec3(0.f, 0.f, 0.f),   glm::vec3(0.f), glm::vec4(0.2f, 1.0f, 0.3f, 1.f) });
    trolls.push_back({ &models[0], spawnPoints[idx++], glm::vec3(0.f, 0.f, 0.f),    glm::vec3(0.f), glm::vec4(0.2f, 0.5f, 0.5f, 1.f) });
    trolls.push_back({ &models[0], spawnPoints[idx++],   glm::vec3(0.f, 0.f, 0.f),  glm::vec3(0.f), glm::vec4(0.2f, 1.f, 1.0f, 1.f) });

    cats.push_back({ &models[1], spawnPoints[idx++],  glm::vec3(-90.f, 0.f, 0.f),  glm::vec3(0.f, 0.f, 0.f), glm::vec4(1.f, 1.f, 1.f, 1.f) });

    woodenTowers.push_back({ &models[2], spawnPoints[idx++],  glm::vec3(0.f, 0.f, 0.f),  glm::vec3(0.f, 0.f, 0.f), glm::vec4(1.f, 1.f, 1.f, 1.f) });

    skulls.push_back({ &models[3], spawnPoints[idx++],  glm::vec3(-90.f, 0.f, 0.f),  glm::vec3(0.f, 0.f, 0.f), glm::vec4(1.f, 1.f, 1.f, 1.f) });
    skulls.push_back({ &models[3], spawnPoints[idx++],  glm::vec3(-90.f, 0.f, 0.f),  glm::vec3(0.f, 0.f, 0.f), glm::vec4(1.f, 1.f, 1.f, 1.f) });
    skulls.push_back({ &models[3], spawnPoints[idx++],  glm::vec3(-90.f, 0.f, 0.f),  glm::vec3(0.f, 0.f, 0.f), glm::vec4(1.f, 1.f, 1.f, 1.f) });

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


void Scene::Update(float dt)
{
    Render();
    camera.Update(dt);
}


// Recorre todos los objetos de la escena y los envía a renderizar.
// Para cada objeto construye su matriz de modelo : Translation * RotationX * RotationY * RotationZ * Scale
// Se obtienen las matrices de proyección y vista de la cámara y se pasan al RenderManager junto con la geometría del modelo.

void Scene::Render()
{
    RenderManager* RM = RenderManager::GetInstance();

    // Obtener matrices de cámara actualizadas para este frame
    glm::mat4 projection = camera.GetProjectionMatrix();
    glm::mat4 view = camera.GetViewMatrix();

    // Suelo
    RM->SetColor(glm::vec4(0.92f, 0.75f, 0.45f, 1.f)); 
    glm::mat4 floorModel = GenerateTranslationMatrix(floor.position) * GenerateScaleMatrix(floor.scale);
    RM->DrawFloor(floorModel);

    //Trolls
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