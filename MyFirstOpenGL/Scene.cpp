#include "Scene.h"
#include "RenderManager.h"
#include "Transform.h"
#include <cstdlib> 
#include <ctime>   

void Scene::Setup()
{
    srand(time(NULL));
    
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
    sunAngle += (360.f / CYCLE_DURATION) * dt;
    if (sunAngle >= 360.f) sunAngle -= 360.f;

    float rad = glm::radians(sunAngle);

    glm::vec3 sunPos = glm::vec3(cos(rad) * 10.f, sin(rad) * 10.f, 0.f);
    glm::vec3 sceneCenter = glm::vec3(0.f, 0.f, 0.f);

    glm::mat4 sunView = glm::lookAt(sunPos, sceneCenter, glm::vec3(0.f, 0.f, 1.f));
    sunDirection = glm::vec3(sunView * glm::vec4(0.f, 0.f, -1.f, 0.f));

    float t = (sin(rad) + 1.f) * 0.5f;
    glm::vec3 nightColor = glm::vec3(0.0f, 0.0f, 0.08f);
    glm::vec3 dayColor = glm::vec3(0.2f, 0.18f, 0.08f);
    // mix: Sirve para interpolar entre el nightColor y el dayColor
    ambientColor = glm::mix(nightColor, dayColor, t);
    sunIntensity = t * 1.2f;

    Render();
    flashlight.Update(camera.GetCamPos(), camera.GetCamFront(), dt);
    camera.Update(dt);
}

// Recorre todos los objetos de la escena y los envía a renderizar.
// Para cada objeto construye su matriz de modelo : Translation * RotationX * RotationY * RotationZ * Scale
// Se obtienen las matrices de proyección y vista de la cámara y se pasan al RenderManager junto con la geometría del modelo.

void Scene::Render()
{
    RenderManager* RM = RenderManager::GetInstance();
    GLuint prog = RM->GetProgram();

    flashlight.SendToShader(prog);

    // Sol y ambiente
    glUniform3f(glGetUniformLocation(prog, "ambientColor"),
        ambientColor.r, ambientColor.g, ambientColor.b);
    glUniform3f(glGetUniformLocation(prog, "sunDirection"),
        sunDirection.x, sunDirection.y, sunDirection.z);
    glUniform1f(glGetUniformLocation(prog, "sunIntensity"), sunIntensity);

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