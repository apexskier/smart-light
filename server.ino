void handleRoot() {
  if (server.method() == HTTP_GET) {
    String html = "<!DOCTYPE html><meta charset=utf-8><title>Smart Light</title><style>body{font-family:sans-serif;color:#222;padding:10px 15px}.lightbox-container{width:100%;text-align:center;margin-bottom:10px}#lightbox{margin:0 auto;height:100px;width:100px;border-radius:4px;border:1px solid #222";
    html += currentColor.r;
    html += ",";
    html += currentColor.g;
    html += ",";
    html += currentColor.b;
    html += ")}#preview{display:inline-block;background-color:#fff;height:1em;width:1em;margin-left:.5em;vertical-align:text-bottom}#message{border-radius:2px;background-color:#ffcdcd;color:#9e0000;border:1px solid;padding:4px 8px;display:none}</style><h1>Smart Light</h1><p id=message><div class=lightbox-container><div id=lightbox></div></div><p><button id=turnOff>Off</button> <button id=refresh>Refresh</button><p>Red <input id=red value=255 type=range step=1 min=0 max=255><br>Green <input id=green value=255 type=range step=1 min=0 max=255><br>Blue <input id=blue value=255 type=range step=1 min=0 max=255><br><p><button id=setButton>Set</button> <span id=preview></span></p><script>function setPreview(){preview.style.backgroundColor=\"rgb(\"+newColor[0]+\",\"+newColor[1]+\",\"+newColor[2]+\")\"}function error(e){message.textContent=\"Error: \"+(error.response||\"unknown error\"),message.style.display=\"block\"}function update(){ajax(\"GET\",\"/status\",function(e){var t=JSON.parse(e);lightbox.style.backgroundColor=\"rgb(\"+t.color.r+\",\"+t.color.g+\",\"+t.color.b+\")\",message.style.display=\"none\"},error)}function ajax(e,t,n,r){var o=new XMLHttpRequest;o.onreadystatechange=function(){o.readyState==XMLHttpRequest.DONE&&(200==o.status?n(o.responseText):r(o))},o.open(e,t,!0),o.send()}var lightbox=document.getElementById(\"lightbox\"),message=document.getElementById(\"message\"),preview=document.getElementById(\"preview\"),redSlider=document.getElementById(\"red\"),blueSlider=document.getElementById(\"blue\"),greenSlider=document.getElementById(\"green\"),offButton=document.getElementById(\"turnOff\"),refreshButton=document.getElementById(\"refresh\"),setButton=document.getElementById(\"setButton\"),newColor=[255,255,255];redSlider.addEventListener(\"input\",function(e){newColor[0]=e.target.value,setPreview()}),greenSlider.addEventListener(\"input\",function(e){newColor[1]=e.target.value,setPreview()}),blueSlider.addEventListener(\"input\",function(e){newColor[2]=e.target.value,setPreview()}),offButton.addEventListener(\"click\",function(e){ajax(\"POST\",\"/off?duration=1000\",update,error)}),setButton.addEventListener(\"click\",function(e){ajax(\"POST\",\"/color?r=\"+newColor[0]+\"&g=\"+newColor[1]+\"&b=\"+newColor[2]+\"&duration=1000\",update,error)}),refreshButton.addEventListener(\"click\",function(e){update()})</script>";
    server.send(200, "text/html", html);
  } else {
    server.send(405, "text/plain", "{\"error\":\"method not allowed\"}");
  }
}

void handleStatus() {
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
    message +=  ",";
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

