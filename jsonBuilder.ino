JSONVar getAllFlags(){
  JSONVar fullObject = undefined;
  fullObject["Flags"]["TEST_MODE"] = TEST_MODE;
  fullObject["Flags"]["SAFE_MODE"] = SAFE_MODE;
  fullObject["Flags"]["Draaien_too_long"] = draaienTooLong;
  fullObject["Flags"]["Kantelen_too_long"] = kantelenTooLong;
  fullObject["Flags"]["Wifi_connected"] = wifiConnected;
  fullObject["Flags"]["Mqtt_connected"] = mqttConnected;
  return fullObject;
}

JSONVar getAllForcedMovements(){
  JSONVar fullObject = undefined;
  fullObject["ForceMovement"]["LEFT_Force"] = linksDraaien_FORCE;
  fullObject["ForceMovement"]["Force_RIGHT"] = rechtsDraaien_FORCE;
  fullObject["ForceMovement"]["Force_OUT"] = uitschuiven_FORCE;
  fullObject["ForceMovement"]["Force_IN"] = inschuiven_FORCE;
  return fullObject;
}

JSONVar getTimeOuts(){
  JSONVar fullObject = undefined;
  fullObject["TimeOuts"]["antiPendelTime"] = antiPendelTime;
  fullObject["TimeOuts"]["maxMovementTime"] = maxMovementTime;
  fullObject["TimeOuts"]["logBook_Timer_delay"] = logBook_Timer_delay;
  fullObject["TimeOuts"]["retryTime"] = retryTime;
  return fullObject;
}

JSONVar getRemainingTime(){
  JSONVar fullObject = undefined;
  fullObject["TimeRemaining"]["antiPendel_Draaien_Timer"] = antiPendel_Draaien_Timer_Remaining;
  fullObject["TimeRemaining"]["antiPendel_Kantelen_Timer"] = antiPendel_Kantelen_Timer_Remaining;
  fullObject["TimeRemaining"]["draaien_TimeOut"] = draaien_TimeOut_Remaining;
  fullObject["TimeRemaining"]["kantelen_TimeOut"] = kantelen_TimeOut_Remaining;
  fullObject["TimeRemaining"]["logBook_Timer"] = logBook_Timer_Remaining;
  fullObject["TimeRemaining"]["retryTimer"] = retryTimer_Remaining;
  return fullObject;
}

JSONVar getOtherData(){
  JSONVar fullObject = undefined;
  fullObject["Reset_Alarms"] = false;
  RTCTime currentTime = getRTCTime();
  fullObject["Time"] = currentTime.toString();
  fullObject["Turn_Time"] = timeNeededToTurn;
  fullObject["Tilt_Time"] = timeNeededToTilt;
  fullObject["Turn_Percentage"] = (float)currentTurnPercentage/10000.0;
  fullObject["Tilt_Percentage"] = (float)currentTiltPercentage/10000.0;
  return fullObject;
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
