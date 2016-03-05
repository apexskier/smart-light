const String index_html_head = "<!DOCTYPE html><meta charset=utf-8><meta name=viewport content=\"width=device-width,initial-scale=1\"><meta name=apple-mobile-web-app-capable content=yes><link rel=apple-touch-icon href=http://10.0.1.4:10011/icon.png><title>💡</title><style>*{box-sizing:border-box}body{font-family:-apple-system,\"Helvetica Neue\",\"Lucida Grande\",sans-serif;color:#eee;padding:24px 16px;margin:0;font-size:28px;background-color:#000}h1{font-size:30px;margin-top:0}p{margin-top:0;margin-bottom:.5em;text-align:center}.lb-c{width:100%;text-align:center;margin-bottom:10px}.ctl{padding-top:32px}.ctl2{padding-top:16px;padding-bottom:8px}.e{font-family:AppleColorEmoji,default;position:relative;font-size:16px;transform:scale(2);display:inline-block;width:32px;height:32px;margin:0 32px}#lb{margin:0 auto;height:100px;width:100px;background-color:rgb(";
const String index_html_tail = ")}#pv{display:inline-block;background-color:#fff;height:1em;width:50%;vertical-align:text-bottom}#lb,#pv{border-radius:4px;border:1px solid #eee}#msg{border-bottom-left-radius:2px;border-bottom-right-radius:2px;background-color:#ffcdcd;color:#9e0000;border:1px solid;border-top:none;padding:4px 8px;font-size:18px;text-align:left;position:absolute;top:0;left:15px;right:15px;transition:transform .4s 0s}#msg.hide{transition:transform .4s 0s;transform:translateY(-100%)}input[type=range]{-webkit-appearance:none;width:100%;margin-bottom:12px}input[type=range]:focus{outline:0}input[type=range]::-webkit-slider-runnable-track{width:100%;height:4px;border-radius:2px;cursor:pointer;background:#999;box-shadow:inset 0 1px 1px rgba(0,0,0,.4)}input[type=range]::-webkit-slider-thumb{box-shadow:0 1px 6px rgba(0,0,0,.4);height:32px;width:32px;border-radius:16px;background:#aaa;cursor:pointer;-webkit-appearance:none;margin-top:-14px;background-image:linear-gradient(to bottom,rgba(255,255,255,.1) 0,rgba(0,0,0,.1) 100%)}input[type=range]:focus::-webkit-slider-runnable-track{background:#222}input#red[type=range]::-webkit-slider-thumb{border:1px solid #a00;background-color:red}input#green[type=range]::-webkit-slider-thumb{border:1px solid #0a0;background-color:#0f0}input#blue[type=range]::-webkit-slider-thumb{border:1px solid #00a;background-color:#00f}</style><p id=msg class=hide><div class=lb-c><div id=lb></div></div><p><input id=red value=255 type=range step=1 min=0 max=255><br><input id=green value=255 type=range step=1 min=0 max=255><br><input id=blue value=255 type=range step=1 min=0 max=255><br><p class=ctl><span id=sr class=e>&#127749;</span><span id=rb class=e>&#127752;</span><span id=spkl class=e>&#10024;</span><p class=ctl2><span id=setbtn class=e>&#128678;</span><span id=turnOff class=e>&#128244;</span><span id=refresh class=e>&#128260;</span><p><span id=pv></span></p><script>function setpv(){pv.style.backgroundColor=\"rgb(\"+newColor[0]+\",\"+newColor[1]+\",\"+newColor[2]+\")\"}function off(e){ajax(\"POST\",\"/off?duration=1000\",function(){lb.style.backgroundColor=\"rgb(0, 0, 0)\"}),e.preventDefault()}function set(e){ajax(\"POST\",\"/color?r=\"+newColor[0]+\"&g=\"+newColor[1]+\"&b=\"+newColor[2]+\"&duration=1000\",function(){lb.style.backgroundColor=\"rgb(\"+newColor[0]+\", \"+newColor[1]+\", \"+newColor[2]+\")\"}),e.preventDefault()}function rbAction(e){ajax(\"POST\",\"/rainbow\"),e.preventDefault()}function spklAction(e){ajax(\"POST\",\"/sparkle\"),e.preventDefault()}function srAction(e){ajax(\"POST\",\"/sunrise\"),e.preventDefault()}function update(e){ajax(\"GET\",\"/status\",function(e){var t=JSON.parse(e);lb.style.backgroundColor=\"rgb(\"+t.color.r+\",\"+t.color.g+\",\"+t.color.b+\")\"}),e&&e.preventDefault()}function ajax(e,t,n,o){n=n||function(){},o=o||function(){};var r=new XMLHttpRequest;r.onreadystatechange=function(){r.readyState==XMLHttpRequest.DONE&&(200==r.status?(n(r.responseText),msg.classList.add(\"hide\")):(o(r),clearTimeout(msgTO),msg.textContent=\"Error: \"+(r.response||\"unknown error\"),msg.classList.remove(\"hide\"),msgTO=setTimeout(function(){msg.classList.add(\"hide\")},2e3)))},r.open(e,t,!0),r.send()}var lb=document.getElementById(\"lb\"),msg=document.getElementById(\"msg\"),pv=document.getElementById(\"pv\"),rs=document.getElementById(\"red\"),bs=document.getElementById(\"blue\"),gs=document.getElementById(\"green\"),rb=document.getElementById(\"rb\"),spkl=document.getElementById(\"spkl\"),sr=document.getElementById(\"sr\"),offbtn=document.getElementById(\"turnOff\"),refreshbtn=document.getElementById(\"refresh\"),setbtn=document.getElementById(\"setbtn\"),newColor=[255,255,255];rs.addEventListener(\"input\",function(e){newColor[0]=e.target.value,setpv()}),gs.addEventListener(\"input\",function(e){newColor[1]=e.target.value,setpv()}),bs.addEventListener(\"input\",function(e){newColor[2]=e.target.value,setpv()}),offbtn.addEventListener(\"click\",off),offbtn.addEventListener(\"touchend\",off),setbtn.addEventListener(\"click\",set),setbtn.addEventListener(\"touchend\",set),rb.addEventListener(\"click\",rbAction),rb.addEventListener(\"touchend\",rbAction),spkl.addEventListener(\"click\",spklAction),spkl.addEventListener(\"touchend\",spklAction),sr.addEventListener(\"click\",srAction),sr.addEventListener(\"touchend\",srAction),refreshbtn.addEventListener(\"click\",update),refreshbtn.addEventListener(\"touchend\",update);var msgTO</script>";

void handleRoot() {
  // TODO: I bet there's a better way to log (get full url or something)
  Serial.println(server.method() + " /");
  if (server.method() == HTTP_GET) {
    server.send(200, "text/html", index_html_head + String(currentColor.r) + "," + String(currentColor.g) + "," + String(currentColor.r) + index_html_tail);
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
      server.send(200, "application/json", message);

      state = CUSTOM_STATE;
      startTransition(newColor, duration);
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

  message += "\"success\":true";
  message += "}";
  server.send(200, "application/json", message);

  state = OFF_STATE;
  startTransition({0, 0, 0}, duration);
}

void handleStartAlarm() {
  Serial.println(server.method() + " /sunrise");
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

  message += "\"success\": true";
  message += "}";
  server.send(200, "application/json", message);

  startSunrise(duration);
}

void handleSparkle() {
  Serial.println(server.method() + " /sparkle");
  String message = "{";

  uint8_t numArgs = server.args();
  if (numArgs > 0) {
    for (uint8_t i = 0; i < numArgs; i++) {
      String argName = server.argName(i);
      if (argName == "plain") {
        // ignore TODO: find out why this exists
      } else {
        message += "\"error\":\"invalid argument name: '" + server.argName(i) + "'\"}";
        server.send(400, "text/plain", message);
        return;
      }
    }
  }

  message += "\"success\": true";
  message += "}";
  server.send(200, "application/json", message);

  startSparkling();
}

void handleRainbow() {
  Serial.println(server.method() + " /rainbow");
  String message = "{";
  unsigned long speed = 1000 * 5;

  uint8_t numArgs = server.args();
  if (numArgs > 0) {
    for (uint8_t i = 0; i < numArgs; i++) {
      String argName = server.argName(i);
      if (argName == "speed") {
        speed = server.arg(i).toInt();
      } else if (argName == "plain") {
        // ignore TODO: find out why this exists
      } else {
        message += "\"error\":\"invalid argument name: '" + server.argName(i) + "'\"}";
        server.send(400, "text/plain", message);
        return;
      }
    }
  }

  message += "\"success\": true";
  message += "}";
  server.send(200, "application/json", message);

  startRainbow(speed);
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
