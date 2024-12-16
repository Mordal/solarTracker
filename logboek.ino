bool setLogbook(void *){
  print("Sending messages to MQTT...");
  const long timeStamp = getEpochTime();

  JSONVar message = getFlags();
  message["TimeStamp"] = timeStamp;
  mqttClient.beginMessage("flags");
  mqttClient.print(message);
  mqttClient.endMessage();

  message = getLightSensorData();
  message["TimeStamp"] = timeStamp;
  mqttClient.beginMessage("sensorData");
  mqttClient.print(message);
  mqttClient.endMessage();

  message = getTurnMovementData();
  message["TimeStamp"] = timeStamp;
  mqttClient.beginMessage("turnMovementData");
  mqttClient.print(message);
  mqttClient.endMessage();

  message = getTiltMovementData();
  message["TimeStamp"] = timeStamp;
  mqttClient.beginMessage("tiltMovementData");
  mqttClient.print(message);
  mqttClient.endMessage();
  

  message = getOtherData();
  message["TimeStamp"] = timeStamp;
  mqttClient.beginMessage("otherData");
  mqttClient.print(message);
  mqttClient.endMessage();

  return true;
}



