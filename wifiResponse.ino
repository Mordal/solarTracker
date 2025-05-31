
void sendStartData(WiFiClient& client) {
   // print("Sending start data to client...");
   client.println("HTTP/1.1 200 OK");
   client.println("Access-Control-Allow-Origin: *");
   client.println("Access-Control-Allow-Methods: GET, POST");
   // client.println("Access-Control-Allow-Headers: Content-Type");
}

void sendJsonData(WiFiClient& client, const JSONVar& jsonData) {
   sendStartData(client);
   client.println("Content-type:application/json");
   client.println();
   Serial.println("Sending JSON data to client...");
   sendJson(client, jsonData);
   client.println();
}

void sendAllPageDataByWifi(WiFiClient& client) {
   sendStartData(client);
   client.println("Content-type:application/json");
   client.println();
   client.println("GET ALL DATA - DEPRICATED");
   client.println();
}

void sendOk(WiFiClient& client) {
   sendStartData(client);
   client.println("Content-type:text/plain");
   client.println();
   client.println("OK");
   client.println();
}


JSONVar getEndpoints() {
   const char* endpoints[] = {
      "/API",
      "/API/FLAGS",
      "/API/LIGHTSENSORS",
      "/API/TURNMOVEMENT",
      "/API/TILTMOVEMENT",
      "/API/OTHERDATA",
      "/API/FORCEDMOVEMENTS",
      "/API/SETTINGSOFFSETS",
      "/API/SETTINGSTIMEOUTS",
      "/API/TIMERS",
      "/API/RESETALARM",
      "/API/TURNPRESET",
      "/API/TILTPRESET-monthIndex",
      "/API/CLIENTCONNECTED"
   };

   JSONVar endpointsJson = undefined;
   for (int i = 0; i < sizeof(endpoints) / sizeof(endpoints[0]); i++) {
      endpointsJson[i] = endpoints[i];
   }
   return endpointsJson;
}

void sendEndpoints(WiFiClient& client) {
   sendJsonData(client, getEndpoints());
}

void response_API_Request(WiFiClient& client, const String& currentLine) {
   if (requestHasString(currentLine, "GET /API/PAGEDATA")) {
      sendAllPageDataByWifi(client);
   }
   else if (requestHasString(currentLine, "GET /API/FLAGS")) {
      sendJsonData(client, getFlags());
   }
   else if (requestHasString(currentLine, "GET /API/LIGHTSENSORS")) {
      sendJsonData(client, getLightSensorData());
   }
   else if (requestHasString(currentLine, "GET /API/TURNMOVEMENT")) {
      sendJsonData(client, getTurnMovementData());
   }
   else if (requestHasString(currentLine, "GET /API/TILTMOVEMENT")) {
      sendJsonData(client, getTiltMovementData());
   }
   else if (requestHasString(currentLine, "GET /API/OTHERDATA")) {
      sendJsonData(client, getOtherData());
   }
   else if (requestHasString(currentLine, "GET /API/FORCEDMOVEMENTS")) {
      sendJsonData(client, getForcedMovements());
   }
   else if (requestHasString(currentLine, "GET /API/SETTINGSOFFSETS")) {
      sendJsonData(client, getSettings_Offsets());
   }
   else if (requestHasString(currentLine, "GET /API/SETTINGSTIMEOUTS")) {
      sendJsonData(client, getSettings_TimeOuts());
   }
   else if (requestHasString(currentLine, "GET /API/SETTINGS")) {
      sendJsonData(client, getSettings());
   }
   else if (requestHasString(currentLine, "GET /API/TIMERS") &&
      settingsUnlocked) {
      sendJsonData(client, getTimers());
   }

   else if (requestHasString(currentLine, "GET /API/TIMERS")) {
      sendJsonData(client, getRemainingTime());
   }
   else if (requestHasString(currentLine, "GET /API/RESETALARM") &&
      settingsUnlocked) {
      apiResetAlarms(client);
   }
   else if (requestHasString(currentLine, "GET /API/TURNPRESET")) {
      sendJsonData(client, getTurnPresetPositions());
   }
   else if (requestHasString(currentLine, "GET /API/TILTPRESET")) {
      //"GET /API/TILTPRESET-<monthIndex>"

      // Extract the month index from the request
      int monthIndexStart = currentLine.indexOf('-') + 1;
      if (monthIndexStart == 0) {
         sendEndpoints(client);
         return;
      }
      String monthIndexStr = currentLine.substring(monthIndexStart);
      int monthIndex = monthIndexStr.toInt();

      if (!validMonthIndex(monthIndex)) {
         sendInvalidRequest(client, "Invalid month index");
         return;
      }

      sendJsonData(client, getTiltPresetPositions(monthIndex));
   }
   else if (requestHasString(currentLine, "POST /API/SETTINGS")) {
      String body = client.readStringUntil('\r');
      unlockSettings(client, body);
   }
   else if (requestHasString(currentLine, "POST /API/CONTROL")) {
      String body = client.readStringUntil('\r');
      control(client, body);
   }
   else if (requestHasString(currentLine, "POST /API/UNLOCK")) {
      String body = client.readStringUntil('\r');
      unlockSettings(client, body);
   }
   else if (requestHasString(currentLine, "GET /API/CLIENTCONNECTED")) {
      setClientConnectedTimer();
      sendOk(client);

      // deze moet als laatste
   }
   else if (requestHasString(currentLine, "GET /API")) {
      sendEndpoints(client);
   }
   else {
      sendInvalidRequest(client, "Unknown API request");
   }
}

bool requestHasString(String request, String search) {
   return request.indexOf(search) != -1;
}

void setClientConnectedTimer() {
   clientConnectedTimer.cancel();
   clientConnectedTimer.in(clientConnectedTimeOut, clientConnectedDeactivate);
   stop_Logbook_Timer();
   start_sendAllData_Timer();
}

bool clientConnectedDeactivate(void*) {
   clientConnectedTimer.cancel();
   start_Logbook_Timer();
   return false;
}

void unlockSettings(WiFiClient& client, const String& body) {
   // print(body);
   if (body == "reteipreteip") {
      unlock();
      sendStartData(client);
      client.println("Content-type:text/plain");
      client.println();
      client.println("Settings unlocked");
      client.println();
   }
   else {
      sendInvalidRequest(client, "FUCK OFF");
   }
}

void apiResetAlarms(WiFiClient& client) {
   resetAlarms();
   sendStartData(client);
   client.println("Content-type:text/plain");
   client.println();
   client.println("Alarms reset");
   client.println();
}
