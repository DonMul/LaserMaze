#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Start button
#define pushButton D8

HTTPClient http;

const char* ssid = "laser";
const char* password = "lasermaze";

int previousState = 0;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  pinMode(pushButton, INPUT);
}

void loop() {
    int stateButton = digitalRead(pushButton);

    if (stateButton != previousState) {
      previousState = stateButton;

      if (stateButton == HIGH) {
        http.begin("http://192.168.4.1/start");
        http.end();
      }
    }
}
