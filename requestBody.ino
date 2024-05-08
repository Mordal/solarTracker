bool readBody(WiFiClient client, int contentLength){
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
  print("Raw: " + requestBody);
  //voorbeeld string to send as Body:
  // "{\"hallo\":5}"
  // "{\"hallo\":\"no\"}"
  JSONVar jsonBody = JSON.parse(requestBody);
  Serial.println(jsonBody);
  return true
}

void setValues(JSONVar jsonBody){
    if (jsonBody.hasOwnProperty("TEST_MODE")) {
        TEST_MODE = jsonBody["TEST_MODE"];
    }
    if (jsonBody.hasOwnProperty("SAFE_MODE")) {
        SAFE_MODE = jsonBody["SAFE_MODE"];
    }
    if (jsonBody.hasOwnProperty("Force_LEFT")){
        linksDraaien_FORCE = jsonBody["Force_LEFT"];
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
        if jsonBody["Reset_Alarms"] == true{
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
    if (jsonBody.hasOwnProperty("logBook_Timer")){
        logBook_Timer = jsonBody["logBook_Timer"];
    }
    if (jsonBody.hasOwnProperty("retryTime")){
        retryTime = jsonBody["retryTime"];
    }
}