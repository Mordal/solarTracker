

 void response_WiFi_BASIC(WiFiClient client){
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();

    printContent(client);
    // the content of the HTTP response follows the header:
    // client.print("<p style=\"font-size:7vw;\">Click <a href=\"/H\">here</a> turn the LED on<br></p>");
    // client.print("<p style=\"font-size:7vw;\">Click <a href=\"/L\">here</a> turn the LED off<br></p>");

    // The HTTP response ends with another blank line:
    client.println();
 }

void response_WiFi_JSON(WiFiClient client){
  print("responding to JSON request..");
 // Making a JSON Object
  Serial.println(myObject);

  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:application/json");
  client.println();

  // the content of the HTTP response follows the header:
  JSONVar fullObject = addAllInfoToObject();
  client.println(fullObject);
  
  // The HTTP response ends with another blank line:
  client.println();
}

JSONVar addAllInfoToObject(){
  JSONVar fullObject = myObject;
  fullObject["Flags"]["TEST_MODE"] = TEST_MODE;
  fullObject["Flags"]["SAFE_MODE"] = SAFE_MODE;
  fullObject["Flags"]["Draaien_too_long"] = draaienTooLong;
  fullObject["Flags"]["Kantelen_too_long"] = kantelenTooLong;
  fullObject["Flags"]["Wifi_connected"] = wifiConnected;
  fullObject["Flags"]["Mqtt_connected"] = mqttConnected;
  fullObject["ForceMovement"]["Force_LEFT"] = linksDraaien_FORCE;
  fullObject["ForceMovement"]["Force_RIGHT"] = rechtsDraaien_FORCE;
  fullObject["ForceMovement"]["Force_OUT"] = uitschuiven_FORCE;
  fullObject["ForceMovement"]["Force_IN"] = inschuiven_FORCE;
  fullObject["TimeOuts"]["antiPendelTime"] = antiPendelTime;
  fullObject["TimeOuts"]["maxMovementTime"] = maxMovementTime;
  fullObject["TimeOuts"]["logBook_Timer"] = logBook_Timer;
  fullObject["TimeOuts"]["retryTime"] = retryTime;
  fullObject["Reset_Alarms"] = false;
  return fullObject;
}
