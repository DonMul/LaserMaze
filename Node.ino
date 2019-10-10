#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define laserPin D1
#define photoresistor A0

#define USE_SERIAL Serial

const char* ssid = "laser";
const char* password = "lasermaze";

bool lock = false;

void setup() {
  Serial.begin(9600);
  
  tryWifiConnect();
  
  pinMode(laserPin, OUTPUT);
  pinMode(photoresistor, INPUT);
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
  digitalWrite (laserPin, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);

  int sensor = analogRead(photoresistor);

  if (sensor > 200 && lock == false) {
    digitalWrite(LED_BUILTIN, LOW);
    
    lock = true;

    Serial.println("Send punish");
    HTTPClient http;
    http.begin("42.42.42.42", 80, "/punish");
    http.GET();
    http.end();
    Serial.println("Punish send");
      
    digitalWrite(LED_BUILTIN, HIGH);
  } else if (sensor < 200 && lock == true) {
    lock = false;
  }
  
  digitalWrite(LED_BUILTIN, LOW);
}
