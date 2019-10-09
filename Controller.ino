#include <Wire.h>
#include <TM1637Display.h>
#include <DS3231.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define pushButton D8

// TM1637 Display
#define CLK D3
#define DIO D4

TM1637Display display(CLK, DIO);
ESP8266WebServer server(80);

// RTC
RTClib  rtc;

int startTimestamp = 0;
int curTime = 0;
bool timerRunning = true;

const IPAddress apIP = IPAddress(42, 42, 42, 42);

void setup() {
  Serial.begin(9600);
  delay(1000);
  Wire.begin();

  display.showNumberDecEx(0, (0x80 >> 1), true);
  display.setBrightness(7, true);
  DateTime now = rtc.now();
  startTimestamp = now.unixtime();

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  boolean result = WiFi.softAP("laser", "lasermaze");
  
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

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
    DateTime now = rtc.now();
    curTime = now.unixtime() - startTimestamp;

    Serial.println(now.unixtime());

    int stateButton = digitalRead(pushButton);
    if (stateButton == 1) {
      handleStop(); 
    }
  }
  
  display.showNumberDec(curTime);
}

void handleStart() {
  server.send(200, "text/plain", "start");
  Serial.println("Start");

  if (timerRunning == true) {
    return;
  }
  DateTime now = rtc.now();
  startTimestamp = now.unixtime();
  timerRunning = true;
}

void handleStop() {
  server.send(200, "text/plain", "stop");
  timerRunning = false;
}


void handlePunish() {
  server.send(200, "text/plain", "punish");
  if (timerRunning == false) {
    return;
  }
  
  startTimestamp = startTimestamp - 5;
}
