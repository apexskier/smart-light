void handleRoot() {
  // TODO: I bet there's a better way to log (get full url or something)
  Serial.println(server.method() + " /");
  if (server.method() == HTTP_GET) {
    String html = "<!DOCTYPE html><meta charset=utf-8><meta name=viewport content=\"width=device-width,initial-scale=1\"><title>💡</title><style>body{font-family:-apple-system,\"Helvetica Neue\",\"Lucida Grande\",sans-serif;color:#222;padding:10px 15px;font-size:28px}h1{font-size:30px;margin-top:0}p{margin-top:0;margin-bottom:.5em;text-align:center}.lb-c{width:100%;text-align:center;margin-bottom:10px}#lb{margin:0 auto;height:100px;width:100px;border-radius:4px;border:1px solid #222;background-color:rgb("+String(currentColor.r)+","+String(currentColor.g)+","+String(currentColor.b)+")}#pv{display:inline-block;background-color:#fff;height:1em;width:50%;border-radius:4px;vertical-align:text-bottom}#msg{border-radius:2px;background-color:#ffcdcd;color:#9e0000;border:1px solid;padding:4px 8px;display:none;font-size:18px;text-align:left}input[type=range]{-webkit-appearance:none;width:100%;margin-bottom:12px}input[type=range]:focus{outline:0}input[type=range]::-webkit-slider-runnable-track{width:100%;height:4px;border-radius:2px;cursor:pointer;background:#999;box-shadow:inset 0 1px 1px rgba(0,0,0,.4)}input[type=range]::-webkit-slider-thumb{box-shadow:0 1px 6px rgba(0,0,0,.4);height:24px;width:24px;border-radius:12px;background:#aaa;cursor:pointer;-webkit-appearance:none;margin-top:-10px;background-image:linear-gradient(to bottom,rgba(255,255,255,.1) 0,rgba(0,0,0,.1) 100%)}input[type=range]:focus::-webkit-slider-runnable-track{background:#222}input#red[type=range]::-webkit-slider-thumb{border:1px solid #a00;background-color:red}input#green[type=range]::-webkit-slider-thumb{border:1px solid #0a0;background-color:#0f0}input#blue[type=range]::-webkit-slider-thumb{border:1px solid #00a;background-color:#00f}</style><h1>Smart Light &#128161;</h1><p id=msg><div class=lb-c><div id=lb></div></div><p><input id=red value=255 type=range step=1 min=0 max=255><br><input id=green value=255 type=range step=1 min=0 max=255><br><input id=blue value=255 type=range step=1 min=0 max=255><br><p class=ctl><span id=turnOff>&#128244;</span> &nbsp;&nbsp; <span id=refresh>&#128260;</span> &nbsp;&nbsp; <span id=setbtn>&#10024;</span><p><span id=pv></span></p><script>function setpv(){pv.style.backgroundColor=\"rgb(\"+newColor[0]+\",\"+newColor[1]+\",\"+newColor[2]+\")\"}function error(e){msg.textContent=\"Error: \"+(error.response||\"unknown error\"),msg.style.display=\"block\"}function update(){ajax(\"GET\",\"/status\",function(e){var t=JSON.parse(e);lb.style.backgroundColor=\"rgb(\"+t.color.r+\",\"+t.color.g+\",\"+t.color.b+\")\",msg.style.display=\"none\"},error)}function ajax(e,t,n,o){var r=new XMLHttpRequest;r.onreadystatechange=function(){r.readyState==XMLHttpRequest.DONE&&(200==r.status?n(r.responseText):o(r))},r.open(e,t,!0),r.send()}var lb=document.getElementById(\"lb\"),msg=document.getElementById(\"msg\"),pv=document.getElementById(\"pv\"),rs=document.getElementById(\"red\"),bs=document.getElementById(\"blue\"),gs=document.getElementById(\"green\"),offbtn=document.getElementById(\"turnOff\"),refreshbtn=document.getElementById(\"refresh\"),setbtn=document.getElementById(\"setbtn\"),newColor=[255,255,255];rs.addEventListener(\"input\",function(e){newColor[0]=e.target.value,setpv()}),gs.addEventListener(\"input\",function(e){newColor[1]=e.target.value,setpv()}),bs.addEventListener(\"input\",function(e){newColor[2]=e.target.value,setpv()}),offbtn.addEventListener(\"click\",function(e){ajax(\"POST\",\"/off?duration=1000\",function(){lb.style.backgroundColor=\"rgb(0, 0, 0)\"},error)}),setbtn.addEventListener(\"click\",function(e){ajax(\"POST\",\"/color?r=\"+newColor[0]+\"&g=\"+newColor[1]+\"&b=\"+newColor[2]+\"&duration=1000\",function(){lb.style.backgroundColor=\"rgb(\"+newColor[0]+\", \"+newColor[1]+\", \"+newColor[2]+\")\"},error)}),refreshbtn.addEventListener(\"click\",function(e){update()})</script>";
    server.send(200, "text/html", html);
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

