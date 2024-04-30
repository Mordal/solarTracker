bool setLogbook(void *){
  print("Sending messages to MQTT...");

  JSONVar message = myObject["lichtSensors"];
  mqttClient.beginMessage("lichtSensors");
  mqttClient.print(message);
  mqttClient.endMessage();

  message = myObject["TURN_LEFT"];
  mqttClient.beginMessage("turnLeft");
  mqttClient.print(message);
  mqttClient.endMessage();

  message = myObject["TURN_RIGHT"];
  mqttClient.beginMessage("turnRight");
  mqttClient.print(message);
  mqttClient.endMessage();

  message = myObject["EXTEND"];
  mqttClient.beginMessage("extend");
  mqttClient.print(message);
  mqttClient.endMessage();

  message = myObject["RETRACT"];
  mqttClient.beginMessage("retract");
  mqttClient.print(message);
  mqttClient.endMessage();
  
  message = getFlags()
  mqttClient.beginMessage("flags");
  mqttClient.print(message);
  mqttClient.endMessage();

  return true;
}

  // bool wifiConnected = false;
  // bool mqttConnected = false;
  // bool TEST_MODE = false;
  // bool SAFE_MODE = false;
  // bool draaienTooLong = false;
  // bool kantelenTooLong = false;

JSONVar getFlags(){
  JSONVar flagObject;
  flagObject["wifiConnected"] = wifiConnected;
  flagObject["mqttConnected"] = mqttConnected;
  flagObject["TEST_MODE"] = TEST_MODE;
  flagObject["SAFE_MODE"] = SAFE_MODE;
  flagObject["draaienTooLong"] = draaienTooLong;
  flagObject["kantelenTooLong"] = kantelenTooLong;

  return flagObject;
}