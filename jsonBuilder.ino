
JSONVar getFlags() {
   JSONVar flagsObject = undefined;
   flagsObject["Flags"]["TEST_MODE"] = TEST_MODE;
   flagsObject["Flags"]["SAFE_MODE"] = SAFE_MODE;
   flagsObject["Flags"]["STOP_MODE"] = STOP_MODE;
   flagsObject["Flags"]["isNight"] = isNight();
   flagsObject["Flags"]["Draaien_too_long"] = draaienTooLong;
   flagsObject["Flags"]["Kantelen_too_long"] = kantelenTooLong;
   flagsObject["Flags"]["Wifi_connected"] = wifiConnected;
   flagsObject["Flags"]["Mqtt_connected"] = mqttConnected;
   flagsObject["Flags"]["Reset"] = resetHappend;
   flagsObject["Flags"]["SettingsUnlocked"] = settingsUnlocked;

   return flagsObject;
}

JSONVar getLightSensorData() {
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

JSONVar getTurnMovementData() {
   JSONVar movementDataObject = undefined;
   movementDataObject["Turn"]["percentage"] = currentTurnPercentage;
   movementDataObject["Turn"]["antiPendel"] = antiPendel_Draaien;
   movementDataObject["Turn"]["goToPosition"] = gotoTurnPosition;

   movementDataObject["Left"]["sensors"] = linksDraaien_Sensors;
   movementDataObject["Left"]["moving"] = linksDraaien;
   movementDataObject["Left"]["eindeloop"] = einde_Linksdraaien;
   movementDataObject["Left"]["force"] = linksDraaien_FORCE;

   movementDataObject["Right"]["sensors"] = rechtsDraaien_Sensors;
   movementDataObject["Right"]["moving"] = rechtsDraaien;
   movementDataObject["Right"]["eindeloop"] = einde_Rechtsdraaien;
   movementDataObject["Right"]["force"] = rechtsDraaien_FORCE;

   return movementDataObject;
}

JSONVar getTiltMovementData() {
   JSONVar movementDataObject = undefined;
   movementDataObject["Tilt"]["percentage"] = currentTiltPercentage;
   movementDataObject["Tilt"]["antiPendel"] = antiPendel_Kantelen;
   movementDataObject["Tilt"]["goToPosition"] = gotoTiltPosition;

   movementDataObject["Extend"]["sensors"] = uitschuiven_Sensors;
   movementDataObject["Extend"]["moving"] = uitschuiven;
   movementDataObject["Extend"]["eindeloop"] = einde_Uitschuiven;
   movementDataObject["Extend"]["force"] = uitschuiven_FORCE;

   movementDataObject["Retract"]["sensors"] = inschuiven_Sensors;
   movementDataObject["Retract"]["moving"] = inschuiven;
   movementDataObject["Retract"]["eindeloop"] = einde_Inschuiven;
   movementDataObject["Retract"]["force"] = inschuiven_FORCE;

   return movementDataObject;
}

JSONVar getOtherData() {
   JSONVar otherDataObject = undefined;
   RTCTime currentTime = getRTCTime();
   otherDataObject["Time"] = currentTime.toString();
   otherDataObject["unixTime"] = getEpochTime();
   otherDataObject["Turn_Time"] = timeNeededToTurn;
   otherDataObject["Tilt_Time"] = timeNeededToTilt;
   otherDataObject["settingsUnlocked"] = settingsUnlocked;
   return otherDataObject;
}

JSONVar getForcedMovements() {
   JSONVar forceMovementObject = undefined;
   forceMovementObject["ForceMovement"]["LEFT_Force"] = linksDraaien_FORCE;
   forceMovementObject["ForceMovement"]["RIGHT_Force"] = rechtsDraaien_FORCE;
   forceMovementObject["ForceMovement"]["OUT_Force"] = uitschuiven_FORCE;
   forceMovementObject["ForceMovement"]["IN_Force"] = inschuiven_FORCE;
   return forceMovementObject;
}

JSONVar getTurnPresetPositions() {
   JSONVar presetPositionsObject = undefined;
   JSONVar turnArray = undefined;

   for (int i = 0; i < 14; i++) {
      turnArray[i] = turnPercentage_Presets[i];
   };
   presetPositionsObject["Turn"] = turnArray;
   turnArray = undefined;

   return presetPositionsObject;
}

JSONVar getTiltPresetPositions(byte monthIndex) {
   JSONVar presetPositionsObject = undefined;
   JSONVar tiltArray = undefined;

   for (int col = 0; col < 14; col++) {
      tiltArray[col] = tiltPercentage_Presets[monthIndex][col];
   };

   presetPositionsObject["Tilt"] = tiltArray;
   tiltArray = undefined;
   return presetPositionsObject;
}

JSONVar getSettings_Offsets() {
   JSONVar settingsObject = undefined;
   settingsObject["Offsets"]["LB"] = lichtSensor_LB_offset;
   settingsObject["Offsets"]["RB"] = lichtSensor_RB_offset;
   settingsObject["Offsets"]["LO"] = lichtSensor_LO_offset;
   settingsObject["Offsets"]["RO"] = lichtSensor_RO_offset;
   settingsObject["Sensors"]["licht_marge"] = licht_marge;
   return settingsObject;
}


JSONVar getSettings_TimeOuts() {
   JSONVar settingsObject = undefined;
   settingsObject["TimeOuts"]["APTime"] = antiPendelTime;
   settingsObject["TimeOuts"]["maxMoveTime"] = maxMovementTime;
   settingsObject["TimeOuts"]["periodicalTime"] = periodicalTime;
   settingsObject["TimeOuts"]["logbookTime"] = logbookTime;
   settingsObject["TimeOuts"]["clientConTimeOut"] = clientConnectedTimeOut;
   settingsObject["TimeOuts"]["sendAllDataTime"] = sendAllDataTime;
   settingsObject["TimeOuts"]["settingsUnlockedTime"] = settingsUnlockedTime;
   return settingsObject;
}

JSONVar getRemainingTime() {
   JSONVar remainingTimeObject = undefined;
   remainingTimeObject["TimeRem"]["draaien_AP"] =
      antiPendel_Draaien_Timer_Remaining;
   remainingTimeObject["TimeRem"]["kantelen_AP"] =
      antiPendel_Kantelen_Timer_Remaining;
   remainingTimeObject["TimeRem"]["draaien_TO"] = draaien_TimeOut_Remaining;
   remainingTimeObject["TimeRem"]["kantelen_TO"] = kantelen_TimeOut_Remaining;
   remainingTimeObject["TimeRem"]["logBookTimer"] = logBook_Timer_Remaining;
   remainingTimeObject["TimeRem"]["clientTimer"] =
      clientConnectedTimer_Remaining;
   remainingTimeObject["TimeRem"]["periodicalTimer"] = periodicalTimer_Remaining;
   remainingTimeObject["TimeRem"]["setUnlockTimer"] =
      settingsUnlockedTimer_Remaining;
   remainingTimeObject["TimeRem"]["gotoPosTimer"] =
      gotoPosition_Timer_Remaining;
   return remainingTimeObject;
}

//  IPAddress ip = WiFi.localIP();
//    Serial.print("IP Address: ");
//    Serial.println(ip);

//    // print the SSID of the network you're attached to:
//    Serial.print("Network name: ");
//    Serial.println(WiFi.SSID());

//    // print the received signal strength:
//    long rssi = WiFi.RSSI();
//    Serial.print("signal strength (RSSI):");

JSONVar getWifiData() {
   JSONVar wifiDataObject = undefined;
   wifiDataObject["Wifi"]["SSID"] = WiFi.SSID();
   wifiDataObject["Wifi"]["RSSI"] = WiFi.RSSI();
   wifiDataObject["Wifi"]["Status"] = WiFi.status();
   wifiDataObject["Wifi"]["Connected"] = wifiConnected;
   return wifiDataObject;
}

JSONVar getMqttSettings() {
   //fill mqttSettings with the topics
   JSONVar mqttSettings;
   for (int i = 0; i < 12; i++) {
      char* topic = mqtt_Logbook[i];
      mqttSettings["mqtt_Logbook"][i] = topic;
   }
   for (int i = 0; i < 12; i++) {
      char* topic = mqtt_allData[i];
      mqttSettings["mqtt_allData"][i] = topic;
   }
   return mqttSettings;
}


void getPageData() {
   return;   // DEPRECATED

   JSONVar pageData = undefined;
   pageData["Flags"]["TEST_MODE"] = TEST_MODE;
   pageData["Flags"]["SAFE_MODE"] = SAFE_MODE;
   pageData["Flags"]["STOP_MODE"] = STOP_MODE;
   pageData["Flags"]["isNight"] = isNight();
   pageData["Flags"]["Draaien_too_long"] = draaienTooLong;
   pageData["Flags"]["Kantelen_too_long"] = kantelenTooLong;
   pageData["Flags"]["Wifi_connected"] = wifiConnected;
   pageData["Flags"]["Mqtt_connected"] = mqttConnected;
   pageData["Flags"]["Reset"] = resetHappend;
   pageData["Flags"]["SettingsUnlocked"] = settingsUnlocked;

   // print(" - Flags added");

   pageData["Tilt"]["percentage"] = currentTiltPercentage;
   pageData["Tilt"]["antiPendel"] = antiPendel_Kantelen;
   pageData["Tilt"]["goToPosition"] = gotoTiltPosition;

   pageData["Extend"]["sensors"] = uitschuiven_Sensors;
   pageData["Extend"]["moving"] = uitschuiven;
   pageData["Extend"]["eindeloop"] = einde_Uitschuiven;
   pageData["Extend"]["force"] = uitschuiven_FORCE;

   pageData["Retract"]["sensors"] = inschuiven_Sensors;
   pageData["Retract"]["moving"] = inschuiven;
   pageData["Retract"]["eindeloop"] = einde_Inschuiven;
   pageData["Retract"]["force"] = inschuiven_FORCE;

   // print(" - Tilt added");

   pageData["Turn"]["percentage"] = currentTurnPercentage;
   pageData["Turn"]["antiPendel"] = antiPendel_Draaien;
   pageData["Turn"]["goToPosition"] = gotoTurnPosition;

   pageData["Left"]["sensors"] = linksDraaien_Sensors;
   pageData["Left"]["moving"] = linksDraaien;
   pageData["Left"]["eindeloop"] = einde_Linksdraaien;
   pageData["Left"]["force"] = linksDraaien_FORCE;

   pageData["Right"]["sensors"] = rechtsDraaien_Sensors;
   pageData["Right"]["moving"] = rechtsDraaien;
   pageData["Right"]["eindeloop"] = einde_Rechtsdraaien;
   pageData["Right"]["force"] = rechtsDraaien_FORCE;

   // print(" - Turn added");

   pageData["ForceMovement"]["LEFT_Force"] = linksDraaien_FORCE;
   pageData["ForceMovement"]["RIGHT_Force"] = rechtsDraaien_FORCE;
   pageData["ForceMovement"]["OUT_Force"] = uitschuiven_FORCE;
   pageData["ForceMovement"]["IN_Force"] = inschuiven_FORCE;

   // print(" - ForceMovement added");

   pageData["TimeRem"]["antiPendel_Draaien_Timer"] =
      antiPendel_Draaien_Timer_Remaining;
   pageData["TimeRem"]["antiPendel_Kantelen_Timer"] =
      antiPendel_Kantelen_Timer_Remaining;
   pageData["TimeRem"]["draaien_TimeOut"] = draaien_TimeOut_Remaining;
   pageData["TimeRem"]["kantelen_TimeOut"] = kantelen_TimeOut_Remaining;
   pageData["TimeRem"]["logBook_Timer"] = logBook_Timer_Remaining;
   pageData["TimeRem"]["periodicalTimer"] = periodicalTimer_Remaining;
   pageData["TimeRem"]["settingsUnlockedTimer"] =
      settingsUnlockedTimer_Remaining;
   pageData["TimeRem"]["gotoPosition_Timer"] = gotoPosition_Timer_Remaining;

   // print(" - TimeRemaining added");

   pageData["Sensors"]["lichtSensor_LB"] = lichtSensor_LB;
   pageData["Sensors"]["lichtSensor_LO"] = lichtSensor_LO;
   pageData["Sensors"]["lichtSensor_RB"] = lichtSensor_RB;
   pageData["Sensors"]["lichtSensor_RO"] = lichtSensor_RO;
   pageData["Sensors"]["links"] = lichtSensors_Links;
   pageData["Sensors"]["rechts"] = lichtSensors_Rechts;
   pageData["Sensors"]["boven"] = lichtSensors_Boven;
   pageData["Sensors"]["onder"] = lichtSensors_Onder;
   pageData["Sensors"]["isNight"] = isNight();

   // print(" - Sensors added");

   // return pageData;
}

void sendJson(WiFiClient& client, const JSONVar& object) {
   client.print(JSON.stringify(object));
}

void sendPartJson(WiFiClient& client, JSONVar& object) {
   JSONVar keys = object.keys();
   for (int i = 0; i < keys.length(); i++) {
      client.print(keys[i]);
      client.print(": ");
      client.print(object[keys[i]]);
      if (i < keys.length() - 1) {
         client.print(", ");
      }
   }
}
