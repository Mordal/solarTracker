
JSONVar getFlags(){
  JSONVar flagsObject = undefined;
  flagsObject["Flags"]["TEST_MODE"] = TEST_MODE;
  flagsObject["Flags"]["SAFE_MODE"] = SAFE_MODE;
  flagsObject["Flags"]["STOP"] = STOP;
  flagsObject["Flags"]["isNight"] = isNight();
  flagsObject["Flags"]["Draaien_too_long"] = draaienTooLong;
  flagsObject["Flags"]["Kantelen_too_long"] = kantelenTooLong;
  flagsObject["Flags"]["Wifi_connected"] = wifiConnected;
  flagsObject["Flags"]["Mqtt_connected"] = mqttConnected;
  return flagsObject;
}

JSON getLightSensorData(){
  JSONVar sensorDataObject = undefined;
  sensorDataObject["Sensors"]["lichtSensor_LB"] = lichtSensor_LB;
  sensorDataObject["Sensors"]["lichtSensor_LO"] = lichtSensor_LO;
  sensorDataObject["Sensors"]["lichtSensor_RB"] = lichtSensor_RB;
  sensorDataObject["Sensors"]["lichtSensor_RO"] = lichtSensor_RO;
  sensorDataObject["Sensors"]["links"] = lichtSensors_Links;
  sensorDataObject["Sensors"]["rechts"] = lichtSensors_Rechts;
  sensorDataObject["Sensors"]["boven"] = lichtSensors_Boven;
  sensorDataObject["Sensors"]["onder"] = lichtSensors_Onder;  
  sensorDataObject["Sensors"]["isNight"] = isNight();
  return sensorDataObject;
}

JSONVar getTurnMovementData(){
  JSONVar movementDataObject;
  movementDataObject["turn"]["percentage"] = (float)currentTurnPercentage/10000.0;
  movementDataObject["turn"]["antiPendel"] = antiPendel_Draaien;
  
  movementDataObject["Left"]["sensors"] = linksDraaien_Sensors;
  movementDataObject["Left"]["moving"] = linksDraaien;
  movementDataObject["Left"]["eindeloop"] = einde_Linksdraaien;

  movementDataObject["Right"]["sensors"] = rechtsDraaien_Sensors;
  movementDataObject["Right"]["moving"] = rechtsDraaien;
  movementDataObject["Right"]["eindeloop"] = einde_Rechtsdraaien;

  return movementDataObject;
}

JSONVar getTiltMovementData(){
  JSONVar movementDataObject = undefined;

  movementDataObject["tilt"]["percentage"] = (float)currentTiltPercentage/10000.0;
  movementDataObject["tilt"]["antiPendel"] = antiPendel_Kantelen;
  
  movementDataObject["Extend"]["sensors"] = uitschuiven_Sensors;
  movementDataObject["Extend"]["moving"] = uitschuiven;
  movementDataObject["Extend"]["eindeloop"] = einde_Uitschuiven;

  movementDataObject["Retract"]["sensors"] = inschuiven_Sensors;
  movementDataObject["Retract"]["moving"] = inschuiven;
  movementDataObject["Retract"]["eindeloop"] = einde_Inschuiven;

  return movementDataObject;
}

JSONVar getOtherData(){
  JSONVar otherDataObject = undefined;
  otherDataObject["Reset_Alarms"] = false;
  RTCTime currentTime = getRTCTime();
  otherDataObject["Time"] = currentTime.toString();
  otherDataObject["Turn_Time"] = timeNeededToTurn;
  otherDataObject["Tilt_Time"] = timeNeededToTilt;
  return otherDataObject;
}

JSONVar getAllForcedMovements(){
  JSONVar forceMovementObject = undefined;
  forceMovementObject["ForceMovement"]["LEFT_Force"] = linksDraaien_FORCE;
  forceMovementObject["ForceMovement"]["RIGHT_Force"] = rechtsDraaien_FORCE;
  forceMovementObject["ForceMovement"]["OUT_Force"] = uitschuiven_FORCE;
  forceMovementObject["ForceMovement"]["IN_Force"] = inschuiven_FORCE;
  return forceMovementObject;
}


JSONVar getSettings(){
  JSONVar settingsObject = undefined;
  settingsObject["sensorOffsets"]["LB"] = lichtSensor_LB_offset;
  settingsObject["sensorOffsets"]["RB"] = lichtSensor_RB_offset;
  settingsObject["sensorOffsets"]["LO"] = lichtSensor_LO_offset;
  settingsObject["sensorOffsets"]["RO"] = lichtSensor_RO_offset;
  settingsObject["TimeOuts"]["licht_marge"] = licht_marge;
  settingsObject["TimeOuts"]["antiPendelTime"] = antiPendelTime;
  settingsObject["TimeOuts"]["maxMovementTime"] = maxMovementTime;
  settingsObject["TimeOuts"]["retryTime"] = retryTime;
  settingsObject["TimeOuts"]["logBook_Timer_delay"] = logBook_Timer_delay;
  return settingsObject;
}

JSONVar getRemainingTime(){
  JSONVar remainingTimeObject = undefined;
  remainingTimeObject["TimeRemaining"]["antiPendel_Draaien_Timer"] = antiPendel_Draaien_Timer_Remaining;
  remainingTimeObject["TimeRemaining"]["antiPendel_Kantelen_Timer"] = antiPendel_Kantelen_Timer_Remaining;
  remainingTimeObject["TimeRemaining"]["draaien_TimeOut"] = draaien_TimeOut_Remaining;
  remainingTimeObject["TimeRemaining"]["kantelen_TimeOut"] = kantelen_TimeOut_Remaining;
  remainingTimeObject["TimeRemaining"]["logBook_Timer"] = logBook_Timer_Remaining;
  remainingTimeObject["TimeRemaining"]["retryTimer"] = retryTimer_Remaining;
  return remainingTimeObject;
}



void printAllProperties(JSONVar object, WiFiClient client){
  JSONVar keys = object.keys();
  for (int i = 0; i < keys.length(); i++) {
    client.print(keys[i]);
    client.print(": ");
    client.print(object[keys[i]]);
    if(i < keys.length()-1){
      client.print(", ");
    }
  }
}


void clientPrintAllData(WiFiClient client){
  client.print("{");

  printAllProperties(getAllFlags(), client);
  client.print(", ");
  printAllProperties(getAllForcedMovements(), client);
  client.print(", ");
  printAllProperties(getTimeOuts(), client);
  client.print(", ");
  printAllProperties(getRemainingTime(), client);
  client.print(", ");
  printAllProperties(getOtherData(), client);
  client.print("}"); 
}
