#define DimmingLight_h

#include <Arduino.h>

enum LoadLevelStatus
{
    Minimum,
    Maximum,
    BetweenMinimumAndMaximum
};

class DimmingLight
{
private:
    int _pin;
    int loadLevelTarget = 0;
    int onEffectLevel = 100;
    int lastConfiguredOnEffectLevel = 100;
    int minLevel = 0;
    int maxLevel = 100;
    String onEffect = "Default";
    String lastConfiguredonEffect = "Default";
    int stepDelta = 1; // Default step delta value
    int rampTime = 0; // Default ramp time value

    bool isRamping = false;
    bool rampPaused = false;

public:
    DimmingLight(uint8_t pin);
    void begin();
    void SetLoadLevelTarget(int newLoadLevelTarget);
    void TurnOn();
    void TurnOff();
    void StartRampUp();
    void StartRampDown();
    void StepUp();
    void StepDown();
    int GetLoadLevelTarget();
    void SetOnEffectLevel(int newOnEffectLevel);
    int GetMinLevel();
    void SetOnEffect(String newOnEffect);
    void GetOnEffectParameters(String &retOnEffect, int &retOnEffectLevel);
    void SetStepDelta(int newStepDelta);
    int GetStepDelta();
    void SetRampTime(int newRampTime);
    int GetRampTime();
    bool GetIsRamping();
    void PauseRamp();
    void ResumeRamp();
    void PulseEffect(int pulseInterval);
    void CalculateStepDelta(int targetLoadLevel);
    LoadLevelStatus GetLoadLevelStatus();
};