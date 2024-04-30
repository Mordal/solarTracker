



void readLichtSensors(){
  lichtSensor_LB = analogRead(PIN_lichtSensor_LB);
  lichtSensor_RB = analogRead(PIN_lichtSensor_RB);
  lichtSensor_LO = analogRead(PIN_lichtSensor_LO);
  lichtSensor_RO = analogRead(PIN_lichtSensor_RO);
  myObject["lichtSensors"]["Links_Boven"] = lichtSensor_LB;
  myObject["lichtSensors"]["Rechts_Boven"] = lichtSensor_RB;
  myObject["lichtSensors"]["Links_Onder"] = lichtSensor_LO;
  myObject["lichtSensors"]["Rechts_Onder"] = lichtSensor_RO;
}

//onder de 300 is donker (??)
bool isNight(){
  if(lichtSensor_LB < 300 && lichtSensor_RB < 300 && lichtSensor_LO < 300 && lichtSensor_RO < 300){
    myObject["lichtSensors"]["isNight"] = true;
    return true;
  }
  myObject["lichtSensors"]["isNight"] = false;
  return false;
}