bool setLogbook(void *){
  // print("Sending messages to MQTT...");
  const long timeStamp = getUnixTime();

  JSONVar message = getFlags();
  message["TimeStamp"] = timeStamp;
  mqttClient.beginMessage("flags");
  mqttClient.print(message);
  mqttClient.endMessage();

  message = getSensorData();
  message["TimeStamp"] = timeStamp;
  mqttClient.beginMessage("sensorData");
  mqttClient.print(message);
  mqttClient.endMessage();

  message = getMovementData();
  message["TimeStamp"] = timeStamp;
  mqttClient.beginMessage("movementData");
  mqttClient.print(message);
  mqttClient.endMessage();


  message = getOtherData();
  message["TimeStamp"] = timeStamp;
  mqttClient.beginMessage("otherData");
  mqttClient.print(message);
  mqttClient.endMessage();

  return true;
}



