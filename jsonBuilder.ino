JSONVar addAllFlagsToObject(JSONVar fullObject = undefined){
  fullObject["Flags"]["TEST_MODE"] = TEST_MODE;
  fullObject["Flags"]["SAFE_MODE"] = SAFE_MODE;
  fullObject["Flags"]["Draaien_too_long"] = draaienTooLong;
  fullObject["Flags"]["Kantelen_too_long"] = kantelenTooLong;
  fullObject["Flags"]["Wifi_connected"] = wifiConnected;
  fullObject["Flags"]["Mqtt_connected"] = mqttConnected;
  return fullObject;
}

JSONVar addAllForcedMovementsToObject(JSONVar fullObject = undefined){
  fullObject["ForceMovement"]["Force_LEFT"] = linksDraaien_FORCE;
  fullObject["ForceMovement"]["Force_RIGHT"] = rechtsDraaien_FORCE;
  fullObject["ForceMovement"]["Force_OUT"] = uitschuiven_FORCE;
  fullObject["ForceMovement"]["Force_IN"] = inschuiven_FORCE;
  return fullObject;
}

JSONVar addTimeOutsToObject(JSONVar fullObject = undefined){
  fullObject["TimeOuts"]["antiPendelTime"] = antiPendelTime;
  fullObject["TimeOuts"]["maxMovementTime"] = maxMovementTime;
  fullObject["TimeOuts"]["logBook_Timer_delay"] = logBook_Timer_delay;
  fullObject["TimeOuts"]["retryTime"] = retryTime;
  return fullObject;
}

JSONVar addRemainingTimeToObject(JSONVar fullObject = undefined){
  fullObject["TimeRemaining"]["antiPendel_Draaien_Timer"] = antiPendel_Draaien_Timer_Remaining;
  fullObject["TimeRemaining"]["antiPendel_Kantelen_Timer"] = antiPendel_Kantelen_Timer_Remaining;
  fullObject["TimeRemaining"]["draaien_TimeOut"] = draaien_TimeOut_Remaining;
  fullObject["TimeRemaining"]["kantelen_TimeOut"] = kantelen_TimeOut_Remaining;
  fullObject["TimeRemaining"]["logBook_Timer"] = logBook_Timer_Remaining;
  fullObject["TimeRemaining"]["retryTimer"] = retryTimer_Remaining;
  return fullObject;
}

JSONVar addOtherDataToObject(JSONVar fullObject = undefined){
  fullObject["Reset_Alarms"] = false;
  return fullObject;
}

JSONVar getAllData(){
  JSONVar fullObject = myObject;
  fullObject = addAllFlagsToObject(fullObject);
  fullObject = addAllForcedMovementsToObject(fullObject);
  fullObject = addTimeOutsToObject(fullObject);
  fullObject = addRemainingTimeToObject(fullObject);
  fullObject = addOtherDataToObject(fullObject);
  return fullObject;
}
