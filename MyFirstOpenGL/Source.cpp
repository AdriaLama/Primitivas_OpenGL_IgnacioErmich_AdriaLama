#include "RenderManager.h"
#include "TimeManager.h"
#include "Scene.h"

void main()
{
    RenderManager* RM = RenderManager::GetInstance();
    TimeManager* TM = TimeManager::GetInstance();

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

        // Limpiamos los buffers
        RM->ClearBuffers();

        // Actualizamos y renderizamos la escena
        scene.Update(TM->GetDt());

        // Cambiamos buffers
        RM->SwapBuffers();
    }

    RM->Shutdown();
}