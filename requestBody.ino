bool readBody(WiFiClient client, int contentLength){
  // Example curl http://192.48.56.2/ -d "{\"hallo\":\"no\"}"
  String requestBody;
  if (!contentLength || contentLength < 1) {
    return false;
    }
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
}