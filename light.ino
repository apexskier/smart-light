#include <ESP8266WiFi.h>
#include <SPI.h>
#include "Adafruit_TLC59711.h"

const byte READ = 0b11111100;  // SCP1000's read command
const byte WRITE = 0b00000010; // SCP1000's write command

const char* ssid     = "ssid";
const char* password = "not my real password";

#define touchPin 0
#define tlcPin 10

#define NUM_TLC59711 1 // number of boards chained
#define data 11
#define clck 13

Adafruit_TLC59711 tlc = Adafruit_TLC59711(NUM_TLC59711, clck, data);

uint16_t colorVal(float f);
void setColor(uint16_t r, uint16_t g, uint16_t b);
void setOff();

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  pinMode(touchPin, INPUT);
  pinMode(tlcPin, OUTPUT);
  
  tlc.begin();
  tlc.write();
  
  delay(100);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

  int touching = digitalRead(touchPin);

  if (touching) {
    setColor(colorVal(rand()), colorVal(rand()), colorVal(rand()));
  }
}

uint16_t colorVal(float f) {
  return uint16_t (f * 65535);
}

void setColor(uint16_t r, uint16_t g, uint16_t b) {
  for (uint16_t i = 0; i < 4; i++) {
    tlc.setLED(i, r, g, b);
    tlc.write();
  }
}

void setOff() {
  for (uint16_t i = 0; i < 4; i++) {
    tlc.setLED(i, 0, 0, 0);
    tlc.write();
  }
}

