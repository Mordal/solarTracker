//  // Voorbeeld curl commando:
//     //curl -X POST -d "{\"hallo\":5}" http://192.48.56.2/
//  //voorbeeld string to send as Body:
//     // "{\"hallo\":5}"
//     // "{\"hallo\":\"no\"}"

String readBody(WiFiClient& client, int contentLength) {
   String body = "";
   while (client.available() && body.length() < contentLength) {
      body += (char)client.read();  // lees de body byte voor byte
      }
   // Serial.print("Body received:");
   // Serial.println(body);
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
      sendInvalidRequest(client);
      return true;
      }
   return false;
   }

bool validateJson(WiFiClient& client, JSONVar& jsonBody) {
   if (JSON.typeof(jsonBody) != "object") {
      print("Invalid JSON body");
      sendInvalidRequest(client);
      return false;
      }
   return true;
   }

void control(WiFiClient& client, const String& body) {
   if (settingsAreLocked(client)) return;

   JSONVar jsonBody = JSON.parse(body);
   if (!validateJson(client, jsonBody)) return;

   // FORCED MOVEMENTS
   setFromJson(jsonBody, "LEFT_Force", linksDraaien_FORCE);
   setFromJson(jsonBody, "RIGHT_Force", rechtsDraaien_FORCE);
   setFromJson(jsonBody, "OUT_Force", uitschuiven_FORCE);
   setFromJson(jsonBody, "IN_Force", inschuiven_FORCE);

   // MODES
   setFromJson(jsonBody, "TEST_MODE", TEST_MODE);
   setFromJson(jsonBody, "SAFE_MODE", SAFE_MODE);
   setFromJson(jsonBody, "STOP_MODE", STOP_MODE);

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
      Serial.print("Set ");
      Serial.print(key);
      Serial.print(" to: ");
      Serial.println(var);
      }
   }

void setValues(WiFiClient& client, const String& body) {
   if (settingsAreLocked(client)) return;

   JSONVar jsonBody = JSON.parse(body);
   if (!validateJson(client, jsonBody)) return;
   Serial.println(jsonBody);

   setFromJson(jsonBody, "LB_Offset", lichtSensor_LB_offset);
   setFromJson(jsonBody, "RB_Offset", lichtSensor_RB_offset);
   setFromJson(jsonBody, "LO_Offset", lichtSensor_LO_offset);
   setFromJson(jsonBody, "RO_Offset", lichtSensor_RO_offset);
   setFromJson(jsonBody, "licht_marge", licht_marge);
   setFromJson(jsonBody, "antiPendelTime", antiPendelTime);
   setFromJson(jsonBody, "maxMovementTime", maxMovementTime);

   if (jsonBody.hasOwnProperty("retryTime")) {
      retryTime = (int)jsonBody["retryTime"];
      Serial.print("Set retryTime to: ");
      Serial.println(retryTime);
      setTimers();
      }
   if (jsonBody.hasOwnProperty("logBook_Timer_delay")) {
      logBook_Timer_delay = (int)jsonBody["logBook_Timer_delay"];
      Serial.print("Set logBook_Timer_delay to: ");
      Serial.println(logBook_Timer_delay);
      setTimers();
      }

   if (jsonBody.hasOwnProperty("tilt_Presets")) {
      JSONVar tilt_Presets = jsonBody["tilt_Presets"];
      byte monthIndex = (byte)tilt_Presets["monthIndex"];
      JSONVar arr = tilt_Presets["presets"];
      byte newtiltPresets[14];
      for (int i = 0; i < 14; i++) {
         newtiltPresets[i] = (byte)arr[i];
         }
      setTiltPercentage_Presets(monthIndex, newtiltPresets);
      Serial.println("Set tilt_Presets");
      }

   if (jsonBody.hasOwnProperty("turn_Presets")) {
      JSONVar turn_Presets = jsonBody["turn_Presets"];
      JSONVar arr = turn_Presets["turnPresets"];
      byte newturnPresets[14];
      for (int i = 0; i < 14; i++) {
         newturnPresets[i] = (byte)arr[i];
         }
      setTurnPercentage_Presets(newturnPresets);
      Serial.println("Set turn_Presets");
      }

   sendJsonData(client, jsonBody);
   }