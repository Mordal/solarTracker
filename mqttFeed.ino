// MOSQUITTO_SUB code:
// ALL TOPICS: mosquitto_sub -v -h 172.17.0.3 -p 1883 -t '#'
// TOPIC combinedData: mosquitto_sub -v -h 172.17.0.2 -p 1883 -t 'combinedData'

enum mqtt_data {
   flags,
   tilt,
   turn,
   force,
   timeRemaining,
   sensors,
   other
};


bool setLogbook(void*) {

   // loop through char mqtt_Data[][10]
   for (int i = 0; i < sizeof(mqtt_Logbook) / sizeof(mqtt_Logbook[0]); i++) {
      mqtt_data topic = mqtt_Logbook[i];
      mqtt_sendData(topic);
   }

   // mqtt_getFlags();
   // mqtt_getLightSensorData();
   // mqtt_getTurnMovementData();
   // mqtt_getTiltMovementData();
   // mqtt_getOtherData();
   return true;
}

bool sendAllPageData(void*) {
   if (clientConnectedTimer.empty()) {
      return false;
   }

   mqtt_getFlags();
   mqtt_getTiltMovementData();
   mqtt_getTurnMovementData();
   mqtt_getForceMovements();
   mqtt_getRemainingTime();
   mqtt_getLightSensorData();
   return true;
}

void mqtt_sendData(mqtt_data topic) {

   mqtt_data dataTopic = topic;
   switch (dataTopic) {
   case flags:
      mqtt_getFlags();
      break;
   case tilt:
      mqtt_getTiltMovementData();
      break;
   case turn:
      mqtt_getTurnMovementData();
      break;
   case force:
      mqtt_getForceMovements();
      break;
   case timeRemaining:
      mqtt_getRemainingTime();
      break;
   case sensors:
      mqtt_getLightSensorData();
      break;
   case other:
      mqtt_getOtherData();
      break;

   default:
      break;
   }

}


void mqtt_getFlags() {
   const long timeStamp = getEpochTime();
   JSONVar message = getFlags();
   message["TimeStamp"] = timeStamp;
   mqttClient.beginMessage("flags");
   mqttClient.print(message);
   mqttClient.endMessage();
}

void mqtt_getLightSensorData() {
   const long timeStamp = getEpochTime();
   JSONVar message = getLightSensorData();
   message["TimeStamp"] = timeStamp;
   mqttClient.beginMessage("sensorData");
   mqttClient.print(message);
   mqttClient.endMessage();
}

void mqtt_getTurnMovementData() {
   const long timeStamp = getEpochTime();
   JSONVar message = getTurnMovementData();
   message["TimeStamp"] = timeStamp;
   mqttClient.beginMessage("turnMovementData");
   mqttClient.print(message);
   mqttClient.endMessage();
}
void mqtt_getTiltMovementData() {
   const long timeStamp = getEpochTime();
   JSONVar message = getTiltMovementData();
   message["TimeStamp"] = timeStamp;
   mqttClient.beginMessage("tiltMovementData");
   mqttClient.print(message);
   mqttClient.endMessage();
}
void mqtt_getOtherData() {
   const long timeStamp = getEpochTime();
   JSONVar message = getOtherData();
   message["TimeStamp"] = timeStamp;
   mqttClient.beginMessage("otherData");
   mqttClient.print(message);
   mqttClient.endMessage();
}
void mqtt_getForceMovements() {
   const long timeStamp = getEpochTime();
   JSONVar message = getForcedMovements();
   message["TimeStamp"] = timeStamp;
   mqttClient.beginMessage("forceMovement");
   mqttClient.print(message);
   mqttClient.endMessage();
}

void mqtt_getRemainingTime() {
   const long timeStamp = getEpochTime();
   JSONVar message = getRemainingTime();
   message["TimeStamp"] = timeStamp;
   mqttClient.beginMessage("timeRemaining");
   mqttClient.print(message);
   mqttClient.endMessage();
}