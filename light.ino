#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <limits.h>
#include "color.h"

#define TOUCH_PIN 4
#define R_PIN 13
#define G_PIN 12
#define B_PIN 14
#define PWMRANGE_CUSTOM 255

const char* ssid     = "ssid";
const char* password = "not my real password";

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

// A webserver
ESP8266WebServer server(80);

void (*void_function)();

// Time syncronizing stuff (time.ino)
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
void setupTime();
unsigned long sendNTPpacket(IPAddress& address);
void syncTimeStart();
void timeLoopCall();
void testColorSequence();
double lightRedShift(unsigned long currentTime);

// Web server functions (server.ino)
void handle404();
void handleColor();
void handleOff();
void handleStartAlarm();
void handleSparkle();
void handleRainbow();
void handleStatus();
void handleRoot();

// LED manipulation over PWM
void startTransition(rgb_color to, unsigned long duration);
void startSunrise(unsigned long duration);
void startSparkling();
void startRainbow(unsigned long speed);
void wakeupAlarm();
void writeOff();
void writeColor(rgb_color color);
uint scaledPWM(float intensity);
uint linearPWM(float intensity);
void colorLoopCall();
rgb_color redShiftColor(rgb_color normal);

// color transition
rgb_color transitionStartColor = {0, 0, 0};
rgb_color transitionEndColor = {0, 0, 0};
unsigned long transitionStartTime  = 0;
unsigned long transitionEndTime = 0;
unsigned long transitionSpeed = 0;
void touchLoopCall();

// lamp states
const int DIM_STATES = 6;
const int OFF_STATE = 0;
const int INVALID_STATE = -1;
const int SUNRISE_STATE = -2;
const int CUSTOM_STATE = -3;
const int SPARKLE_STATE = -4;
const int RAINBOW_STATE = -5;
int state = OFF_STATE;

// globals to handle touch sensor
int wasTouching = 0;
unsigned long touchStart = 0;
unsigned long nextTouch = 0;
unsigned long lastTouch = 0;
const unsigned long TOUCH_DELAY = 50; // poll for touches every 50 ms
const unsigned long LONG_TOUCH = 1000; // 1 second
const unsigned long STATE_DELAY = 5000; // turn off on new touches after 5 seconds

// global state of lamp -- should always be in sync with reality
rgb_color currentColor = {0, 0, 0};

// global time since reset, to avoid calling millis more than once a loop
unsigned long current_millis = 0;

void testDaylightOffset();
void testLightRedShift();

void assert(bool result, String msg) {
  if (!result) {
    Serial.print("F");
    Serial.println();
    Serial.println(msg);
  } else {
    Serial.print(".");
  }
}

void runTests() {
  Serial.println("Running tests");
  testDaylightOffset();
  testLightRedShift();
  Serial.println("Done with tests");
  Serial.println();
  Serial.println();
}

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
  delay(5000); // Wait a moment before starting up
  digitalRead(TOUCH_PIN); // clear

  testColorSequence();
  //runTests();

  // connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

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

  // ntp time sync
  setupTime();

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  // webserver routing
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/off", handleOff);
  server.on("/sunrise", handleStartAlarm);
  server.on("/sparkle", handleSparkle);
  server.on("/rainbow", handleRainbow);
  server.on("/color", handleColor);
  server.onNotFound(handle404);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  current_millis = millis();

  timeLoopCall(); // logic to update current time asyncronously
  touchLoopCall(); // logic to handle touches
  colorLoopCall(); // logic to fade between colors
  server.handleClient(); // webserver

  Alarm.delay(0); // allow alarm processing
}

void touchLoopCall() {
  // rate limit to once every TOUCH_DELAY
  if (current_millis > nextTouch) {
    nextTouch = current_millis + TOUCH_DELAY;

    int touching = digitalRead(TOUCH_PIN);
//    Serial.print("touching: ");
//    Serial.print(!!touching);
//    Serial.print(", wasTouching: ");
//    Serial.print(!!wasTouching);
//    Serial.println();

    // don't register multiple touches when the person's touch hasn't ended
    if (touching) {
      if (!wasTouching) {
        touchStart = current_millis;

        // if we're not in a dim state (e.g. sunrise) set off
        // this means lamp will turn off
        if (state < 0) {
          state = DIM_STATES - 1;
        }

        // increment state by one (loop down at max) unless it's been off for a while
        // then turn off
        state = int(current_millis <= lastTouch | state == OFF_STATE) * (state + 1) % DIM_STATES;
        byte intensity = byte((float(state) / float(DIM_STATES - 1)) * 255);
        rgb_color newColor = (rgb_color) {intensity, intensity, intensity};

        Serial.print("state: ");
        Serial.println(state);
        // fade to next dim level (or off)
        startTransition(newColor, 500);
      } else {
        if (current_millis - touchStart > LONG_TOUCH && state != OFF_STATE) {
          state = DIM_STATES - 1; // brightest
          startTransition({255, 255, 255}, 500);
        }
      }
    }

    // rate limiting
    if (touching) {
      lastTouch = current_millis + STATE_DELAY;
    }

    wasTouching = touching;
  }
}

void colorLoopCall() {
  rgb_color newColor;
  
  // fade to next color without blocking
  if (transitionEndTime > current_millis) {
    unsigned long duration = transitionEndTime - transitionStartTime;
    float percent = float(current_millis - transitionStartTime) / duration;

    constrain(percent, 0, 1);

    byte randByte;
    float hue;

    switch (state) {
    case SUNRISE_STATE:
      newColor = sunriseInterpolation(percent);
      if (percent == 1) {
        state = DIM_STATES - 1; // highest brightness
      }
      break;
    case SPARKLE_STATE:
      randByte = byte(random(100, 255));
      newColor = {randByte, randByte, randByte};
      break;
    case RAINBOW_STATE:
      hue = float(current_millis % transitionSpeed) / transitionSpeed;
      newColor = hsv_to_rgb({hue, 1, 1});
      Serial.print("rainbow: ");
      Serial.print(hue);
      Serial.print(", ");
      Serial.println(rgb_to_hex(newColor), HEX);
      break;
    default:
      newColor = interpolateColor(transitionStartColor, transitionEndColor, percent);
    }

    if (state > OFF_STATE) {
      newColor = redShiftColor(newColor);
    }
    writeColor(newColor);

    Serial.print("transition: ");
    Serial.print(percent * 100);
    Serial.print("%, ");
    Serial.println(rgb_to_hex(newColor), HEX);
  } else if (!equals(transitionEndColor, currentColor)) {
    // ensure we always end on the end color, and don't stop early
    
    Serial.print("transition: ");
    Serial.print(100);
    Serial.print("%, ");
    Serial.println(rgb_to_hex(transitionEndColor), HEX);

    newColor = transitionEndColor;

    if (state > OFF_STATE) {
      newColor = redShiftColor(newColor);
    }

    // TODO: redshift bugfixing
    transitionEndColor = newColor;
    
    writeColor(newColor);
  }
  

  //writeColor(newColor);
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

void startSparkling() {
  state = SPARKLE_STATE;
  transitionEndTime = ULONG_MAX;
}

void startRainbow(unsigned long speed) {
  state = RAINBOW_STATE;
  transitionSpeed = speed;
  transitionEndTime = ULONG_MAX;
}

// default sunrise
void wakeupAlarm() {
  Serial.println("Wake up!");
  startSunrise(1000 * 60 * 30);
}

void writeOff() {
  currentColor = {0, 0, 0};
  transitionEndTime = 0;
  analogWrite(R_PIN, /*PWMRANGE_CUSTOM - */0);
  analogWrite(G_PIN, /*PWMRANGE_CUSTOM - */0);
  analogWrite(B_PIN, /*PWMRANGE_CUSTOM - */0);
}

void writeColor(rgb_color color) {
  currentColor = color;
  analogWrite(R_PIN, /*PWMRANGE_CUSTOM - */scaledPWM(float(color.r) / 255));
  analogWrite(G_PIN, /*PWMRANGE_CUSTOM - */scaledPWM(float(color.g) / 255));
  analogWrite(B_PIN, /*PWMRANGE_CUSTOM - */scaledPWM(float(color.b) / 255));
}

uint scaledPWM(float intensity) {
  // PWMRANGE_CUSTOM should be 255
  return uint(double(PWMRANGE_CUSTOM) * pow(intensity, 2));
}

uint linearPWM(float intensity) {
  return uint(double(PWMRANGE_CUSTOM) * intensity);
}

void testColorSequence() {
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
}

rgb_color redShiftColor(rgb_color normal) {
  rgb_color fullRed = {255, 147, 41};
  rgb_color fullWhite = {255, 255, 255};
  double redShift = lightRedShift(now());
  Serial.print("redShift: ");
  Serial.println(redShift);
  rgb_color shift = interpolateColor(fullWhite, fullRed, redShift);
  return {
    byte(double(shift.r) * (double(normal.r) / 255.0)),
    byte(double(shift.g) * (double(normal.g) / 255.0)),
    byte(double(shift.b) * (double(normal.b) / 255.0))
  };
}

