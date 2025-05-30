// MOSQUITTO_SUB code:
// ALL TOPICS: mosquitto_sub -v -h 172.17.0.3 -p 1883 -t '#'
// TOPIC combinedData: mosquitto_sub -v -h 172.17.0.2 -p 1883 -t 'combinedData'

bool setLogbook(void*) {
   // print("Sending messages to MQTT...");

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

bool sendAllPageData(void*) {
   if (clientConnectedTimer.empty()) {
      return false;
   }

   const long timeStamp = getEpochTime();
   // int timeStart = millis();
   // print("Sending messages to MQTT...");

   JSONVar message = getFlags();
   message["TimeStamp"] = timeStamp;
   mqttClient.beginMessage("flags");
   mqttClient.print(message);
   mqttClient.endMessage();

   message = getTiltMovementData();
   message["TimeStamp"] = timeStamp;
   mqttClient.beginMessage("tiltMovementData");
   mqttClient.print(message);
   mqttClient.endMessage();

   message = getTurnMovementData();
   message["TimeStamp"] = timeStamp;
   mqttClient.beginMessage("turnMovementData");
   mqttClient.print(message);
   mqttClient.endMessage();

   message = getForcedMovements();
   message["TimeStamp"] = timeStamp;
   mqttClient.beginMessage("forceMovement");
   mqttClient.print(message);
   mqttClient.endMessage();

   message = getRemainingTime();
   message["TimeStamp"] = timeStamp;
   mqttClient.beginMessage("timeRemaining");
   mqttClient.print(message);
   mqttClient.endMessage();

   message = getLightSensorData();
   message["TimeStamp"] = timeStamp;
   mqttClient.beginMessage("sensorData");
   mqttClient.print(message);
   mqttClient.endMessage();

   return true;
}
