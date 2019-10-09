#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
// Start button
#define pushButton D8

const char* ssid = "laser";
const char* password = "lasermaze";

int previousState = 0;

void setup() {
    USE_SERIAL.begin(115200);
    WiFi.begin(ssid, password);
    delay(1000);
    Serial.print("Connecting to Access point..");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(100);
    }
    delay(100);
    Serial.println("connected");

    pinMode(pushButton, INPUT);
    pinMode(BUILTIN_LED, OUTPUT);
}

void loop() {
    digitalWrite(BUILTIN_LED, HIGH);
    int stateButton = digitalRead(pushButton);

    if (stateButton != previousState) {
      previousState = stateButton;

      if (stateButton == HIGH) {
        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        http.begin("42.42.42.42", 80, "/start"); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
      }
    }
}
