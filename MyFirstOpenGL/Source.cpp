#include "RenderManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Scene.h"

void main()
{
    RenderManager* RM = RenderManager::GetInstance();
    TimeManager* TM = TimeManager::GetInstance();
    InputManager* IM = InputManager::GetInstance();

    RM->Init();
    TM->InitDt();

    // Creamos la escena y la inicializamos
    Scene scene;
    scene.Setup();

    // Game loop
    while (!RM->WindowShouldClose())
    {
        // Actualizamos delta time
        TM->UpdateDt();

        // Pulleamos los eventos (botones, teclas, mouse...)
        RM->PollEvents();

        // El InputManager se actualiza aquí, una vez por frame
        IM->Update(RM->GetWindow());

        // Limpiamos los buffers
        RM->ClearBuffers();

        // Actualizamos y renderizamos la escena
        scene.Update(TM->GetDt());

        // Cambiamos buffers
        RM->SwapBuffers();
    }

    RM->Shutdown();
}