#include <Wire.h>
#include <TM1637Display.h>
#include "DS3231.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// End button
#define pushButton D8

// WiFi
const IPAddress apIP = IPAddress(192, 168, 4, 1);
const char* ssid = "laser";
const char* password = "lasermaze";

// TM1637 Display
#define CLK D3
#define DIO D4

TM1637Display display(CLK, DIO);
ESP8266WebServer server(80);

// RTC
RTClib RTC;

int startTimestamp = 0;
int curTime = 0;
bool timerRunning = true;

void setup() {
  Serial.begin(9600);
  delay(1000);
  Wire.begin();
  
  display.showNumberDecEx(0, (0x80 >> 1), true);
  display.setBrightness(7, true);
  DateTime now = RTC.now();
  startTimestamp = now.unixtime();

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
  
  server.on("/start", handleStart);
  server.on("/stop", handleStop);
  server.on("/punish", handlePunish);

  server.begin();
  pinMode(pushButton, INPUT);
  handleStart();
}

void loop() {
  server.handleClient();
  if (timerRunning == true) {
    DateTime now = RTC.now();
    curTime = now.unixtime() - startTimestamp;

    int stateButton = digitalRead(pushButton);
    if (stateButton == 1) {
      handleStop(); 
    }
  }
  
  display.showNumberDec(curTime);
}

void handleStart() {
  if (timerRunning == true) {
    return;
  }
  DateTime now = RTC.now();
  startTimestamp = now.unixtime();
  timerRunning = true;
}

void handleStop() {
  if (timerRunning == false) {
    return;
  }
  
  timerRunning = false;
}

void handlePunish() {
  if (timerRunning == false) {
    return;
  }
  
  startTimestamp = startTimestamp - 5;
}
