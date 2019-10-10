#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Start button
#define pushButton D8

const char* ssid = "laser";
const char* password = "lasermaze";

int previousState = 0;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    
    tryWifiConnect();

    pinMode(pushButton, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
}

void tryWifiConnect() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to Access point..");
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(100);
    }
    
    Serial.println("connected");
  }
}

void loop() {
    tryWifiConnect();
    digitalWrite(LED_BUILTIN, HIGH);
    
    int stateButton = digitalRead(pushButton);

    if (stateButton != previousState) {
      previousState = stateButton;

      if (stateButton == HIGH) {
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("Send start");
        
        HTTPClient http;
        http.begin("42.42.42.42", 80, "/start");
        int httpCode = http.GET();
        http.end();
        
        Serial.println("Start send");
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }
    
    digitalWrite(LED_BUILTIN, LOW);
}
