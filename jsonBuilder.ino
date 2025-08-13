
JSONVar getFlags() {
   JSONVar flagsObject = undefined;
   flagsObject["Flags"]["TEST_MODE"] = TEST_MODE;
   flagsObject["Flags"]["SAFE_MODE"] = SAFE_MODE;
   flagsObject["Flags"]["STOP_MODE"] = STOP_MODE;
   bool forceMode = linksDraaien_FORCE || rechtsDraaien_FORCE ||
      uitschuiven_FORCE || inschuiven_FORCE || gotoTurnPosition ||
      gotoTiltPosition;
   flagsObject["Flags"]["FORCE_MODE"] = forceMode;
   flagsObject["Flags"]["NIGHT_MODE"] = NIGHT_MODE;
   flagsObject["Flags"]["Draaien_TL"] = draaienTooLong;
   flagsObject["Flags"]["Kantelen_TL"] = kantelenTooLong;
   flagsObject["Flags"]["Wifi_con"] = wifiConnected;
   flagsObject["Flags"]["Mqtt_con"] = mqttConnected;
   flagsObject["Flags"]["ignoreLS"] = ignoreLightSensors;
   flagsObject["Flags"]["forceIgnoreLS"] = forceIgnoreLightSensors;

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
   otherDataObject["syncTime"] = syncTime;
   otherDataObject["Reset"] = resetHappend;
   otherDataObject["resetTime"] = resetTime.toString();
   otherDataObject["SettingsUnlock"] = settingsUnlocked;
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
   settingsObject["Settings"]["licht_fullSun_treshold"] = licht_fullSun_treshold;
   return settingsObject;
}


JSONVar getSettings_TimeOuts() {
   JSONVar settingsObject = undefined;
   settingsObject["TimeOuts"]["APTime"] = antiPendelTime;
   settingsObject["TimeOuts"]["maxMoveTime"] = maxMovementTime;
   settingsObject["TimeOuts"]["periodicalTime"] = periodicalTime;
   settingsObject["TimeOuts"]["settingsUnlockTime"] = settingsUnlockedTime;
   settingsObject["TimeOuts"]["logbookTime"] = logbookTime;
   settingsObject["TimeOuts"]["sendAllDataTime"] = sendAllDataTime;
   settingsObject["TimeOuts"]["clientConTimeOut"] = clientConnectedTimeOut;
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
   remainingTimeObject["TimeRem"]["logBookTime"] = logBook_Timer_Remaining;
   remainingTimeObject["TimeRem"]["clientTimer"] =
      clientConnectedTimer_Remaining;
   remainingTimeObject["TimeRem"]["periodicalTimer"] = periodicalTimer_Remaining;
   remainingTimeObject["TimeRem"]["setUnlockTimer"] =
      settingsUnlockedTimer_Remaining;
   remainingTimeObject["TimeRem"]["gotoPosTimer"] =
      gotoPosition_Timer_Remaining;
   return remainingTimeObject;
}

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
