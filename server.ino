#define ACTION_NONE 0
#define ACTION_SET_COLOR 1
#define ACTION_FADE_COLOR 2

void handleRoot() {
  static rgb_color blackColor = {0, 0, 0};
  if (server.method() == HTTP_GET) {
    String message;
    message += "{\"health\": \"okay\",";
    message +=  "\"color\": {";
    message +=    "\"r\": " + String(currentColor.r) + ",";
    message +=    "\"g\": " + String(currentColor.g) + ",";
    message +=    "\"b\": " + String(currentColor.b);
    message +=  "},";
    message +=  "\"on\": " + equals(currentColor, blackColor) ? "true" : "false";
    message += "}";
    server.send(200, "text/json", "{\"health\": \"okay\"}");
  } else {
    server.send(405, "text/plain", "{\"error\": \"method not allowed\"}");
  }
}

void handleColor() {
  String message = "{";
  if (server.method() == HTTP_GET) {
    message += "\"r\": ";
    message += currentColor.r;
    message += ", \"g\": ";
    message += currentColor.g;
    message += ", \"b\": ";
    message += currentColor.b;
    message += "}";
    server.send(200, "text/json", message);
  } else {
    // post
    uint8_t action = ACTION_NONE;
    int duration = 0;
    int numArgs = server.args();
    for (int i = 0; i < numArgs; i++) {
      String argName = server.argName(i);
      if (argName == "action") {
        action = uint8_t(server.arg(i).toInt());
      } else if (argName == "duration") {
        duration = server.arg(i).toInt();
      } else {
        message += "\"error\": \"invalid argument name: " + server.argName(i) + "\"}";
        server.send(400, "text/plain", message);
        return;
      }
    }

    switch (action) {
      case ACTION_SET_COLOR:
        // TODO
        break;
      case ACTION_NONE:
        // fallthrough
      default:
        server.send(400, "text/plain", "{\"error\": \"invalid action\"}");
        return;
    }
  }
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

