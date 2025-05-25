//  // Voorbeeld curl commando:
//     //curl -X POST -d "{\"hallo\":5}" http://192.48.56.2/
//  //voorbeeld string to send as Body:
//     // "{\"hallo\":5}"
//     // "{\"hallo\":\"no\"}"

void sendInvalidRequest(WiFiClient& client, const String& errorMessage = "") {
   client.println("HTTP/1.1 400 BAD REQUEST");
   client.println("Access-Control-Allow-Origin: *");
   client.println("Access-Control-Allow-Methods: GET, POST");
   client.println("Content-type:text/plain");
   client.println();
   client.println("Invalid request- " + errorMessage);
   client.println();
}


String readBody(WiFiClient& client, int contentLength) {
   String body = "";
   while (client.available() && body.length() < contentLength) {
      body += (char)client.read();  // lees de body byte voor byte
   }
   return body;
}

void unlock() {
   settingsUnlocked = true;
   settingsUnlockedTimer.cancel();
   settingsUnlockedTimer.in(1200000, lockSettings);  // 20min == 1200000
}

bool lockSettings(void*) {
   settingsUnlocked = false;
   return false;
}

bool settingsAreLocked(WiFiClient& client) {
   if (!settingsUnlocked) {
      print("Settings are locked");
      sendInvalidRequest(client, "Settings are locked");
      return true;
   }
   return false;
}

bool validateJson(WiFiClient& client, JSONVar& jsonBody) {
   if (JSON.typeof(jsonBody) != "object") {
      print("Invalid JSON body");
      sendInvalidRequest(client, "Invalid JSON body");
      return false;
   }
   return true;
}

void control(WiFiClient& client, const String& body) {
   if (settingsAreLocked(client)) return;

   JSONVar jsonBody = JSON.parse(body);
   if (!validateJson(client, jsonBody)) return;

   // FORCED MOVEMENTS
   setFromJson(jsonBody, "LEFT_Force", linksDraaien_FORCE_EXT);
   setFromJson(jsonBody, "RIGHT_Force", rechtsDraaien_FORCE_EXT);
   setFromJson(jsonBody, "OUT_Force", uitschuiven_FORCE_EXT);
   setFromJson(jsonBody, "IN_Force", inschuiven_FORCE_EXT);

   // MODES
   setFromJson(jsonBody, "TEST_MODE", TEST_MODE);
   setFromJson(jsonBody, "SAFE_MODE", SAFE_MODE);


   if (jsonBody.hasOwnProperty("STOP_MODE")) {
      STOP_MODE = (bool)jsonBody["STOP_MODE"];
      stopMomevement();
      clearForceSignals();
   }

   // GOTO POSITION
   if (jsonBody.hasOwnProperty("TURN_Position")) {
      int input = (int)jsonBody["TURN_Position"];
      int turnPosition = normalizePosition(input);
      gotoTurnPercentage(turnPosition);
      Serial.print("Goto TURN_Position: ");
      Serial.println(turnPosition);
   }
   if (jsonBody.hasOwnProperty("TILT_Position")) {
      int input = (int)jsonBody["TILT_Position"];
      int tiltPosition = normalizePosition(input);
      gotoTiltPercentage(tiltPosition);
      Serial.print("Goto TILT_Position: ");
      Serial.println(tiltPosition);
   }

   sendJsonData(client, jsonBody);
}

int normalizePosition(int position) {
   if (position < 0) return 0;
   if (position > 10000) return 10000;
   return position;
}

void setFromJson(JSONVar& json, const char* key, int& var) {
   if (json.hasOwnProperty(key)) {
      var = (int)json[key];
      Serial.print("Set ");
      Serial.print(key);
      Serial.print(" to: ");
      Serial.println(var);
   }
}

void setFromJson(JSONVar& json, const char* key, unsigned int& var) {
   if (json.hasOwnProperty(key)) {
      var = (int)json[key];
      Serial.print("Set ");
      Serial.print(key);
      Serial.print(" to: ");
      Serial.println(var);
   }
}

void setFromJson(JSONVar& json, const char* key, bool& var) {
   if (json.hasOwnProperty(key)) {
      var = (bool)json[key];
   }
}

void setValues(WiFiClient& client, const String& body) {
   if (settingsAreLocked(client)) return;

   JSONVar jsonBody = JSON.parse(body);
   if (!validateJson(client, jsonBody)) return;

   setFromJson(jsonBody, "LB_Offset", lichtSensor_LB_offset);
   setFromJson(jsonBody, "RB_Offset", lichtSensor_RB_offset);
   setFromJson(jsonBody, "LO_Offset", lichtSensor_LO_offset);
   setFromJson(jsonBody, "RO_Offset", lichtSensor_RO_offset);
   setFromJson(jsonBody, "licht_marge", licht_marge);
   setFromJson(jsonBody, "antiPendelTime", antiPendelTime);
   setFromJson(jsonBody, "maxMovementTime", maxMovementTime);

   if (jsonBody.hasOwnProperty("periodicalTime")) {
      periodicalTime = (int)jsonBody["periodicalTime"];
      Serial.print("Set periodicalTime to: ");
      Serial.println(periodicalTime);
      setTimers();
   }
   if (jsonBody.hasOwnProperty("logBook_Timer_delay")) {
      logBook_Timer_delay = (int)jsonBody["logBook_Timer_delay"];
      Serial.print("Set logBook_Timer_delay to: ");
      Serial.println(logBook_Timer_delay);
      setTimers();
   }

   if (jsonBody.hasOwnProperty("tilt_Presets")) {
      //example JSON :
      // {"tilt_Presets":{"monthIndex":0,"presets":[0,0,0,0,0,0,10,10,0,0,0,0,0,0]}}
      // curl -X POST -d "{\"tilt_Presets\":{\"monthIndex\":2,\"presets\":[0,0,0,0,0,0,10,10,5,0,0,0,0,0]}}}" http://192.168.0.205:90/SETTINGS

      JSONVar tilt_Presets = jsonBody["tilt_Presets"];
      byte monthIndex = (byte)tilt_Presets["monthIndex"];
      JSONVar arr = tilt_Presets["presets"];
      byte newtiltPresets[14];

      if (!validMonthIndex(monthIndex)) {
         sendInvalidRequest(client, "Invalid month index");
         return;
      }


      for (int i = 0; i < 14; i++) {
         if (!validPresetValue(arr[i])) {
            sendInvalidRequest(client, "Invalid tilt preset value");
            return;
         }
         newtiltPresets[i] = (byte)arr[i];
      }
      setTiltPercentage_Presets(monthIndex, newtiltPresets);
   }

   if (jsonBody.hasOwnProperty("turn_Presets")) {

      //example JSON :
      // {"turn_Presets":{"turnPresets":[0,5,10,19,28,35,42,52,61,70,75,80,88,100]}}
      // curl -X POST -d "{\"turn_Presets\":{\"turnPresets\":[0,5,10,19,28,35,42,52,61,70,75,80,88,100]}}" http://192.168.0.205:90/SETTINGS

      JSONVar turn_Presets = jsonBody["turn_Presets"];
      JSONVar arr = turn_Presets["turnPresets"];
      byte newturnPresets[14];
      for (int i = 0; i < 14; i++) {
         if (!validPresetValue(arr[i])) {
            sendInvalidRequest(client, "Invalid turn preset value");
            return;
         }

         newturnPresets[i] = (byte)arr[i];
      }
      setTurnPercentage_Presets(newturnPresets);
   }

   sendJsonData(client, jsonBody);
}

bool validPresetValue(int value) {
   return (value >= 0 && value <= 100);
}

bool validMonthIndex(int monthIndex) {
   return (monthIndex >= 0 && monthIndex <= 11);
}