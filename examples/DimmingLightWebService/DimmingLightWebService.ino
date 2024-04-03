/*
  ESP8266 Dimmable Lamp Control Sketch

  This sketch allows controlling a dimmable lamp using an ESP8266 microcontroller.
  It implements a web server to provide a user interface for controlling the lamp.

  The lamp control functionality is encapsulated in the DimmingLight class,
  which handles interactions with the dimmable lamp hardware.

  The web server provides the following functionalities:
  - Setting the brightness level of the lamp
  - Setting the OnEffectLevel of the lamp
  - Performing step-up and step-down actions on the lamp's brightness
  - Starting ramp-up and ramp-down actions on the lamp's brightness
  - Restarting the ESP8266 system

  Author: lmissel
  Date: 13.03.2024

  Libraries:
  - ESP8266WiFi.h: Library for connecting ESP8266 to a Wi-Fi network.
  - ESP8266WebServer.h: Library for creating a web server on ESP8266.
  - DimmingLight.h: Library for controlling the dimmable lamp.
  - EEPROM.h: Library for accessing EEPROM memory on ESP8266.

  Hardware:
  - ESP8266-based microcontroller
  - Dimmable lamp connected to the microcontroller's GPIO pin

  Wiring:
  - Adjust the pin used for the dimmable lamp according to your hardware configuration.

  Instructions:
  1. Update the Wi-Fi credentials (SSID and password) to match your network.
  2. Adjust the pin assignment for the dimmable lamp if necessary.
  3. Upload the sketch to your ESP8266 microcontroller.
  4. Access the web interface by navigating to the ESP8266's IP address in a web browser.
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DimmingLight.h>
#include <EEPROM.h>

// WiFi Credentials
#ifndef STASSID
#define STASSID "SSID"            // <------------- WiFi SSID
#define STAPSK "PASSWORD"         // <------------- WiFi PASSWORD
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

// Instance of the DimmingLight class
DimmingLight dimmableLight(D1);  // Adjust the pin according to your hardware

unsigned int eepromAddress = 0;
int storedBrightness = 0;

// Instance of the ESP8266WebServer
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed, stopping sketch");
    while (1) { delay(1000); }
  }

  Serial.println("Connected to WiFi");
  Serial.println("IP Address: " + WiFi.localIP().toString());

  // Initialize EEPROM
  EEPROM.begin(sizeof(storedBrightness));
  Serial.println("EEPROM size: " + String(sizeof(storedBrightness)) + " bytes");

  // Initialize dimmable lamp
  dimmableLight.begin();  
  if (storedBrightness == 0)
  {
    Serial.println("Reading onEffectLevel from EEPROM:");
    EEPROM.get(eepromAddress, storedBrightness);
    Serial.println(storedBrightness);
  }
  dimmableLight.SetOnEffectLevel(storedBrightness);
  dimmableLight.SetOnEffect("onEffectLevel");
  dimmableLight.SetDebugMode(true);

  // Set routes for the web server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/setBrightness", HTTP_POST, handleSetBrightness);
  server.on("/setOnEffectLevel", HTTP_POST, handleSetOnEffectLevel);
  server.on("/restart", HTTP_POST, handleRestart);
  server.on("/stepUp", HTTP_POST, handleStepUp);
  server.on("/stepDown", HTTP_POST, handleStepDown);
  server.on("/startRampUp", HTTP_POST, handleStartRampUp);
  server.on("/startRampDown", HTTP_POST, handleStartRampDown);

  // Starting the web server
  server.begin();
}

void loop() {
  // Handle web server
  server.handleClient();

  // Handle dimmable lamp
  dimmableLight.process();
  
  // Implement your other code here if required
}

void handleRoot() {
  
  String retOnEffect;
  int retOnEffectLevel;
  dimmableLight.GetOnEffectParameters(retOnEffect, retOnEffectLevel);

  String html = "<!DOCTYPE html>";
  html += "<html lang='en'>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Dimmable Lamp</title>";
  html += "<style>";
  html += "body {";
  html += "  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;";
  html += "  background-color: #f9f9f9;";
  html += "  margin: 0;";
  html += "  padding: 0;";
  html += "}";
  html += "h1 {";
  html += "  color: #3498db;";
  html += "  text-align: center;";
  html += "}";
  html += "form {";
  html += "  max-width: 400px;";
  html += "  margin: 20px auto;";
  html += "  background-color: #fff;";
  html += "  padding: 20px;";
  html += "  border-radius: 10px;";
  html += "  box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);";
  html += "}";
  html += "label {";
  html += "  font-weight: bold;";
  html += "}";
  html += "p {";
  html += "  font-size: small;";
  html += "  color: #555555;";
  html += "}";
  html += "input[type='range'], input[type='submit'] {";
  html += "  width: calc(100% - 22px);";
  html += "  padding: 10px;";
  html += "  margin: 10px 0;";
  html += "  border: none;";
  html += "  border-radius: 5px;";
  html += "  background-color: #3498db;";
  html += "  color: #fff;";
  html += "  cursor: pointer;";
  html += "  transition: background-color 0.3s;";
  html += "}";
  html += "input[type='range']::-webkit-slider-thumb {";
  html += "  appearance: none;";
  html += "  width: 18px;";
  html += "  height: 18px;";
  html += "  border-radius: 50%;";
  html += "  background-color: #fff;";
  html += "  border: 2px solid #3498db;";
  html += "  cursor: pointer;";
  html += "}";
  html += "input[type='range']::-webkit-slider-thumb:hover {";
  html += "  background-color: #ffffff;";
  html += "}";
  html += "input[type='range']::-moz-range-thumb {";
  html += "  width: 18px;";
  html += "  height: 18px;";
  html += "  border-radius: 50%;";
  html += "  background-color: #fff;";
  html += "  border: 2px solid #3498db;";
  html += "  cursor: pointer;";
  html += "}";
  html += "input[type='range']::-moz-range-thumb:hover {";
  html += "  background-color: #3498db;";
  html += "}";
  html += "input[type='submit']:hover {";
  html += "  background-color: #2980b9;";
  html += "}";
  html += "input[type='text'] {";
  html += "  width: calc(100% - 20px);";
  html += "  padding: 10px;";
  html += "  margin: 10px 0;";
  html += "  border: none;";
  html += "  border-radius: 5px;";
  html += "  background-color: #f2f2f2;";
  html += "  text-align: center;";
  html += "  color: #555;";
  html += "}";
  html += ".restart-btn {";
  html += "  background-color: #e74c3c;";
  html += "}";
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<h1>Control a dimmable Lamp</h1>";
  html += "<form action='/setBrightness' method='POST' onsubmit='reloadWebsite()'>";
  html += "  <label id='labelBrightness' for='brightness'>Brightness (0-100): </label>";
  html += "  <input type='range' id='brightness' name='brightness' min='0' max='100' value='" + String(dimmableLight.GetLoadLevelStatus()) + "' onchange='updateTextInput(this.value, \"brightnessValue\")' control-id='ControlID-1'>";
  html += "  <input type='text' id='brightnessValue' name='brightnessValue' aria-labelledby='labelBrightness' value='" + String(dimmableLight.GetLoadLevelStatus()) + "' readonly='control-id=\"ControlID-1\"' control-id='ControlID-2'>";
  html += "  <input type='submit' value='Set Brightness' control-id='ControlID-3'>";
  html += "</form>";
  html += "<form action='/stepUp' method='POST' onsubmit='reloadWebsite()'>";
  html += "  <input type='submit' value='Step up' control-id='ControlID-6'>";
  html += "</form>";
  html += "<form action='/stepDown' method='POST' onsubmit='reloadWebsite()'>";
  html += "  <input type='submit' value='Step down' control-id='ControlID-7'>";
  html += "</form>";
  html += "<form action='/startRampUp' method='POST' onsubmit='reloadWebsite()'>";
  html += "  <input type='submit' value='Start of the ramp-up' control-id='ControlID-8'>";
  html += "</form>";
  html += "<form action='/startRampDown' method='POST' onsubmit='reloadWebsite()'>";
  html += "  <input type='submit' value='Start of the ramp-down' control-id='ControlID-9'>";
  html += "</form>";
  html += "<form action='/setOnEffectLevel' method='POST' onsubmit='reloadWebsite()'>";
  html += "  <label id='labelOnEffectLevel' for='OnEffectLevel'>OnEffectLevel (0-100): </label>";
  html += "  <p>'OnEffectLevel' is a variable or parameter used to control the degree or intensity of a particular visual or auditory effect that occurs when a dimmable lamp is switched on or activated.</p>";
  html += "  <input type='range' id='OnEffectLevel' name='OnEffectLevel' min='0' max='100' value='" + String(retOnEffectLevel) + "' onchange='updateTextInput(this.value, \"OnEffectLevelValue\")'  control-id='ControlID-4'>";
  html += "  <input type='text' id='OnEffectLevelValue' name='OnEffectLevelValue' aria-labelledby='labelOnEffectLevel' value='" + String(retOnEffectLevel) + "' readonly='control-id=\"ControlID-4\"'  control-id='ControlID-5'>";
  html += "  <input type='submit' value='Set on effect level' control-id='ControlID-5'>";
  html += "</form>";
  html += "<form action='/restart' method='POST' onsubmit='showRestartMessage()'>";
  html += "  <label for='restartButton'>ESP8266: </label>";
  html += "  <input type='submit' id='restartButton' value='Restart' style='background-color: #e74c3c;' control-id='ControlID-10'>";
  html += "</form>";
  html += "<script>";
  html += "function updateTextInput(val, inputId) {";
  html += "  document.getElementById(inputId).value = val;";
  html += "}";
  html += "function showRestartMessage() {";
  html += "  alert('Scheduled restart action for the system - Automatic website refresh in 3 seconds');";
  html += "  setTimeout(function(){ location.reload(); }, 3000);";
  html += "}";
  html += "function reloadWebsite() {";
  html += "  setTimeout(function(){ location.reload(); }, 250);";
  html += "}";
  html += "</script>";
  html += "</body>";
  html += "</html>";

  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.sendHeader("X-Content-Type-Options", "nosniff");
  server.send(200, "text/html; charset=utf-8", html);
}

void handleSetBrightness() {
  if (server.hasArg("brightness")) {
    int brightness = server.arg("brightness").toInt();
    dimmableLight.SetLoadLevelTarget(brightness);
    server.send(204);
  } else {
    server.send(400, "text/plain", "Invalid request");
  }
}

void handleSetOnEffectLevel() {
  if (server.hasArg("OnEffectLevel")) {
    int brightness = server.arg("OnEffectLevel").toInt();
    dimmableLight.SetOnEffectLevel(brightness);
    EEPROM.put(eepromAddress, brightness);
    EEPROM.commit();
    server.send(204);
  } else {
    server.send(400, "text/plain", "Invalid request");
  }
}

void handleRestart() {
  server.send(204);
  delay(500);
  ESP.restart();
}

void handleStepUp() {
  server.send(204);
  dimmableLight.StepUp();
}

void handleStepDown() {
  server.send(204);
  dimmableLight.StepDown();
}

void handleStartRampUp() {
  server.send(204);
  dimmableLight.StartRampUp();
}

void handleStartRampDown() {
  server.send(204);
  dimmableLight.StartRampDown();
}