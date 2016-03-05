void handleRoot() {
  // TODO: I bet there's a better way to log (get full url or something)
  Serial.println(server.method() + " /");
  if (server.method() == HTTP_GET) {
    server.send(200, "text/html", index_html);
  } else {
    server.send(405, "text/plain", "{\"error\":\"method not allowed\"}");
  }
}

void handleStatus() {
  Serial.println(server.method() + " /status");
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
    message +=  (equals(currentColor, blackColor) ? "false" : "true");
    message +=  ",";
    message +=  "\"state\":";
    message +=  state;
    message +=  ",";
    message +=  "\"time\":\"";
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
  Serial.println(server.method() + " /color");
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
  Serial.println(server.method() + " /off");
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
  Serial.println(server.method() + " /alarm/start");
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

void handleAppIcon() {
  String data = "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";
  server.send(200, "image/png", data);
}

void handle404() {
  Serial.println(server.method() + " 404");
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
