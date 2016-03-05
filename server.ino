const String index_html_head = "<!DOCTYPE html><meta charset=utf-8><meta content=\"width=device-width,initial-scale=1,maximum-scale=1,user-scalable=0\"name=viewport><meta name=apple-mobile-web-app-capable content=yes><title>💡</title><style>*{box-sizing:border-box}body,html{overflow:hidden;height:100%;width:100%;position:fixed;-webkit-user-select:none;user-select:none;-webkit-tap-highlight-color:transparent}body{font-family:-apple-system,\"Helvetica Neue\",\"Lucida Grande\",sans-serif;color:#eee;margin:0;font-size:28px;background-color:#000;-webkit-user-select:none;user-select:none;-webkit-tap-highlight-color:transparent}p{margin:0}#msg{border-bottom-left-radius:2px;border-bottom-right-radius:2px;background-color:#ffcdcd;color:#9e0000;border:1px solid;border-top:none;padding:4px 8px;font-size:18px;position:absolute;top:0;left:15px;right:15px;transition:transform .4s 0s ease-in-out}#msg.hide{transform:translateY(-100%)}.c{position:relative;height:100%;display:flex;flex-direction:column;justify-content:space-between}.lb-c{width:100%;text-align:center}#lb{height:100px;width:100px;margin:0 auto;transition:background-color .5s ease-in-out;border-radius:4px;border:1px solid #eee;background-color:rgb(";
const String index_html_tail = ")}.ctl{display:flex;justify-content:space-around}.e{font-family:AppleColorEmoji,default;position:relative;font-size:16px;line-height:16px;width:16px;height:16px}.e>div{transform:scale(3);transform-origin:50% 50%;height:16px;padding-top:6px;padding-right:23px;width:16px;display:inline-block;vertical-align:middle;transition:text-shadow .4s .1s ease-in-out}.e.touching>div,.e>div.touching{transition:text-shadow 50ms ease-in-out;text-shadow:0 0 10px #fff}.input{height:48px;position:relative}#pv{background-color:#fff;position:relative}#pv,input[type=range]{-webkit-appearance:none;width:100%;height:100%}input[type=range]{background-color:transparent}input[type=range]:focus{outline:0}#pv,input[type=range]::-webkit-slider-runnable-track{height:48px;cursor:pointer;padding:6px 8px 6px 4px;display:block}input[type=range]::-webkit-slider-runnable-track{background-color:transparent}input[type=range]::-webkit-slider-thumb{height:32px;width:32px;border-radius:16px;cursor:pointer;-webkit-appearance:none;background-image:linear-gradient(to bottom,rgba(255,255,255,.1) 0,rgba(0,0,0,.1) 100%)}#input-r{background-image:linear-gradient(to right,#0ff 0,#fff 100%)}#input-g{background-image:linear-gradient(to right,#f0f 0,#fff 100%)}#input-b{background-image:linear-gradient(to right,#ff0 0,#fff 100%)}input#red[type=range]::-webkit-slider-thumb{border:1px solid #a00;background-color:red}input#green[type=range]::-webkit-slider-thumb{border:1px solid #0a0;background-color:#0f0}input#blue[type=range]::-webkit-slider-thumb{border:1px solid #00a;background-color:#00f}</style><p id=msg class=hide><div class=c><div></div><div class=lb-c><div id=lb></div></div><div class=ctl><div id=sr class=e><div>&#127749;</div></div><div id=rb class=e><div>&#127752;</div></div><div id=spkl class=e><div>&#10024;</div></div></div><div class=ctl><div id=turnOff class=e><div>&#128244;</div></div><div id=refresh class=e><div>&#128260;</div></div></div><div class=m><div id=input-r class=input><input id=red value=255 type=range step=1 min=0 max=255></div><div id=input-g class=input><input id=green value=255 type=range step=1 min=0 max=255></div><div id=input-b class=input><input id=blue value=255 type=range step=1 min=0 max=255></div><div id=pv></div></div></div><script>function setpv(){pv.style.backgroundColor=\"rgb(\"+newColor[0]+\",\"+newColor[1]+\",\"+newColor[2]+\")\";var e=\"0,\"+newColor[1]+\",\"+newColor[2],t=\"255,\"+newColor[1]+\",\"+newColor[2];ri.style.backgroundImage=\"linear-gradient(to right,rgb(\"+e+\") 0%,rgb(\"+t+\") 100%)\";var n=newColor[0]+\",0,\"+newColor[2],o=newColor[0]+\",255,\"+newColor[2];gi.style.backgroundImage=\"linear-gradient(to right,rgb(\"+n+\") 0%,rgb(\"+o+\") 100%)\";var r=newColor[0]+\",\"+newColor[1]+\",0\",l=newColor[0]+\",\"+newColor[1]+\",255\";bi.style.backgroundImage=\"linear-gradient(to right,rgb(\"+r+\") 0%,rgb(\"+l+\") 100%)\"}function off(e){ajax(\"POST\",\"/off?duration=1000\",function(){lb.style.backgroundColor=\"rgb(0, 0, 0)\"}),e.preventDefault()}function set(e){ajax(\"POST\",\"/color?r=\"+newColor[0]+\"&g=\"+newColor[1]+\"&b=\"+newColor[2]+\"&duration=1000\",function(){lb.style.backgroundColor=\"rgb(\"+newColor[0]+\", \"+newColor[1]+\", \"+newColor[2]+\")\"}),e.preventDefault()}function rbAction(e){ajax(\"POST\",\"/rainbow\"),e.preventDefault()}function spklAction(e){ajax(\"POST\",\"/sparkle\"),e.preventDefault()}function srAction(e){ajax(\"POST\",\"/sunrise\"),e.preventDefault()}function update(e){ajax(\"GET\",\"/status\",function(e){var t=JSON.parse(e);lb.style.backgroundColor=\"rgb(\"+t.color.r+\",\"+t.color.g+\",\"+t.color.b+\")\"}),e&&e.preventDefault()}function ajax(e,t,n,o){n=n||function(){},o=o||function(){};var r=new XMLHttpRequest;r.onreadystatechange=function(){r.readyState==XMLHttpRequest.DONE&&(200==r.status?(n(r.responseText),msg.classList.add(\"hide\")):(o(r),clearTimeout(msgTO),msg.textContent=\"Error: \"+(r.response||\"unknown error\"),msg.classList.remove(\"hide\"),msgTO=setTimeout(function(){msg.classList.add(\"hide\")},2e3)))},r.open(e,t,!0),r.send()}var lb=document.getElementById(\"lb\"),msg=document.getElementById(\"msg\"),pv=document.getElementById(\"pv\"),rs=document.getElementById(\"red\"),bs=document.getElementById(\"blue\"),gs=document.getElementById(\"green\"),ri=document.getElementById(\"input-r\"),bi=document.getElementById(\"input-b\"),gi=document.getElementById(\"input-g\"),rb=document.getElementById(\"rb\"),spkl=document.getElementById(\"spkl\"),sr=document.getElementById(\"sr\"),offbtn=document.getElementById(\"turnOff\"),refreshbtn=document.getElementById(\"refresh\");document.ontouchstart=function(e){e.preventDefault()},document.ontouchmove=function(e){e.preventDefault()};var newColor=[255,255,255];rs.addEventListener(\"input\",function(e){newColor[0]=e.target.value,setpv()}),gs.addEventListener(\"input\",function(e){newColor[1]=e.target.value,setpv()}),bs.addEventListener(\"input\",function(e){newColor[2]=e.target.value,setpv()}),[].forEach.call(document.querySelectorAll(\".e\"),function(e){console.log(e),e.addEventListener(\"touchstart\",function(e){e.target.classList.add(\"touching\")}),e.addEventListener(\"touchend\",function(e){e.target.classList.remove(\"touching\")})}),offbtn.addEventListener(\"click\",off),offbtn.addEventListener(\"touchend\",off),pv.addEventListener(\"click\",set),pv.addEventListener(\"touchend\",set),rb.addEventListener(\"click\",rbAction),rb.addEventListener(\"touchend\",rbAction),spkl.addEventListener(\"click\",spklAction),spkl.addEventListener(\"touchend\",spklAction),sr.addEventListener(\"click\",srAction),sr.addEventListener(\"touchend\",srAction),refreshbtn.addEventListener(\"click\",update),refreshbtn.addEventListener(\"touchend\",update);var msgTO</script>";

void logServerInfo() {
  Serial.println(server.method() + " " + server.uri());
}

void handleRoot() {
  logServerInfo();
  if (server.method() == HTTP_GET) {
    server.send(200, "text/html", index_html_head + String(currentColor.r) + "," + String(currentColor.g) + "," + String(currentColor.r) + index_html_tail);
  } else {
    server.send(405, "text/plain", "{\"error\":\"method not allowed\"}");
  }
}

void handleStatus() {
  logServerInfo();
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
  logServerInfo();
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
  logServerInfo();
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
  logServerInfo();
  String message = "{";
  unsigned long duration = 1000 * 60 * 3;

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
  logServerInfo();
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
  logServerInfo();
  String message = "{";
  unsigned long speed = 1000 * 6;

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
  logServerInfo();
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
