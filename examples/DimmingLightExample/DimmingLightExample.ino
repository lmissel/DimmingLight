/*
  -----------------------
  DimmingLight-Library
  -----------------------
  This Arduino script demonstrates how to control the brightness of a dimmable lamp using the DimmingLight library. 
  It uses an Arduino microcontroller and an LED string connected to pin 9 (PWM). The script switches the LED string on, gradually changes its brightness and then switches it off again. 
  and then switches it off again. The brightness is ramped up and down by a ramp control and can also be set directly to certain levels. The script 
  thus offers a simple way to control a dimmable lamp (e.g. LED, LED string) and is customisable for different brightness levels and ramp times. 

  Here is a brief explanation of the script: 
  - First, the DimmingLight library is integrated.
  - Then an instance of the DimmingLight class is created, which is connected to pin 9 (PWM) of the Arduino board. 
  
  Some settings are made in the setup() function: 
  - Function to handle the hardware configuration.

  The sequence is controlled in the loop() function:
  - The lamp is switched on and activated for 5 seconds.
  - Then the brightness of the lamp is gradually reduced (downwards) and held for a further 5 seconds.
  - The brightness is then gradually increased (upwards) and held again for 5 seconds.
  - The brightness is changed to 75%, 50% and 25%, each activated for 5 seconds.
  - Finally, the lamp is switched off and held off for a further 5 seconds.
  - This sequence is repeated in an endless loop (loop()).

  Created by lmissel
*/

#include <DimmingLight.h>

int led = 9;
DimmingLight dimmingLight(led);

void setup() {
  Serial.begin(9600);                         // Configuring the serial interface
  dimmingLight.begin();                       // The hardware configuration takes place in a dedicated function instead of the constructor.
}

void loop() {
  dimmingLight.TurnOn();                      // Switches an LED on.
  delay(5000);                                // Wait for 5 seconds
  dimmingLight.StartRampDown();               // Starts a ramp LED down.
  delay(5000);                                // Wait for 5 seconds
  dimmingLight.StartRampUp();                 // Starts a ramp LED up.
  delay(5000);                                // Wait for 5 seconds
  dimmingLight.SetOnEffectLevel(75);          // Determines the brightness of the LED. 
  dimmingLight.SetOnEffect("onEffectLevel");  // Specifies that the LED is switched on with the specified brightness.
  delay(5000);                                // Wait for 5 seconds
  dimmingLight.SetOnEffectLevel(50);          // Determines the brightness of the LED. 
  dimmingLight.SetOnEffect("onEffectLevel");  // Specifies that the LED is switched on with the specified brightness.
  delay(5000);                                // Wait for 5 seconds
  dimmingLight.SetOnEffectLevel(25);          // Determines the brightness of the LED. 
  dimmingLight.SetOnEffect("onEffectLevel");  // Specifies that the LED is switched on with the specified brightness.
  delay(5000);                                // Wait for 5 seconds
  dimmingLight.TurnOff();                     // Switches an LED off.
  delay(5000);                                // Wait for 5 seconds
}