void setTimeFromNet(){

// Start de NTP client
  timeClient.begin();
  // Update de tijd
  timeClient.update();

  // Haal de epoch tijd op
  unsigned long epochTime = timeClient.getEpochTime();
  
  // Bereken lokale tijd (met tijdzone en zomertijd)
  time_t localTime = calculateLocalTime(epochTime);
  
  // Stel de RTC tijd in
  setRTC(localTime);
}

void setRTC(time_t localTime) {
  RTCTime now;
  now.setUnixTime(localTime);
  RTC.setTime(now);
}

time_t calculateLocalTime(unsigned long epochTime) {
  // Zet de epoch tijd om naar de tijdstructuur -> hierdoor kan je 'year()', 'month()', 'day()', 'hour()', 'minute()' en 'second()' gebruiken
  setTime(epochTime);
  
  // Bepaal of het zomertijd is
  bool isDST = summertime(year(), month(), day(), hour(), 1);
  
  // Bereken de lokale tijd (CET is UTC+1, CEST is UTC+2)
  return isDST ? epochTime + 7200 : epochTime + 3600;
}

// Zomertijd berekenen (laatste zondag van maart tot laatste zondag van oktober)
bool summertime(int year, byte month, byte day, byte hour, byte tzHours) {
  if (month < 3 || month > 10) return false; // geen zomertijd in Jan, Feb, Nov, Dec
  if (month > 3 && month < 10) return true;  // zomertijd in Apr, May, Jun, Jul, Aug, Sep

  int lastSunday = (31 - ((5 * year / 4 + 4) % 7));  // Bereken laatste zondag van de maand
  
  if (month == 3) return (day > lastSunday || (day == lastSunday && hour >= 2));
  if (month == 10) return (day < lastSunday || (day == lastSunday && hour < 2));
  return false; // Deze regel zou nooit moeten worden bereikt
}


void printTime() {
  // Haal de RTC tijd op
  RTCTime currentTime = getRTCTime();
  String timeString = currentTime.toString();
  
  // Print de RTC tijd
  Serial.print(currentTime.getDayOfMonth());
  Serial.print("/");
  Serial.print(Month2int(currentTime.getMonth()));
  Serial.print("/");
  Serial.print(currentTime.getYear());
  Serial.print(" - ");
  Serial.print(currentTime.getHour());
  Serial.print(":");
  Serial.print(currentTime.getMinutes());
  Serial.print(":");
  Serial.println(currentTime.getSeconds());
}

unsigned long getEpochTime() {
  RTCTime RTCTime = getRTCTime();
  return RTCTime.getUnixTime();
}

RTCTime getRTCTime(){
  RTCTime currentTime;
  RTC.getTime(currentTime);
  return currentTime;
}

