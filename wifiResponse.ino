
 void response_WiFi_BASIC(WiFiClient client){
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();

    printContent_Global(client);
    // the content of the HTTP response follows the header:
    // client.print("<p style=\"font-size:7vw;\">Click <a href=\"/H\">here</a> turn the LED on<br></p>");
    // client.print("<p style=\"font-size:7vw;\">Click <a href=\"/L\">here</a> turn the LED off<br></p>");

    // The HTTP response ends with another blank line:
    client.println();
 }

void response_WiFi_JSON(WiFiClient client){
  print("responding to JSON request..");
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:application/json");
  client.println();

  // the content of the HTTP response follows the header:
  clientPrintAllData(client);
  
  // The HTTP response ends with another blank line:
  client.println();
}

void response_WiFi_Timers(WiFiClient client){
  print("responding to Timers request..");
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:application/json");
  client.println();

  // the content of the HTTP response follows the header:
  JSONVar  timersObject = getRemainingTime();
  client.println(timersObject);
  
  // The HTTP response ends with another blank line:
  client.println();
}

