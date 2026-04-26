#pragma once

class TimeManager
{
public:
    static TimeManager* GetInstance();

    void InitDt();
    void UpdateDt();
    float GetDt() const;

private:
    TimeManager() = default;

    static TimeManager* instance;

    float fCurrentTime = 0.f;
    float fLastTime = 0.f;
    float fDeltaTime = 0.f;
};
