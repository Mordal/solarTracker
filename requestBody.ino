
//  // Voorbeeld curl commando:
//     //curl -X POST -d "{\"hallo\":5}" http://192.48.56.2/
//  //voorbeeld string to send as Body:
//     // "{\"hallo\":5}"
//     // "{\"hallo\":\"no\"}"

String readBody(WiFiClient client, int contentLength) {
   String body = "";
   while (client.available() && body.length() < contentLength) {
      body += (char)client.read();  // lees de body byte voor byte
   }
   // Serial.print("Body received:");
   // Serial.println(body);
   return body;
}

// bool readBody_OLD(WiFiClient client, int contentLength){
//   // Example curl http://192.48.56.2/ -d "{\"hallo\":\"no\"}"

//   if (!contentLength || contentLength < 1 || contentLength > 1024) {
//     return false;
//     }
//   String requestBody;
//   int bodyRead = 0;
//   while (bodyRead < contentLength && client.available()) {
//     char c = client.read();
//     requestBody += c;
//     bodyRead++;
//     }
//   Serial.println("Raw: " + requestBody);

//   JSONVar jsonBody = JSON.parse(requestBody);
//   Serial.println(jsonBody);
//   setValues(jsonBody);
//   return true;
// }

void unlock() {
   settingsUnlocked = true;
   settingsUnlockedTimer.cancel();
   settingsUnlockedTimer.in(1200000, lockSettings);  // 20min == 1200000
}

bool lockSettings(void *) {
   settingsUnlocked = false;
   return false;
}

void control(WiFiClient client, String body) {
   if (!settingsUnlocked) {
      print("Settings are locked");
      sendInvalidRequest(client);
      return;
   }

   JSONVar jsonBody = JSON.parse(body);

   // FORCED MOVEMENTS //
   if (jsonBody.hasOwnProperty("LEFT_Force")) {
      linksDraaien_FORCE = jsonBody["LEFT_Force"];
   }
   if (jsonBody.hasOwnProperty("RIGHT_Force")) {
      rechtsDraaien_FORCE = jsonBody["RIGHT_Force"];
   }
   if (jsonBody.hasOwnProperty("OUT_Force")) {
      uitschuiven_FORCE = jsonBody["OUT_Force"];
   }
   if (jsonBody.hasOwnProperty("IN_Force")) {
      inschuiven_FORCE = jsonBody["IN_Force"];
   }

   // GOTO POSITION //
   if (jsonBody.hasOwnProperty("TURN_Position")) {
      int input = jsonBody["TURN_Position"];
      int turnPosition = normalizePosition(input);
      gotoTurnPercentage(turnPosition);
   }
   if (jsonBody.hasOwnProperty("TILT_Position")) {
      int input = jsonBody["TILT_Position"];
      int tiltPosition = normalizePosition(input);
      gotoTiltPercentage(tiltPosition);
   }

   // MODES //
   if (jsonBody.hasOwnProperty("TEST_MODE")) {
      TEST_MODE = jsonBody["TEST_MODE"];
   }
   if (jsonBody.hasOwnProperty("SAFE_MODE")) {
      SAFE_MODE = jsonBody["SAFE_MODE"];
   }
   if (jsonBody.hasOwnProperty("STOP_MODE")) {
      STOP_MODE = jsonBody["STOP_MODE"];
   }
   sendJsonData(client, jsonBody);
}

int normalizePosition(int position) {
   if (position < 0) {
      return 0;
   }

   if (position > 10000) {
      return 10000;
   }
   return position;
}

void setValues(WiFiClient client, String body) {
   if (!settingsUnlocked) {
      print("Settings are locked");
      sendInvalidRequest(client);
      return;
   }

   JSONVar jsonBody = JSON.parse(body);
   Serial.println(jsonBody);

   if (jsonBody.hasOwnProperty("LB_Offset")) {
      lichtSensor_LB_offset = jsonBody["LB_Offset"];
   }
   if (jsonBody.hasOwnProperty("RB_Offset")) {
      lichtSensor_RB_offset = jsonBody["RB_Offset"];
   }
   if (jsonBody.hasOwnProperty("LO_Offset")) {
      lichtSensor_LO_offset = jsonBody["LO_Offset"];
   }
   if (jsonBody.hasOwnProperty("RO_Offset")) {
      lichtSensor_RO_offset = jsonBody["RO_Offset"];
   }

   if (jsonBody.hasOwnProperty("licht_marge")) {
      licht_marge = jsonBody["licht_marge"];
   }
   if (jsonBody.hasOwnProperty("antiPendelTime")) {
      antiPendelTime = jsonBody["antiPendelTime"];
   }
   if (jsonBody.hasOwnProperty("maxMovementTime")) {
      maxMovementTime = jsonBody["maxMovementTime"];
   }
   if (jsonBody.hasOwnProperty("retryTime")) {
      retryTime = jsonBody["retryTime"];
      setTimers();
   }
   if (jsonBody.hasOwnProperty("logBook_Timer_delay")) {
      logBook_Timer_delay = jsonBody["logBook_Timer_delay"];
      setTimers();
   }
   sendJsonData(client, jsonBody);
}