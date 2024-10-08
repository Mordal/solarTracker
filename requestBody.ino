
String readBody(WiFiClient client, int contentLength){
    String body ="";
    while (client.available() && body.length() < contentLength) {
        body += (char)client.read();   // lees de body byte voor byte
    }
    Serial.print("Body received:");
    Serial.println(body); 
    return body;
}

bool readBody_OLD(WiFiClient client, int contentLength){
  // Example curl http://192.48.56.2/ -d "{\"hallo\":\"no\"}"
  
  if (!contentLength || contentLength < 1 || contentLength > 1024) {
    return false;
    }
  String requestBody;
  int bodyRead = 0;
  while (bodyRead < contentLength && client.available()) {
    char c = client.read();
    requestBody += c;
    bodyRead++;
    }
  Serial.println("Raw: " + requestBody);
 // Voorbeeld curl commando:
    //curl -X POST -d "{\"hallo\":5}" http://192.48.56.2/
 //voorbeeld string to send as Body:
    // "{\"hallo\":5}"
    // "{\"hallo\":\"no\"}"
  JSONVar jsonBody = JSON.parse(requestBody);
  Serial.println(jsonBody);
  setValues(jsonBody);
  return true;
}

void unlockSettings(){
    settingsUnlocked = true;
    settingsUnlockedTimer.in(1200000, lockSettings);  //20min == 1200000
}


bool lockSettings(void *){
    settingsUnlocked = false;
    return false;
}


void setValues(JSONVar jsonBody){
    if (jsonBody.hasOwnProperty("TEST_MODE")) {
        TEST_MODE = jsonBody["TEST_MODE"];
    }
    if (jsonBody.hasOwnProperty("SAFE_MODE")) {
        SAFE_MODE = jsonBody["SAFE_MODE"];
    }
    if (jsonBody.hasOwnProperty("LEFT_Force")){
        linksDraaien_FORCE = jsonBody["LEFT_Force"];
    }
    if (jsonBody.hasOwnProperty("Force_RIGHT")){
        rechtsDraaien_FORCE = jsonBody["Force_RIGHT"];
    }
    if (jsonBody.hasOwnProperty("Force_OUT")){
        uitschuiven_FORCE = jsonBody["Force_OUT"];
    }
    if (jsonBody.hasOwnProperty("Force_IN")){
        inschuiven_FORCE = jsonBody["Force_IN"];
    }
    if (jsonBody.hasOwnProperty("Reset_Alarms")){
        if (jsonBody["Reset_Alarms"]){
            draaienTooLong = false;
            kantelenTooLong = false;
        }
    }
    if (jsonBody.hasOwnProperty("licht_marge")){
        licht_marge = jsonBody["licht_marge"];
    }
    if (jsonBody.hasOwnProperty("antiPendelTime")){
        antiPendelTime = jsonBody["antiPendelTime"];
    }
    if (jsonBody.hasOwnProperty("maxMovementTime")){
        maxMovementTime = jsonBody["maxMovementTime"];
    }
    if (jsonBody.hasOwnProperty("logBook_Timer_delay")){
        logBook_Timer_delay = jsonBody["logBook_Timer_delay"];
        setTimers();
    }
    if (jsonBody.hasOwnProperty("retryTime")){
        retryTime = jsonBody["retryTime"];
        setTimers();
    }
}