

void readLichtSensors() {
  lichtSensor_LB = analogRead(PIN_lichtSensor_LB) + lichtSensor_LB_offset;
  lichtSensor_RB = analogRead(PIN_lichtSensor_RB) + lichtSensor_RB_offset;
  lichtSensor_LO = analogRead(PIN_lichtSensor_LO) + lichtSensor_LO_offset;
  lichtSensor_RO = analogRead(PIN_lichtSensor_RO) + lichtSensor_RO_offset;
  lichtSensors_Links = lichtSensor_LB + lichtSensor_LO;
  lichtSensors_Rechts = lichtSensor_RB + lichtSensor_RO;
  lichtSensors_Boven = lichtSensor_LB + lichtSensor_RB;
  lichtSensors_Onder = lichtSensor_LO + lichtSensor_RO;
}

//onder de 300 is donker (??)
bool isNight() {
  byte currentHour = getHourNumber();
  if (isSummerTimeRTC()) {
    currentHour--;
  }
  return (currentHour >= 21 || currentHour < 6);
}