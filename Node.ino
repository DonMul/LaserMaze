#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define laserPin D1
#define photoresistor A0

HTTPClient http;

const char* ssid = "laser";
const char* password = "lasermaze";

bool lock = false;

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
  
  pinMode(laserPin, OUTPUT);
  pinMode(photoresistor, INPUT);
}

void loop() {  
  digitalWrite (laserPin, HIGH);

  int sensor = analogRead(photoresistor);
  if (sensor > 25) {
    if (lock == false) {
      lock = true;
      http.begin("http://192.168.4.1/punish");
      http.end();
    }
  } else {
    if (lock == true) {
      lock = false;
    }
  }
}
