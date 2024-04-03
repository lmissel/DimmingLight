#include <DimmingLight.h>
#include <EEPROM.h>

int led = 9;
DimmingLight dimmingLight(led);
int eepromAddress = 0; // Adresse im EEPROM, an der die Helligkeitsstufe gespeichert wird
int Brightness = 50; // Wert der Helligkeitsstufe

void setup() {
  Serial.begin(9600);
  dimmingLight.begin();

  EEPROM.write(eepromAddress, Brightness);    // Helligkeitsstufe in das EEPROM schreiben
  dimmingLight.SetOnEffectLevel(Brightness);  // Setze die Helligkeitsstufe auf den gespeicherten Wert
  dimmingLight.SetOnEffect("onEffectLevel");
}

void loop() {
  dimmingLight.process();
}