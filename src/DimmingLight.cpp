#include "DimmingLight.h"
#include <Arduino.h>

// Konstruktor
DimmingLight::DimmingLight(uint8_t pin)
{
    // Initialisierung von Variablen hier, wenn nötig
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
}

void DimmingLight::begin()
{
    pinMode(_pin, OUTPUT); // Setze den Pin als Ausgang für die LED
}

void DimmingLight::SetLoadLevelTarget(int newLoadLevelTarget)
{
    if (newLoadLevelTarget >= minLevel && newLoadLevelTarget <= maxLevel)
    {
        loadLevelTarget = newLoadLevelTarget;
        int mappingLoadLevelTarget = map(newLoadLevelTarget, minLevel, maxLevel, 0, 255); // Mappen des Bereichs von 0-100 auf 0-255 für PWM
        analogWrite(_pin, mappingLoadLevelTarget); // Setze die Helligkeit der LED entsprechend dem Zielhelligkeitsniveau
        Serial.println("Load level target set to: " + String(loadLevelTarget));
    }
    else
    {
        Serial.println("Invalid load level target.");
    }
}

void DimmingLight::TurnOn()
{
    SetOnEffect(onEffect);
    Serial.println("LED turned on.");
}

void DimmingLight::TurnOff()
{
    isRamping = false;
    analogWrite(_pin, 0); // Setze die Helligkeit auf 0, um die LED auszuschalten
    Serial.println("LED turned off.");
}

void DimmingLight::StartRampUp()
{
    if (isRamping)
    {
        Serial.println("Ramp is already in progress.");
        return;
    }

    isRamping = true;
    Serial.println("Starting ramp up with a time of " + String(rampTime) + " milliseconds.");

    int currentLevel = loadLevelTarget;
    int targetLevel = maxLevel; // Zielhelligkeitsniveau

    while (currentLevel < targetLevel)
    {
        currentLevel = min(currentLevel + stepDelta, targetLevel);
        SetLoadLevelTarget(currentLevel);
        Serial.println("Current load level: " + String(loadLevelTarget));
        delay(rampTime / (targetLevel / stepDelta)); // Anpassung der Schrittgeschwindigkeit entsprechend des Zeitrahmens
    }

    isRamping = false;
    Serial.println("Ramp up complete.");
}

void DimmingLight::StartRampDown()
{
    if (isRamping)
    {
        Serial.println("Ramp is already in progress.");
        return;
    }

    isRamping = true;
    Serial.println("Starting ramp down with a time of " + String(rampTime) + " milliseconds.");

    int currentLevel = loadLevelTarget;
    int targetLevel = minLevel; // Zielhelligkeitsniveau

    while (currentLevel > targetLevel)
    {
        currentLevel = max(currentLevel - stepDelta, targetLevel);
        SetLoadLevelTarget(currentLevel);
        Serial.println("Current load level: " + String(loadLevelTarget));
        delay(rampTime / (100 / stepDelta)); // Anpassung der Schrittgeschwindigkeit entsprechend des Zeitrahmens
    }

    isRamping = false;
    Serial.println("Ramp down complete.");
}

void DimmingLight::StepUp()
{
    if (loadLevelTarget + stepDelta <= maxLevel)
    {
        SetLoadLevelTarget(loadLevelTarget + stepDelta);
        Serial.println("Increased load level to: " + String(loadLevelTarget));
    }
    else
    {
        Serial.println("Maximum load level reached.");
    }
}

void DimmingLight::StepDown()
{
    if (loadLevelTarget - stepDelta >= minLevel)
    {
        SetLoadLevelTarget(loadLevelTarget - stepDelta);
        Serial.println("Decreased load level to: " + String(loadLevelTarget));
    }
    else
    {
        Serial.println("Minimum load level reached.");
    }
}

int DimmingLight::GetLoadLevelTarget()
{
    Serial.println("Current load level target: " + String(loadLevelTarget));
    return loadLevelTarget;
}

void DimmingLight::SetOnEffectLevel(int newOnEffectLevel)
{
    if (newOnEffectLevel >= minLevel && newOnEffectLevel <= maxLevel)
    {
        onEffectLevel = newOnEffectLevel;
        Serial.println("On effect level set to: " + String(onEffectLevel));
    }
    else
    {
        Serial.println("Invalid on effect level.");
    }
}

int DimmingLight::GetMinLevel()
{
    Serial.println("Minimum level: " + String(minLevel));
    return minLevel;
}

void DimmingLight::SetOnEffect(String newOnEffect)
{
    onEffect = newOnEffect;
    PauseRamp();

    if (newOnEffect == "Pulse")
    {
        PulseEffect(rampTime);
        Serial.println("Pulse effect activated.");
    }
    else if (newOnEffect == "onEffectLevel")
    {
        // Setze LoadLevelTarget auf den aktuellen Wert von onEffectLevel
        SetLoadLevelTarget(onEffectLevel);
        Serial.println("Setting LoadLevelTarget to current value: " + String(onEffectLevel));
    }
    else if (newOnEffect == "LastSetting")
    {
        // Setze onEffectLevel auf den Wert der letzten Konfiguration
        onEffect = lastConfiguredonEffect;
        Serial.println("Setting onEffect to last configured value: " + lastConfiguredonEffect);

        onEffectLevel = lastConfiguredOnEffectLevel;
        SetLoadLevelTarget(onEffectLevel);
        Serial.println("Setting onEffectLevel to last configured value: " + String(lastConfiguredOnEffectLevel));
    }
    else
    {
        SetLoadLevelTarget(100);
    }

    lastConfiguredOnEffectLevel = onEffectLevel;
    lastConfiguredonEffect = onEffect;

    Serial.println("On effect set to: " + onEffect);
}

void DimmingLight::GetOnEffectParameters(String &retOnEffect, int &retOnEffectLevel)
{
    retOnEffect = onEffect;
    retOnEffectLevel = onEffectLevel;
    Serial.println("On effect: " + retOnEffect + ", On effect level: " + String(retOnEffectLevel));
}

void DimmingLight::SetStepDelta(int newStepDelta)
{
    if (newStepDelta > minLevel && newStepDelta <= maxLevel)
    {
        stepDelta = newStepDelta;
        Serial.println("Step delta set to: " + String(stepDelta));
    }
    else
    {
        Serial.println("Invalid step delta.");
    }
}

int DimmingLight::GetStepDelta()
{
    Serial.println("Current step delta: " + String(stepDelta));
    return stepDelta;
}

void DimmingLight::SetRampTime(int newRampTime)
{
    rampTime = newRampTime;
    Serial.println("Ramp time set to: " + String(rampTime) + " milliseconds");
}

int DimmingLight::GetRampTime()
{
    Serial.println("Current ramp time: " + String(rampTime) + " milliseconds");
    return rampTime;
}

bool DimmingLight::GetIsRamping()
{
    Serial.println("Is ramping: " + String(isRamping));
    return isRamping;
}

void DimmingLight::PauseRamp()
{
    if (isRamping)
    {
        rampPaused = true;
        Serial.println("Ramp paused.");
    }
    else
    {
        Serial.println("No ramp in progress to pause.");
    }
}

void DimmingLight::ResumeRamp()
{
    if (rampPaused)
    {
        rampPaused = false;
        Serial.println("Ramp resumed.");
    }
    else
    {
        Serial.println("No ramp paused to resume.");
    }
}

void DimmingLight::PulseEffect(int pulseInterval)
{
    while (!rampPaused)
    {
        StartRampUp();
        StartRampDown();
        delay(pulseInterval);
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

    Serial.println("Calculated step delta: " + String(stepDelta));
}

LoadLevelStatus DimmingLight::GetLoadLevelStatus()
{
    if (loadLevelTarget == minLevel)
    {
        return Minimum;
    }
    else if (loadLevelTarget == maxLevel)
    {
        return Maximum;
    }
    else
    {
        return BetweenMinimumAndMaximum;
    }
}