# DimmingLight
DimmingLight is a small library that makes it very easy to control a dimmable lamp.

## Description
DimmingLight is a small library that allows you to adjust the brightness of a dimmable lamp with minimal code. It works with any lamp that supports PWM (pulse-width modulation) signals, such as LED or halogen lamps. You can use DimmingLight to create different lighting effects, such as fading, blinking, or pulsing. DimmingLight is easy to install and use, and it has a simple and intuitive API. You can control the lamp with just a few lines of code, and you can customize the parameters to suit your needs.

## Installation
The Arduino environment can be extended through the use of libraries, just like most programming platforms. Libraries provide extra functionality for use in sketches, e.g. working with hardware or manipulating data. To use a library in a sketch, select it from ***Sketch > Import Library***.

A number of libraries come installed with the IDE, but you can also download or create your own. See [these instructions](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries/) for details on installing libraries. There is also a [tutorial on writing your own libraries](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries/). See the [API Style Guide](https://docs.arduino.cc/learn/contributions/arduino-library-style-guide/) for information on making a good Arduino-style API for your library.

## Usage (DimmingLightExample)
This Arduino script demonstrates how to control the brightness of a dimmable lamp using the DimmingLight library. It uses an Arduino microcontroller and an LED connected to pin 9 (PWM). The script switches the LED on, gradually changes its brightness and then switches it off again. The brightness is adjusted up and down via a ramp control and can also be set directly to specific values. The script therefore offers a simple way of controlling a dimmable lamp (e.g. LED, LED string) and can be customised for different brightness levels and ramp times. 

### List of components
|Name|Quantity|Component|
|---|---|---|
|U1|1|Arduino Uno R3|
|R1|1|100 Ohm resistor|
|D1|1|Blue LED|

### Circuit view
Please connect the blue LED and the 100 Ohm resistor to the Arduino Uno R3 as follows:
![Circuit view](/media/Layout.png "DimmingLightExample")

### Code
Here is the 'examples\DimmingLightExample\' example code:

```ino
#include <DimmingLight.h>

int led = 9;
DimmingLight dimmingLight(led);

void setup() {
  Serial.begin(9600);                         // Configuring the serial interface
  dimmingLight.begin();                       // The hardware configuration takes place in a dedicated function instead of the constructor.
}

void loop() {
  dimmingLight.TurnOn();                      // Switches an LED on.
  dimmingLight.process();
  delay(5000);                                // Wait for 5 seconds
  dimmingLight.StartRampDown();               // Starts an LED ramp down. This function is currently still blocking the thread.
  delay(5000);                                // Wait for 5 seconds
  dimmingLight.StartRampUp();                 // Starts an LED ramp down. This function is currently still blocking the thread.
  delay(5000);                                // Wait for 5 seconds
  dimmingLight.SetOnEffectLevel(75);          // Determines the brightness of the LED. 
  dimmingLight.SetOnEffect("onEffectLevel");  // Specifies that the LED is switched on with the specified brightness.
  dimmingLight.process();
  delay(5000);                                // Wait for 5 seconds
  dimmingLight.SetOnEffectLevel(50);          // Determines the brightness of the LED. 
  dimmingLight.SetOnEffect("onEffectLevel");  // Specifies that the LED is switched on with the specified brightness.
  dimmingLight.process();
  delay(5000);                                // Wait for 5 seconds
  dimmingLight.SetOnEffectLevel(25);          // Determines the brightness of the LED. 
  dimmingLight.SetOnEffect("onEffectLevel");  // Specifies that the LED is switched on with the specified brightness.
  dimmingLight.process();
  delay(5000);                                // Wait for 5 seconds
  dimmingLight.TurnOff();                     // Switches an LED off.
  dimmingLight.process();
  delay(5000);                                // Wait for 5 seconds
}
```

## Changelog

|Version|State|Comment|
|---|---|---|
|1.0.0.0|done|Initial programming of the library.|

## Notes
The library documentation is still under construction. It will soon be available in German and English.