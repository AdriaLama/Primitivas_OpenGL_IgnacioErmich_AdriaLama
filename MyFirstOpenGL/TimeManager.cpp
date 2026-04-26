#include "TimeManager.h"
#include <GLFW/glfw3.h>

TimeManager* TimeManager::instance = nullptr;

TimeManager* TimeManager::GetInstance()
{
    if (instance == nullptr)
        instance = new TimeManager();
    return instance;
}

void TimeManager::InitDt()
{
    fLastTime = static_cast<float>(glfwGetTime());
    fCurrentTime = fLastTime;
    fDeltaTime = 0.f;
}

void TimeManager::UpdateDt()
{
    fCurrentTime = static_cast<float>(glfwGetTime());
    fDeltaTime = fCurrentTime - fLastTime;
    fLastTime = fCurrentTime;
}

float TimeManager::GetDt() const
{
    return fDeltaTime;
}