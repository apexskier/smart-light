#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include "color.h"

#define TOUCH_PIN 16
#define R_PIN 2
#define G_PIN 4
#define B_PIN 13
#define PWMRANGE_CUSTOM 255

const char* ssid     = "ssid";
const char* password = "not my real password";

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

void (*void_function)();


// Time syncronizing stuff
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

void setupTime();
unsigned long sendNTPpacket(IPAddress& address);
void syncTimeStart();
void timeLoopCall();

void touchLoopCall();
void colorLoopCall();


void startTransition(rgb_color to, unsigned long duration);
void startSunrise(unsigned long duration);
void startTestSunrise();
void wakeupAlarm();
void writeOff();
void writeColor(rgb_color color);
uint scaledPWM(float intensity);
uint linearPWM(float intensity);


const int DIM_STATES = 8;
const int OFF_STATE = 0;
const int SUNRISE_STATE = -1;
int state = 0;
int wasTouching = 0;


unsigned long nextTouch = 0;
unsigned long lastTouch = 0;
unsigned long touchDelay = 100;
unsigned long stateDelay = 5000;

rgb_color transitionStartColor = {0, 0, 0};
rgb_color transitionEndColor = {0, 0, 0};
unsigned long transitionStartTime  = 0;
unsigned long transitionEndTime = 0;

rgb_color currentColor = {0, 0, 0};

// Global time since reset, to avoid calling millis more than once a loop
unsigned long current_millis = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  pinMode(TOUCH_PIN, INPUT);
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

  // Set PWM max to 255, to match color vals exactly. Easier processing
  analogWriteRange(PWMRANGE_CUSTOM);

  writeOff();

  writeColor(hex_to_rgb(0xff0000));
  delay(1000);
  writeColor(hex_to_rgb(0x00ff00));
  delay(1000);
  writeColor(hex_to_rgb(0x0000ff));
  delay(1000);

  writeColor(hex_to_rgb(0xffff00));
  delay(1000);
  writeColor(hex_to_rgb(0xff00ff));
  delay(1000);
  writeColor(hex_to_rgb(0x00ffff));
  delay(1000);

  writeColor(hex_to_rgb(0xffffff));
  delay(200);
  writeColor(hex_to_rgb(0xcccccc));
  delay(200);
  writeColor(hex_to_rgb(0x999999));
  delay(200);
  writeColor(hex_to_rgb(0x666666));
  delay(200);
  writeColor(hex_to_rgb(0x333333));
  delay(200);
  writeColor(hex_to_rgb(0x000000));
  delay(200);

  writeOff();

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Alarm.delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  udp.begin(2390); // start listening on this port
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

  // setup time
  setupTime();

  Alarm.timerRepeat(10, startTestSunrise);

  Alarm.alarmRepeat(dowMonday, 6, 0, 0, wakeupAlarm);
  Alarm.alarmRepeat(dowTuesday, 6, 0, 0, wakeupAlarm);
  Alarm.alarmRepeat(dowWednesday, 6, 0, 0, wakeupAlarm);
  Alarm.alarmRepeat(dowThursday, 6, 0, 0, wakeupAlarm);
  Alarm.alarmRepeat(dowFriday, 6, 0, 0, wakeupAlarm);
}

void startTestSunrise() {
  Serial.println("Timer test");
}

void wakeupAlarm() {
  startSunrise(1000 * 60 * 30);
}

void loop() {
  // put your main code here, to run repeatedly:
  current_millis = millis();

  timeLoopCall();
  touchLoopCall();
  colorLoopCall();
}

void touchLoopCall() {
  if (current_millis > nextTouch) {
    nextTouch = current_millis + touchDelay;

    int touching = digitalRead(TOUCH_PIN);

    if (touching & !wasTouching) {
      if (state < 0) {
        state = OFF_STATE;
      }

      state = int(current_millis <= lastTouch | state == OFF_STATE) * (state + 1) % DIM_STATES;
      byte intensity = byte(scaledPWM(float(state) / float(DIM_STATES - 1)));
      rgb_color newColor = (rgb_color) {intensity, intensity, intensity};
      startTransition(newColor, 500);
    }

    if (touching) {
      lastTouch = current_millis + stateDelay;
    }

    wasTouching = touching;
  }
}

void colorLoopCall() {
  if (transitionEndTime > current_millis) {
    unsigned long duration = transitionEndTime - transitionStartTime;

    float percent = float(current_millis - transitionStartTime) / duration;

    // clamp percent
    if (percent >= 1) {
      percent = 1;
    } else if (percent <= 0) {
      percent = 0;
    }

    rgb_color newColor;
    if (state == SUNRISE_STATE) {
      newColor = sunriseInterpolation(percent);
    } else {
      newColor = interpolateColor(transitionStartColor, transitionEndColor, percent);
    }

    Serial.print("transition: ");
    Serial.print(percent * 100);
    Serial.print("%, ");
    Serial.println(rgb_to_hex(newColor), HEX);

    writeColor(newColor);
  } else if (!equals(transitionEndColor, currentColor)) {
    Serial.print("transition: ");
    Serial.print(100);
    Serial.print("%, ");
    Serial.println(rgb_to_hex(transitionEndColor), HEX);
    writeColor(transitionEndColor);
  }
}

void startTransition(rgb_color to, unsigned long duration) {
  transitionStartTime = current_millis;
  transitionEndTime = current_millis + duration;
  transitionStartColor = currentColor;
  transitionEndColor = to;
}

void startSunrise(unsigned long duration) {
  transitionStartTime = current_millis;
  transitionEndTime = current_millis + duration;
  state = SUNRISE_STATE;
  transitionStartColor = currentColor;
  transitionEndColor = {255, 255, 255};
  Serial.println("Started sunrise");
}

void writeOff() {
  currentColor = {0, 0, 0};
  transitionEndTime = 0;
  analogWrite(R_PIN, PWMRANGE_CUSTOM - 0);
  analogWrite(G_PIN, PWMRANGE_CUSTOM - 0);
  analogWrite(B_PIN, PWMRANGE_CUSTOM - 0);
}

void writeColor(rgb_color color) {
  currentColor = color;
  analogWrite(R_PIN, PWMRANGE_CUSTOM - color.r);
  analogWrite(G_PIN, PWMRANGE_CUSTOM - color.g);
  analogWrite(B_PIN, PWMRANGE_CUSTOM - color.b);
}

uint scaledPWM(float intensity) {
  // PWMRANGE_CUSTOM should be 255
  return uint(double(PWMRANGE_CUSTOM) * pow(intensity, 2));
}

uint linearPWM(float intensity) {
  return uint(double(PWMRANGE_CUSTOM) * intensity);
}

