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
   settingsUnlockedTimer.in(settingsUnlockedTime, lockSettings);  // 20min == 1200000
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

   if (jsonBody.hasOwnProperty("initializeNeededTime")) {
      // curl -X POST -d "{\"initializeNeededTime\":true}" http://192.168.0.205:90/CONTROL
      if (dailyTest_Active) {
         initializeNeededTime();
         sendJsonData(client, jsonBody);
         return;
      }
      dailyTest_Active = true;
      initializeNeededTime();
      dailyTest_Active = false;
   }


   // MODES

   if (jsonBody.hasOwnProperty("SAFE_MODE")) {
      //check if jsonBody["SAFE_MODE"] is a boolean
      if (JSON.typeof(jsonBody["SAFE_MODE"]) == "boolean") {
         // curl -X POST -d "{\"SAFE_MODE\":true}" http://192.168.0.205:90/CONTROL
         SAFE_MODE = (bool)jsonBody["SAFE_MODE"];
         if (SAFE_MODE) {
            gotoSafePosition();
         }
      }
      else {
         // curl -X POST -d "{\"SAFE_MODE\":{\"TURN_Position\":50,\"TILT_Position\":50}}" http://192.168.0.205:90/CONTROL
         int turnPosition = normalizePosition((int)jsonBody["SAFE_MODE"]["TURN_Position"]) || undefined;
         int tiltPosition = normalizePosition((int)jsonBody["SAFE_MODE"]["TILT_Position"]) || undefined;
         gotoSafePosition(turnPosition, tiltPosition);
         clearForceSignals();
      }
      sendJsonData(client, jsonBody);
      return;
   }

   if (jsonBody.hasOwnProperty("STOP_MODE")) {
      // curl -X POST -d "{\"STOP_MODE\":true"} http://192.168.0.205:90/CONTROL
      STOP_MODE = (bool)jsonBody["STOP_MODE"];
      stopMomevement();
      clearForceSignals();
   }

   // GOTO POSITION
   if (jsonBody.hasOwnProperty("TURN_Position")) {
      int input = (int)jsonBody["TURN_Position"];
      int turnPosition = normalizePosition(input);
      gotoTurnPercentage(turnPosition);
   }
   if (jsonBody.hasOwnProperty("TILT_Position")) {
      int input = (int)jsonBody["TILT_Position"];
      int tiltPosition = normalizePosition(input);
      gotoTiltPercentage(tiltPosition);
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
   }
}

void setFromJson(JSONVar& json, const char* key, unsigned int& var) {
   if (json.hasOwnProperty(key)) {
      var = (int)json[key];
   }
}

void setFromJson(JSONVar& json, const char* key, unsigned long& var) {
   if (json.hasOwnProperty(key)) {
      var = (unsigned long)json[key];
   }
}

void setFromJson(JSONVar& json, const char* key, bool& var) {
   if (json.hasOwnProperty(key)) {
      var = (bool)json[key];
   }
}

void setTimerFromJson(JSONVar& json, const char* key, unsigned int& var) {
   if (json.hasOwnProperty(key)) {
      var = (unsigned int)json[key];
      restartTimers();
   }
}

void setTimerFromJson(JSONVar& json, const char* key, unsigned long& var) {
   if (json.hasOwnProperty(key)) {
      var = (unsigned long)json[key];
      restartTimers();
   }
}


void setArrayFromJson(JSONVar& json, const char* key, char var[12][15]) {
   if (json.hasOwnProperty(key)) {
      JSONVar mqttArray = json[key];
      int newSize = mqttArray.length();
      if (newSize > 12) newSize = 12;

      // Leeg de oude array
      for (int i = 0; i < 12; i++) var[i][0] = '\0';

      // Zet de array op basis van de nieuwe grootte
      for (int i = 0; i < newSize; i++) {
         String topic = mqttArray[i];
         if (topic.length() > 14) continue; // max 14 + '\0'
         strncpy(var[i], topic.c_str(), 14);
         var[i][14] = '\0';
      }
   }
}

void setValues(WiFiClient& client, const String& body) {
   if (settingsAreLocked(client)) return;

   JSONVar jsonBody = JSON.parse(body);
   if (!validateJson(client, jsonBody)) return;

   //example offset LB
   // curl -X POST -d "{\"LB_Offset\":100}" http://192.168.0.205:90/SETTINGS

   setFromJson(jsonBody, "LB_Offset", lichtSensor_LB_offset);
   setFromJson(jsonBody, "RB_Offset", lichtSensor_RB_offset);
   setFromJson(jsonBody, "LO_Offset", lichtSensor_LO_offset);
   setFromJson(jsonBody, "RO_Offset", lichtSensor_RO_offset);
   setFromJson(jsonBody, "licht_marge", licht_marge);
   setFromJson(jsonBody, "licht_fullSun_treshold", licht_fullSun_treshold);
   setFromJson(jsonBody, "antiPendelTime", antiPendelTime);
   setFromJson(jsonBody, "maxMovementTime", maxMovementTime);
   setFromJson(jsonBody, "timeNeededToTurn", timeNeededToTurn);
   setFromJson(jsonBody, "timeNeededToTilt", timeNeededToTilt);
   setFromJson(jsonBody, "dailyTest_Active", dailyTest_Active);
   setFromJson(jsonBody, "syncTime", syncTime);

   setTimerFromJson(jsonBody, "logbookTime", logbookTime);
   setTimerFromJson(jsonBody, "periodicalTime", periodicalTime);
   setTimerFromJson(jsonBody, "sendAllDataTime", sendAllDataTime);
   setTimerFromJson(jsonBody, "clientConnectedTimeOut", clientConnectedTimeOut);
   setTimerFromJson(jsonBody, "settingsUnlockedTime", settingsUnlockedTime);

   if (jsonBody.hasOwnProperty("tilt_Presets")) {
      //example JSON :
      // {"tilt_Presets":{"monthIndex":0,"presets":[0,0,0,0,0,0,10,10,0,0,0,0,0,0]}}
      // curl -X POST -d "{\"tilt_Presets\":{\"monthIndex\":2,\"presets\":[0,0,0,0,0,0,10,10,5,0,0,0,0,0]}}}" http://192.168.0.205:90/SETTINGS

      JSONVar tilt_Presets = jsonBody["tilt_Presets"];

      if (!tilt_Presets.hasOwnProperty("monthIndex") || !tilt_Presets.hasOwnProperty("presets")) {
         sendInvalidRequest(client, "Missing monthIndex or presets property in tilt_Presets");
         return;
      }

      byte monthIndex = (byte)tilt_Presets["monthIndex"];
      JSONVar arr = tilt_Presets["presets"];
      byte newtiltPresets[14];

      if (!validMonthIndex(monthIndex) || JSON.typeof(monthIndex) != "number") {
         sendInvalidRequest(client, "Invalid month index");
         return;
      }

      for (int i = 0; i < 14; i++) {
         if (!validPresetValue(arr[i]) || JSON.typeof(arr[i]) != "number") {
            sendInvalidRequest(client, "Invalid tilt preset value [#" + String(i + 1) + "]");
            return;
         }
         newtiltPresets[i] = (byte)arr[i];
      }
      setTiltPercentage_Presets(monthIndex, newtiltPresets);
   }

   if (jsonBody.hasOwnProperty("turn_Presets")) {

      //example JSON :
      // {"turn_Presets":{"turnPresets":[0,5,10,19,28,35,42,52,61,70,75,80,88,100]}}
      // curl -X POST -d "{\"turn_Presets\":{\"presets\":[0,5,10,19,28,35,42,52,61,70,75,80,88,100]}}" http://192.168.0.205:90/SETTINGS

      JSONVar turn_Presets = jsonBody["turn_Presets"];
      if (!turn_Presets.hasOwnProperty("presets")) {
         sendInvalidRequest(client, "Missing presets property in turn_Presets");
         return;
      }

      JSONVar arr = turn_Presets["presets"];
      byte newturnPresets[14];
      for (int i = 0; i < 14; i++) {

         if (!validPresetValue(arr[i]) || JSON.typeof(arr[i]) != "number") {
            sendInvalidRequest(client, "Invalid turn preset value [#" + String(i + 1) + "]");
            return;
         }

         newturnPresets[i] = (byte)arr[i];
      }
      setTurnPercentage_Presets(newturnPresets);
   }

   if (jsonBody.hasOwnProperty("setTime")) {
      //   void setTimeManually(int day, int month, int year, int hour, int minute,
      //    int second) {
      // example JSON:
      // {"setTime":[1,1,2025,12,0,0]}
      // curl -X POST -d "{\"setTime\":[1,5,2025,12,0,0]}" http://192.168.0.205:90/SETTINGS

      JSONVar setTime = jsonBody["setTime"];
      if (setTime.length() != 6) {
         sendInvalidRequest(client, "Invalid setTime array length");
         return;
      }
      int day = (int)setTime[0];
      int month = (int)setTime[1];
      int year = (int)setTime[2];
      int hour = (int)setTime[3];
      int minute = (int)setTime[4];
      int second = (int)setTime[5];

      if (day < 1 || day > 31 || month < 1 || month > 12 || year < 2000 ||
         hour < 0 || hour > 23 || minute < 0 || minute > 59 ||
         second < 0 || second > 59) {
         sendInvalidRequest(client, "Invalid date/time values");
         return;
      }
      setTimeManually(day, month, year, hour, minute, second);
   }

   // MQTT TOPICS
   setArrayFromJson(jsonBody, "mqtt_allData", mqtt_allData);
   //curl -X POST -d "{\"mqtt_allData\":[\"flags\",\"sensors\",\"tilt\"]}" http://192.168.0.205:90/SETTINGS
   setArrayFromJson(jsonBody, "mqtt_Logbook", mqtt_Logbook);
   //curl -X POST -d "{\"mqtt_Logbook\":[\"flags\",\"sensors\",\"tilt\"]}" http://192.168.0.205:90/SETTINGS

   sendJsonData(client, jsonBody);
}

bool validPresetValue(int value) {
   bool isValid = (value >= 0 && value <= 100);
   return (value >= 0 && value <= 100);
}

bool validMonthIndex(int monthIndex) {
   return (monthIndex >= 0 && monthIndex <= 11);
}