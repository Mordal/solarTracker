// MOSQUITTO_SUB code:
// ALL TOPICS: mosquitto_sub -v -h 172.17.0.3 -p 1883 -t '#'
// TOPIC combinedData: mosquitto_sub -v -h 172.17.0.2 -p 1883 -t 'combinedData'



bool setLogbook(void*) {
   // loop through char mqtt_Data[][10]
   for (int i = 0; i < sizeof(mqtt_Logbook) / sizeof(mqtt_Logbook[0]); i++) {
      char* topic = mqtt_Logbook[i];
      mqtt_sendData(topic);
   }
   return true;
}

bool sendAllPageData(void*) {
   if (clientConnectedTimer.empty()) {
      return false;
   }

   // loop through char mqtt_Data[][10]
   for (int i = 0; i < sizeof(mqtt_allData) / sizeof(mqtt_allData[0]); i++) {
      char* topic = mqtt_allData[i];
      mqtt_sendData(topic);
   }
   return true;
}


void mqtt_sendData(char* topic) {
   if (strcmp(topic, "flags") == 0) {
      mqtt_getFlags();
   }
   else if (strcmp(topic, "sensors") == 0) {
      mqtt_getLightSensorData();
   }
   else if (strcmp(topic, "tilt") == 0) {
      mqtt_getTiltMovementData();
   }
   else if (strcmp(topic, "turn") == 0) {
      mqtt_getTurnMovementData();
   }
   else if (strcmp(topic, "force") == 0) {
      mqtt_getForceMovements();
   }
   else if (strcmp(topic, "timeRemaining") == 0) {
      mqtt_getRemainingTime();
   }
   else if (strcmp(topic, "other") == 0) {
      mqtt_getOtherData();
   }
   else if (strcmp(topic, "wifi") == 0) {
      mqtt_getWifiData();
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

void mqtt_getWifiData() {
   const long timeStamp = getEpochTime();
   JSONVar message = getWifiData();
   message["TimeStamp"] = timeStamp;
   mqttClient.beginMessage("wifiData");
   mqttClient.print(message);
   mqttClient.endMessage();
}