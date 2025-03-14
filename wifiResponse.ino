
void sendStartData(WiFiClient client) {
   // print("Sending start data to client...");
   client.println("HTTP/1.1 200 OK");
   client.println("Access-Control-Allow-Origin: *");
   client.println("Access-Control-Allow-Methods: GET, POST");
   // client.println("Access-Control-Allow-Headers: Content-Type");
}

void sendJsonData(WiFiClient client, JSONVar jsonData) {
   sendStartData(client);
   client.println("Content-type:application/json");
   client.println();
   sendJson(client, jsonData);
   client.println();
}

void sendAllPageDataByWifi(WiFiClient client) {
   sendStartData(client);
   client.println("Content-type:application/json");
   client.println();
   client.println("GET ALL DATA - DEPRICATED");
   client.println();
}

void sendOk(WiFiClient client) {
   sendStartData(client);
   client.println("Content-type:text/plain");
   client.println();
   client.println("OK");
   client.println();
}

void sendInvalidRequest(WiFiClient client) {
   client.println("HTTP/1.1 400 BAD REQUEST");
   client.println("Access-Control-Allow-Origin: *");
   client.println("Access-Control-Allow-Methods: GET, POST");
   client.println("Content-type:text/plain");
   client.println();
   client.println("Invalid request");
   client.println();
}

void sendEndpoints(WiFiClient client) {
   sendStartData(client);
   client.println("Content-type:text/html");
   client.println();
   client.println("Available endpoints:");
   client.println("/API");
   client.println("/API/FLAGS");
   client.println("/API/LIGHTSENSORS");
   client.println("/API/TURNMOVEMENT");
   client.println("/API/TILTMOVEMENT");
   client.println("/API/OTHERDATA");
   client.println("/API/FORCEDMOVEMENTS");
   client.println("/API/SETTINGS");
   client.println("/API/TIMERS");
   client.println("/API/RESETALARM");
   client.println("/API/CLIENTCONNECTED");
   client.println();
}

void response_API_Request(WiFiClient client, String currentLine) {
   if (requestHasString(currentLine, "GET /API/PAGEDATA")) {
      sendAllPageDataByWifi(client);
   } else if (requestHasString(currentLine, "GET /API/FLAGS")) {
      sendJsonData(client, getFlags());
   } else if (requestHasString(currentLine, "GET /API/LIGHTSENSORS")) {
      sendJsonData(client, getLightSensorData());
   } else if (requestHasString(currentLine, "GET /API/TURNMOVEMENT")) {
      sendJsonData(client, getTurnMovementData());
   } else if (requestHasString(currentLine, "GET /API/TILTMOVEMENT")) {
      sendJsonData(client, getTiltMovementData());
   } else if (requestHasString(currentLine, "GET /API/OTHERDATA")) {
      sendJsonData(client, getOtherData());
   } else if (requestHasString(currentLine, "GET /API/FORCEDMOVEMENTS")) {
      sendJsonData(client, getForcedMovements());
   } else if (requestHasString(currentLine, "GET /API/SETTINGS")) {
      sendJsonData(client, getSettings());
   } else if (requestHasString(currentLine, "GET /API/TIMERS")) {
      sendJsonData(client, getRemainingTime());
   } else if (requestHasString(currentLine, "GET /API/RESETALARM") &&
              settingsUnlocked) {
      apiResetAlarms(client);
   } else if (requestHasString(currentLine, "GET /API/CLIENTCONNECTED")) {
      setClientConnectedTimer();
      sendOk(client);

      // deze moet als laatste
   } else if (requestHasString(currentLine, "GET /API")) {
      sendEndpoints(client);
   } else {
      sendInvalidRequest(client);
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

bool clientConnectedDeactivate(void *) {
   clientConnectedTimer.cancel();
   start_Logbook_Timer();
   return false;
}

void unlockSettings(WiFiClient client, String body) {
   // print(body);
   if (body == "reteipreteip") {
      unlock();
      sendStartData(client);
      client.println("Content-type:text/plain");
      client.println();
      client.println("Settings unlocked");
      client.println();
   } else {
      sendInvalidRequest(client);
   }
}

void apiResetAlarms(WiFiClient client) {
   resetAlarms();
   sendStartData(client);
   client.println("Content-type:text/plain");
   client.println();
   client.println("Alarms reset");
   client.println();
}
