#include <DimmingLight.h>
#include <EEPROM.h>

int led = 9;
DimmingLight dimmingLight(led);
int eepromAddress = 0; // Adresse im EEPROM, an der die Helligkeitsstufe gespeichert wird
int storedBrightness;

void setup() {
  Serial.begin(9600);
  dimmingLight.begin();
  storedBrightness = EEPROM.read(eepromAddress); // Lese die gespeicherte Helligkeitsstufe aus dem EEPROM
  dimmingLight.SetOnEffectLevel(storedBrightness); // Setze die Helligkeitsstufe auf den gespeicherten Wert
  dimmingLight.SetOnEffect("onEffectLevel");
  dimmingLight.SetDebugMode(true);
}

void loop() {
  dimmingLight.process();
  dimmingLight.GetLoadLevelTarget();
  delay(5000); // Hier können andere Aufgaben ausgeführt werden oder der Arduino in den Schlafmodus versetzt werden
}
