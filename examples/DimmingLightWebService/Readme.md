# DimmingLightWebService
In this example, the brightness of the dimmable lamp is controlled via a simple web interface. 

## Description
The provided sketch implements a simple web server application for the ESP8266 microcontroller that allows a dimmable lamp to be controlled via a web interface. The application uses the ESP8266WiFi and ESP8266WebServer libraries to establish a WiFi connection and process HTTP requests.

The sketch first initialises the WLAN connection with the specified SSID and password information. An instance of the DimmingLight class is then created to enable control of the dimmable lamp.

The web server is started on port 80 and two routes are defined: The main page ("/"), which displays the current brightness of the lamp and provides a form to  change the brightness, and "/setBrightness", which is called to change the brightness based on the values entered in the form.

The handleRoot() function is called to serve the main page, while handleSetBrightness() is called to change the brightness when the form is submitted.

The main programme in the loop() function handles the web server and waits for incoming HTTP requests while other tasks can be performed at the same time.

Overall, the sketch provides a simple way to control a dimmable lamp via a web interface and can be easily customised to suit different requirements.

## Usage
Here is a step-by-step guide on how to change the brightness:

1. connect your computer or a mobile device to the same WLAN network as your ESP8266 board.

2. open a web browser and enter the IP address of your ESP8266 board in the address bar. You should be able to see the IP address on the serial monitor of the Arduino IDE after your board has been successfully connected to the WLAN.

3. press enter to go to the main page of the web server application. The current brightness of the dimmable lamp is displayed there.

4. enter the desired brightness value in the text field. The values should be in the range from 0 to 100.

5. click on the "Set" button to change the brightness. The web server application will then send an HTTP POST request to the URL "/setBrightness" with the new brightness value as a parameter.

6) The ESP8266 web server receives the request and changes the brightness of the dimmable lamp according to the received value.

7. the new brightness is updated on the main page of the web interface and you can repeat the process to further adjust the brightness if necessary.

By using this web interface, you can conveniently control the brightness of the dimmable lamp from any device connected to the same Wi-Fi network without having to install additional software or apps.

## List of components
|Name|Quantity|Component|
|---|---|---|
|U1|1|NodeMCU 1.0 (ESP-12E)|
|R1|1|100 Ohm resistor|
|D1|1|Blue LED|

## Sketch
Here is the `examples\DimmingLightWebService\` example code:

```ino
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DimmingLight.h"

// WiFi Credentials
#ifndef STASSID
#define STASSID "SSID"
#define STAPSK "PASSWORD"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

// Instance of the DimmingLight class
DimmingLight dimmableLight(D1);  // Adjust the pin according to your hardware

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


  // Initialize dimmable lamp
  dimmableLight.begin();

  // Set routes for the web server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/setBrightness", HTTP_POST, handleSetBrightness);

  // Starting the web server
  server.begin();
}

void loop() {
  // Handle web server
  server.handleClient();

  // Implement your other code here if required
}

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>dimmable lamp</h1>";
  html += "<p>Current brightness: " + String(dimmableLight.GetLoadLevelTarget()) + "</p>";
  html += "<form action='/setBrightness' method='POST'>";
  html += "<label for='brightness'>brightness (0-100): </label>";
  html += "<input type='text' id='brightness' name='brightness' maxlength='3' required>";
  html += "<input type='submit' value='Set'>";
  html += "</form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

/*
  Note that the sketch does not provide any error handling for invalid entries when setting the brightness. It may be useful to add additional checks to ensure that the inputs are valid.
*/
void handleSetBrightness() {
  if (server.hasArg("brightness")) {
    int brightness = server.arg("brightness").toInt();
    dimmableLight.SetLoadLevelTarget(brightness);
    server.send(200, "text/plain", "Brightness successfully set");
  } else {
    server.send(400, "text/plain", "Invalid request");
  }
}
```

> [!Note] 
> Make sure you have implemented the DimmingLight library correctly and that it can be used with the ESP8266 board.

> [!Note]
> Make sure you set the correct pin number for the dimmable lamp connector. In this example, `D1` is used, but you should change the pin according to your actual hardware configuration.

## ChangeLog

|Version|State|Comment|
|---|---|---|
|1.0.0.0|done|Initial Commit of the DimmingLightWebService Example|

