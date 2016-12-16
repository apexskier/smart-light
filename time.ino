bool waitingForTime = false;
bool nextUDPTest = 0;
int numUDPTests = 0;
int backoff = 1000;
bool alarmsSet;
const int TIMEZONE_OFFSET = -7;  // Pacific Daylight Time (USA)
const double LATITUDE = 47;
const bool DAYLIGHT_SAVINGS = true;

const unsigned long SUMMER_SOLSTICE = 14893200; // 172 * 24 * 60 * 60; // jun 21th
const unsigned long WINTER_SOLSTICE = 30672000; // 355 * 24 * 60 * 60; // dec 21
const unsigned long SECONDS_IN_YEAR = 31557600; // astronomical

/**
 * Returns a float indicating how long the day is relative to 12h/12h
 * -1 == shortest day
 * 0 == even day
 * 1 == longest day
 */
double daylightOffset(unsigned long currentTime) {
  return cos((currentTime - (SECONDS_IN_YEAR / 2) - WINTER_SOLSTICE) / (SECONDS_IN_YEAR / (2 * PI)));
}

void testDaylightOffset() {
  assert(daylightOffset(SUMMER_SOLSTICE) == 1, "summer solstice not 1");
  assert(daylightOffset(WINTER_SOLSTICE) == -1, "winter solstice not -1");
  assert(daylightOffset((SUMMER_SOLSTICE + WINTER_SOLSTICE) / 2) == 0, "midpoint not 0");
}

double dayLength(unsigned long currentTime) {
  double h = acos(-tan(LATITUDE / 90) * tan(daylightOffset(currentTime)));
  return h * (24 / PI);
}

/**
 * Returns a float indicating how redshifted the current color is.
 * 0 = none (white)
 * 1 = full (red)
 */
double lightRedShift(unsigned long currentTime) {
  double dayLen = dayLength(currentTime);
  if (dayLen == 0) {
    return 1;
  }
  double result = abs((hour(currentTime) - 12) / (dayLen / 2));
  return constrain(result, 0.0, 1.0);
}

bool isDaylightSavingsTime() {
    return DAYLIGHT_SAVINGS;
}

void testLightRedShift() {
  assert(lightRedShift(1460790000) == 1, "midnight redshift all the way on"); // midnight
  assert(lightRedShift(1460833200) == 0, "midday redshift off"); // midday
  assert(lightRedShift(1460847600) == 0.5713196826334157, "4pm partial redshift"); // 4pm
}

void setupTime() {
  syncTimeStart();
}

void syncTimeStart() {
  IPAddress timeServerIP; // time.nist.gov NTP server address
  // get a random server from the pool
  WiFi.hostByName("time.nist.gov", timeServerIP);

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  waitingForTime = true;
  numUDPTests = 0;
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}

void timeLoopCall() {
  // wait to see if a reply is available
  if (waitingForTime && current_millis > nextUDPTest) {
    if (numUDPTests > 16) {
      numUDPTests = 0;
      syncTimeStart();
      return;
    }
    numUDPTests++;
    nextUDPTest = current_millis + backoff;
    backoff *= 2;

    int cb = udp.parsePacket();
    if (!cb) {
      Serial.println("no packet yet");
    } else {
      Serial.print("packet received, length=");
      Serial.println(cb);
      // We've received a packet, read the data from it
      udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

      //the timestamp starts at byte 40 of the received packet and is four bytes,
      // or two words, long. First, esxtract the two words:

      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
      // combine the four bytes (two words) into a long integer
      // this is NTP time (seconds since Jan 1 1900):
      unsigned long secsSince1900 = highWord << 16 | lowWord;
      Serial.print("Seconds since Jan 1 1900 = " );
      Serial.println(secsSince1900);

      // now convert NTP time into everyday time:
      Serial.print("Unix time = ");
      // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
      const unsigned long seventyYears = 2208988800UL;
      // subtract seventy years:
      unsigned long epoch = secsSince1900 - seventyYears;
      Serial.println(epoch);

      waitingForTime = false;
      setTime(epoch);
      adjustTime(TIMEZONE_OFFSET * SECS_PER_HOUR);
      if (isDaylightSavingsTime()) adjustTime(-SECS_PER_HOUR);
      digitalClockDisplay();

      if (!alarmsSet) {
        if (Alarm.timerRepeat(1000 * 60 * 60, syncTimeStart) == dtINVALID_ALARM_ID) {
          Serial.println("Failed to create NTP repeat");
        } else {
          Serial.println("Set up NTP repeat");
        }

        // alarms
        if (Alarm.alarmRepeat(dowMonday, 5, 30, 0, wakeupAlarm) == dtINVALID_ALARM_ID) {
          Serial.println("Failed to create Monday alarm");
        } else {
          Serial.println("Created Monday alarm");
        }
        if (Alarm.alarmRepeat(dowTuesday, 5, 30, 0, wakeupAlarm) == dtINVALID_ALARM_ID) {
          Serial.println("Failed to create Tuesday alarm");
        } else {
          Serial.println("Created Tuesday alarm");
        }
        if (Alarm.alarmRepeat(dowWednesday, 5, 30, 0, wakeupAlarm) == dtINVALID_ALARM_ID) {
          Serial.println("Failed to create Wednesday alarm");
        } else {
          Serial.println("Created Wednesday alarm");
        }
        if (Alarm.alarmRepeat(dowThursday, 5, 30, 0, wakeupAlarm) == dtINVALID_ALARM_ID) {
          Serial.println("Failed to create Thursday alarm");
        } else {
          Serial.println("Created Thursday alarm");
        }
        if (Alarm.alarmRepeat(dowFriday, 5, 30, 0, wakeupAlarm) == dtINVALID_ALARM_ID) {
          Serial.println("Failed to create Friday alarm");
        } else {
          Serial.println("Created Friday alarm");
        }
        if (Alarm.alarmRepeat(dowSunday, 7, 30, 0, wakeupAlarm) == dtINVALID_ALARM_ID) {
          Serial.println("Failed to create Sunday alarm");
        } else {
          Serial.println("Created Sunday alarm");
        }

//        // test
//        if (Alarm.alarmRepeat(hour(), minute() + 1, 0, wakeupAlarm) == dtINVALID_ALARM_ID) {
//          Serial.print("Failed to create test alarm for");
//          Serial.println(String(hour()) + ": " + String(minute() + 1));
//        } else {
//          Serial.print("Created test alarm for");
//          Serial.println(String(hour()) + ": " + String(minute() + 1));
//        }

        alarmsSet = true;
      }
    }
  }
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address) {
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); // NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
