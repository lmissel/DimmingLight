#include <DimmingLight.h>
#include <Arduino.h>

// Konstruktor
DimmingLight::DimmingLight(uint8_t pin)
{
    // Initialisierung von Variablen hier, wenn nötig
    loadLevel = 0;
    loadLevelTarget = 0;
    isRamping = false;
    rampTime = 1000; // Standard-Rampenzeit (in Millisekunden)
    stepDelta = 1;   // Standard-Schrittweite
    minLevel = 0;
    maxLevel = 100;
    onEffectLevel = maxLevel; // Standardwert für On-Effekt-Level
    onEffect = "Default"; // Standardwert für On-Effekt
    rampPaused = false;
    _pin = pin;
    isPulsing = false;
    isOn = false;
}

void DimmingLight::begin()
{
    pinMode(_pin, OUTPUT); // Setze den Pin als Ausgang für die LED
}

void DimmingLight::process()
{
    if(isPulsing)
    {
        StartRampUp();
        StartRampDown();        
    }
    else
    {
        execute();
    }
}

void DimmingLight::execute()
{
    if (loadLevel != loadLevelTarget)
    {
        int mappingLoadLevelTarget = map(loadLevelTarget, minLevel, maxLevel, 0, 255); // Mappen des Bereichs von 0-100 auf 0-255 für PWM
        analogWrite(_pin, mappingLoadLevelTarget); // Setze die Helligkeit der LED entsprechend dem Zielhelligkeitsniveau
        loadLevel = loadLevelTarget;

        if (loadLevel > 0)
        {
            isOn = true;
        }
        else
        {
            isOn = false;
        }
    }
}

void DimmingLight::SetDebugMode(bool debug)
{
    debugMode = debug;
}

void DimmingLight::DebugPrint(const String &message)
{
    if (debugMode) {
       Serial.println(message);
    }
}

void DimmingLight::SetLoadLevelTarget(int newLoadLevelTarget)
{
    if (newLoadLevelTarget >= minLevel && newLoadLevelTarget <= maxLevel)
    {
        loadLevelTarget = newLoadLevelTarget;
        DebugPrint("Load level target set to: " + String(loadLevelTarget));
    }
    else
    {
       DebugPrint("Invalid load level target.");
    }
}

void DimmingLight::TurnOn()
{
    SetOnEffect(onEffect);
    DebugPrint("LED turned on.");
}

void DimmingLight::TurnOff()
{
    isRamping = false;
    // SetOnEffect("Default");
    SetLoadLevelTarget(0); // Setze die Helligkeit auf 0, um die LED auszuschalten
    DebugPrint("LED turned off.");
}

void DimmingLight::StartRampUp()
{
    if (isRamping)
    {
        DebugPrint("Ramp is already in progress.");
        return;
    }

    isRamping = true;
    DebugPrint("Starting ramp up with a time of " + String(rampTime) + " milliseconds.");

    int currentLevel = loadLevelTarget;
    int targetLevel = maxLevel; // Zielhelligkeitsniveau

    while (currentLevel < targetLevel)
    {
        currentLevel = min(currentLevel + stepDelta, targetLevel);
        SetLoadLevelTarget(currentLevel);
        execute();
        DebugPrint("Current load level: " + String(loadLevelTarget));
        delay(rampTime / (targetLevel / stepDelta)); // Anpassung der Schrittgeschwindigkeit entsprechend des Zeitrahmens
    }

    isRamping = false;
    DebugPrint("Ramp up complete.");
}

void DimmingLight::StartRampDown()
{
    if (isRamping)
    {
        DebugPrint("Ramp is already in progress.");
        return;
    }

    isRamping = true;
    DebugPrint("Starting ramp down with a time of " + String(rampTime) + " milliseconds.");

    int currentLevel = loadLevelTarget;
    int targetLevel = minLevel; // Zielhelligkeitsniveau

    while (currentLevel > targetLevel)
    {
        currentLevel = max(currentLevel - stepDelta, targetLevel);
        SetLoadLevelTarget(currentLevel);
        execute();
        DebugPrint("Current load level: " + String(loadLevelTarget));
        delay(rampTime / (100 / stepDelta)); // Anpassung der Schrittgeschwindigkeit entsprechend des Zeitrahmens
    }

    isRamping = false;
    DebugPrint("Ramp down complete.");
}

void DimmingLight::StepUp()
{
    if (loadLevelTarget + stepDelta <= maxLevel)
    {
        SetLoadLevelTarget(loadLevelTarget + stepDelta);
        DebugPrint("Increased load level to: " + String(loadLevelTarget));
    }
    else
    {
        DebugPrint("Maximum load level reached.");
    }
}

void DimmingLight::StepDown()
{
    if (loadLevelTarget - stepDelta >= minLevel)
    {
        SetLoadLevelTarget(loadLevelTarget - stepDelta);
        DebugPrint("Decreased load level to: " + String(loadLevelTarget));
    }
    else
    {
        DebugPrint("Minimum load level reached.");
    }
}

int DimmingLight::GetLoadLevelTarget()
{
    DebugPrint("Current load level target: " + String(loadLevelTarget));
    return loadLevelTarget;
}

void DimmingLight::SetOnEffectLevel(int newOnEffectLevel)
{
    if (newOnEffectLevel >= minLevel && newOnEffectLevel <= maxLevel)
    {
        onEffectLevel = newOnEffectLevel;
        DebugPrint("On effect level set to: " + String(onEffectLevel));
    }
    else
    {
        DebugPrint("Invalid on effect level.");
    }
}

int DimmingLight::GetMinLevel()
{
    DebugPrint("Minimum level: " + String(minLevel));
    return minLevel;
}

void DimmingLight::SetOnEffect(String newOnEffect)
{
    onEffect = newOnEffect;
    PauseRamp();

    if (newOnEffect == "Pulse")
    {
        isPulsing = true;
        DebugPrint("Pulse effect activated.");
    }
    else if (newOnEffect == "onEffectLevel")
    {
        isPulsing = false;

        // Setze LoadLevelTarget auf den aktuellen Wert von onEffectLevel
        SetLoadLevelTarget(onEffectLevel);
        DebugPrint("Setting LoadLevelTarget to current value: " + String(onEffectLevel));
    }
    else if (newOnEffect == "LastSetting")
    {
        isPulsing = false;

        // Setze onEffectLevel auf den Wert der letzten Konfiguration
        onEffect = lastConfiguredonEffect;
        DebugPrint("Setting onEffect to last configured value: " + lastConfiguredonEffect);

        onEffectLevel = lastConfiguredOnEffectLevel;
        SetLoadLevelTarget(onEffectLevel);
        DebugPrint("Setting onEffectLevel to last configured value: " + String(lastConfiguredOnEffectLevel));
    }
    else
    {
        isPulsing = false;

        SetLoadLevelTarget(100);
    }

    lastConfiguredOnEffectLevel = onEffectLevel;
    lastConfiguredonEffect = onEffect;

    DebugPrint("On effect set to: " + onEffect);
}

void DimmingLight::GetOnEffectParameters(String &retOnEffect, int &retOnEffectLevel)
{
    retOnEffect = onEffect;
    retOnEffectLevel = onEffectLevel;
    DebugPrint("On effect: " + retOnEffect + ", On effect level: " + String(retOnEffectLevel));
}

void DimmingLight::SetStepDelta(int newStepDelta)
{
    if (newStepDelta > minLevel && newStepDelta <= maxLevel)
    {
        stepDelta = newStepDelta;
        DebugPrint("Step delta set to: " + String(stepDelta));
    }
    else
    {
        DebugPrint("Invalid step delta.");
    }
}

int DimmingLight::GetStepDelta()
{
    DebugPrint("Current step delta: " + String(stepDelta));
    return stepDelta;
}

void DimmingLight::SetRampTime(int newRampTime)
{
    rampTime = newRampTime;
    DebugPrint("Ramp time set to: " + String(rampTime) + " milliseconds");
}

int DimmingLight::GetRampTime()
{
    DebugPrint("Current ramp time: " + String(rampTime) + " milliseconds");
    return rampTime;
}

bool DimmingLight::GetIsRamping()
{
    DebugPrint("Is ramping: " + String(isRamping));
    return isRamping;
}

void DimmingLight::PauseRamp()
{
    if (isRamping)
    {
        rampPaused = true;
        DebugPrint("Ramp paused.");
    }
    else
    {
        DebugPrint("No ramp in progress to pause.");
    }
}

void DimmingLight::ResumeRamp()
{
    if (rampPaused)
    {
        rampPaused = false;
        DebugPrint("Ramp resumed.");
    }
    else
    {
        DebugPrint("No ramp paused to resume.");
    }
}

void DimmingLight::CalculateStepDelta(int targetLoadLevel)
{
    // Definiere die Kriterien für die Berechnung des StepDelta
    const int minStepDelta = 1;
    const int maxStepDelta = 10;
    const int thresholdLow = minLevel;
    const int thresholdHigh = maxLevel;

    // Berechne den Abstand zwischen dem aktuellen LoadLevelTarget und dem Zielwert
    int delta = abs(targetLoadLevel - loadLevelTarget);

    // Basierend auf dem Abstand und den Schwellenwerten berechne das StepDelta
    if (delta <= thresholdLow)
    {
        stepDelta = maxStepDelta;
    }
    else if (delta >= thresholdHigh)
    {
        stepDelta = minStepDelta;
    }
    else
    {
        // Verwende eine lineare Interpolation zwischen den Schwellenwerten
        double ratio = double(delta - thresholdLow) / (thresholdHigh - thresholdLow);
        stepDelta = round((1 - ratio) * maxStepDelta + ratio * minStepDelta);
    }

    DebugPrint("Calculated step delta: " + String(stepDelta));
}

int DimmingLight::GetLoadLevelStatus()
{
    return loadLevel;
}

bool DimmingLight::GetStatus()
{
    return isOn;
}