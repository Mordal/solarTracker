JSONVar getAllFlags(){
  JSONVar flagsObject = undefined;
  flagsObject["Flags"]["TEST_MODE"] = TEST_MODE;
  flagsObject["Flags"]["SAFE_MODE"] = SAFE_MODE;
  flagsObject["Flags"]["Draaien_too_long"] = draaienTooLong;
  flagsObject["Flags"]["Kantelen_too_long"] = kantelenTooLong;
  flagsObject["Flags"]["Wifi_connected"] = wifiConnected;
  flagsObject["Flags"]["Mqtt_connected"] = mqttConnected;
  return flagsObject;
}

JSONVar getAllForcedMovements(){
  JSONVar forceMovementObject = undefined;
  forceMovementObject["ForceMovement"]["LEFT_Force"] = linksDraaien_FORCE;
  forceMovementObject["ForceMovement"]["Force_RIGHT"] = rechtsDraaien_FORCE;
  forceMovementObject["ForceMovement"]["Force_OUT"] = uitschuiven_FORCE;
  forceMovementObject["ForceMovement"]["Force_IN"] = inschuiven_FORCE;
  return forceMovementObject;
}

JSON getSensorData(){
  JSONVar sensorDataObject = undefined;
  sensorDataObject["Sensors"]["lichtSensor_LB"] = lichtSensor_LB;
  sensorDataObject["Sensors"]["lichtSensor_LO"] = lichtSensor_LO;
  sensorDataObject["Sensors"]["lichtSensor_RB"] = lichtSensor_RB;
  sensorDataObject["Sensors"]["lichtSensor_RO"] = lichtSensor_RO;
  sensorDataObject["EindeLoop"]["links"] = einde_Linksdraaien;
  sensorDataObject["EindeLoop"]["rechts"] = einde_Rechtsdraaien;
  sensorDataObject["EindeLoop"]["uitschuiven"] = einde_Uitschuiven;
  sensorDataObject["EindeLoop"]["inschuiven"] = einde_Inschuiven;
  return sensorDataObject;
}


JSONVar getTimeOuts(){
  JSONVar timeOutsObject = undefined;
  timeOutsObject["TimeOuts"]["antiPendelTime"] = antiPendelTime;
  timeOutsObject["TimeOuts"]["maxMovementTime"] = maxMovementTime;
  timeOutsObject["TimeOuts"]["logBook_Timer_delay"] = logBook_Timer_delay;
  timeOutsObject["TimeOuts"]["retryTime"] = retryTime;
  return timeOutsObject;
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

JSONVar getMovementData(){
  JSONVar movementDataObject = undefined;
  movementDataObject["Turn_Percentage"] = (float)currentTurnPercentage/10000.0;
  movementDataObject["Tilt_Percentage"] = (float)currentTiltPercentage/10000.0;
  movementDataObject["Left"]["sensors"] = linksDraaien_Sensors;
  movementDataObject["Left"]["moving"] = linksDraaien;
  movementDataObject["Right"]["sensors"] = rechtsDraaien_Sensors;
  movementDataObject["Right"]["moving"] = rechtsDraaien;
  movementDataObject["Extend"]["sensors"] = uitschuiven_Sensors;
  movementDataObject["Extend"]["moving"] = uitschuiven;
  movementDataObject["Retract"]["sensors"] = inschuiven_Sensors;
  movementDataObject["Retract"]["moving"] = inschuiven;

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
  printAllProperties(myObject, client);
  client.print(", ");
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
