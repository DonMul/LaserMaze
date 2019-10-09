#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define laserPin D1
#define photoresistor A0

HTTPClient http;


#define USE_SERIAL Serial

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
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {  
  digitalWrite (laserPin, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);

  int sensor = analogRead(photoresistor);

Serial.println(sensor);
  if (sensor > 200) {
    if (lock == false) {
      
  digitalWrite(LED_BUILTIN, LOW);
      lock = true;
        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        http.begin("42.42.42.42", 80, "/punish"); //HTTP
        USE_SERIAL.print("[HTTP] GET...\n");
        
        http.GET();
        http.end();
        
  digitalWrite(LED_BUILTIN, HIGH);
    }
  } else {
    if (lock == true) {
      lock = false;
    }
  }
  
  digitalWrite(LED_BUILTIN, LOW);
}
