void handleRoot() {
  static rgb_color blackColor = {0, 0, 0};
  if (server.method() == HTTP_GET) {
    String message;
    message += "{\"health\":\"okay\",";
    message +=  "\"color\":{";
    message +=    "\"r\":" + String(currentColor.r) + ",";
    message +=    "\"g\":" + String(currentColor.g) + ",";
    message +=    "\"b\":" + String(currentColor.b);
    message +=  "},";
    message +=  "\"on\":";
    message +=  (equals(currentColor, blackColor) ? "true" : "false");
    message +=  ",";
    message +=  "\"state\":";
    message +=  state;
    message +=  ","
    message +=  "\"time\": \"";
      message += year();
      message += '-';
      message += month();
      message += '-';
      message += day();
      message += 'T';
      int hour_ = hour();
      if (hour_ < 10) message += "0";
      message += hour_;
      int min_ = minute();
      message += ":";
      if (min_ < 10) message += "0";
      message += min_;
      int sec_ = second();
      message += ":";
      if (sec_ < 10) message += "0";
      message += sec_;
    message += "\"}";
    server.send(200, "application/json", message);
  } else {
    server.send(405, "text/plain", "{\"error\":\"method not allowed\"}");
  }
}

void handleColor() {
  String message = "{";
  if (server.method() == HTTP_GET) {
    message += "\"r\":";
    message += currentColor.r;
    message += ",\"g\":";
    message += currentColor.g;
    message += ",\"b\":";
    message += currentColor.b;
    message += "}";
    server.send(200, "application/json", message);
  } else {
    // post
    unsigned long duration = 0;
    rgb_color newColor = {0, 0, 0};
    bool colorSet = false;
    uint8_t numArgs = server.args();
    if (numArgs > 0) {
      for (uint8_t i = 0; i < numArgs; i++) {
        String argName = server.argName(i);
        if (argName == "duration") {
          duration = server.arg(i).toInt();
          colorSet = true;
        } else if (argName == "plain") {
          // ignore TODO: find out why this exists
        } else if (argName == "r") {
          newColor.r = byte(server.arg(i).toInt());
          colorSet = true;
        } else if (argName == "g") {
          newColor.g = byte(server.arg(i).toInt());
          colorSet = true;
        } else if (argName == "b") {
          newColor.b = byte(server.arg(i).toInt());
          colorSet = true;
        } else {
          message += "\"error\":\"invalid argument name: '" + server.argName(i) + "'\"}";
          server.send(400, "text/plain", message);
          return;
        }
      }
    }

    if (colorSet) {
      message += "\"r\":";
      message += newColor.r;
      message += ",\"g\":";
      message += newColor.g;
      message += ",\"b\":";
      message += newColor.b;
      message += ",\"duration\":";
      message += duration;
      message += ",\"success\":true";
      message += "}";
      state = CUSTOM_STATE;
      startTransition(newColor, duration);
      server.send(200, "application/json", message);
    } else {
      server.send(400, "text/plain", "{\"error\":\"invalid action\"}");
      return;
    }
  }
}

void handleOff() {
  String message = "{";
  unsigned long duration = 0;

  uint8_t numArgs = server.args();
  if (numArgs > 0) {
    for (uint8_t i = 0; i < numArgs; i++) {
      String argName = server.argName(i);
      if (argName == "duration") {
        duration = server.arg(i).toInt();
      } else if (argName == "plain") {
        // ignore TODO: find out why this exists
      } else {
        message += "\"error\":\"invalid argument name: '" + server.argName(i) + "'\"}";
        server.send(400, "text/plain", message);
        return;
      }
    }
  }

  state = OFF_STATE;
  startTransition({0, 0, 0}, duration);

  message += "\"success\":true";
  message += "}";
  server.send(200, "application/json", message);
}

void handleStartAlarm() {
  String message = "{";
  unsigned long duration = 1000 * 60 * 30;

  uint8_t numArgs = server.args();
  if (numArgs > 0) {
    for (uint8_t i = 0; i < numArgs; i++) {
      String argName = server.argName(i);
      if (argName == "duration") {
        duration = server.arg(i).toInt();
      } else if (argName == "plain") {
        // ignore TODO: find out why this exists
      } else {
        message += "\"error\":\"invalid argument name: '" + server.argName(i) + "'\"}";
        server.send(400, "text/plain", message);
        return;
      }
    }
  }

  startSunrise(duration);

  message += "\"success\": true";
  message += "}";
  server.send(200, "application/json", message);
}

void handle404() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  // TODO: support more methods?
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

