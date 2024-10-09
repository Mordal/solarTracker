
 
 void sendJsonData(WiFiClient client, JSONVar jsonData){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:application/json");
  client.println();
  printJsonData(client, jsonData);
  client.println();
 }

 void sendInvalidRequest(WiFiClient client){
  client.println("HTTP/1.1 400 BAD REQUEST");
  client.println("Content-type:text/plain");
  client.println();
  client.println("Invalid request");
  client.println();
 }

void sendEndpoints(WiFiClient client){
  client.println("HTTP/1.1 200 OK");
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
  client.println();
}

//  void response_WiFi_BASIC(WiFiClient client){
//     client.println("HTTP/1.1 200 OK");
//     client.println("Content-type:text/html");
//     client.println();

//     printContent_Global(client);
//     // the content of the HTTP response follows the header:
//     // client.print("<p style=\"font-size:7vw;\">Click <a href=\"/H\">here</a> turn the LED on<br></p>");
//     // client.print("<p style=\"font-size:7vw;\">Click <a href=\"/L\">here</a> turn the LED off<br></p>");

//     // The HTTP response ends with another blank line:
//     client.println();
//  }

void response_API_Request(WiFiClient client, String currentLine){


  if (requestHasString(currentLine, "GET /API/FLAGS")){
    sendJsonData(client, getFlags()); 
  }
  else if (requestHasString(currentLine, "GET /API/LIGHTSENSORS")){
    sendJsonData(client, getLightSensorData());
  }
  else if (requestHasString(currentLine, "GET /API/TURNMOVEMENT")){
    sendJsonData(client, getTurnMovementData());
  }
  else if (requestHasString(currentLine, "GET /API/TILTMOVEMENT")){
    sendJsonData(client, getTiltMovementData());
  }
  else if (requestHasString(currentLine, "GET /API/OTHERDATA")){
    sendJsonData(client, getOtherData());
  }
  else if (requestHasString(currentLine, "GET /API/FORCEDMOVEMENTS")){
    sendJsonData(client, getForcedMovements());
  }
  else if (requestHasString(currentLine, "GET /API/SETTINGS")){
    sendJsonData(client, getSettings());
  }
  else if (requestHasString(currentLine, "GET /API/TIMERS")){
    sendJsonData(client, getRemainingTime());
  }
  else if (requestHasString(currentLine, "GET /API")){
    sendEndpoints(client);
  }
  else{
    sendInvalidRequest(client);
  }
}

bool requestHasString(String request, String search){
  return request.indexOf(search) != -1;
}


void unlockSettings(WiFiClient client, String body){
  if(body == "reteipreteip"){
    unlock();
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/plain");
    client.println();
    client.println("Settings unlocked");
    client.println();
  }
  else{
    sendInvalidRequest(client);
  }
}


// void response_WiFi_JSON(WiFiClient client){
//   print("responding to JSON request..");
//   // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
//   // and a content-type so the client knows what's coming, then a blank line:
//   client.println("HTTP/1.1 200 OK");
//   client.println("Content-type:application/json");
//   client.println();

//   // the content of the HTTP response follows the header:
//   clientPrintAllData(client);
  
//   // The HTTP response ends with another blank line:
//   client.println();
// }

// void response_WiFi_Timers(WiFiClient client){
//   print("responding to Timers request..");
//   // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
//   // and a content-type so the client knows what's coming, then a blank line:
//   client.println("HTTP/1.1 200 OK");
//   client.println("Content-type:application/json");
//   client.println();

//   // the content of the HTTP response follows the header:
//   JSONVar  timersObject = getRemainingTime();
//   client.println(timersObject);
  
//   // The HTTP response ends with another blank line:
//   client.println();
// }

