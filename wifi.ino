void wifi_Setup(){
  Serial.println("Creating Web Server...");

     // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    return;
  }

  WiFi.config(IPAddress(192,168,0,111)); //set IP
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED ) {
    if (attempts == 2){
      Serial.println("WiFi connection FAILED");
      return;
    }
    WiFi.begin(ssid, pass);
    delay(10000)  ;
    attempts++;
  }
  wifiConnected = true;
  server.begin(); 
  print_WifiData() ;
}

void wiFiLoop(){
  mqttClient.poll();
  WiFiClient client = server.available();
  if (client) { 
    clientConnection(client);
  }
  if(WiFi.status() != WL_CONNECTED){
    wifiConnected = false;
  }
}

void print_WifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the SSID of the network you're attached to:
  Serial.print("Network name: ");
  Serial.println(WiFi.SSID());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);
  Serial.println();

}

void clientConnection(WiFiClient client){
  Serial.println("new client");           // print a message out the serial port
  String currentLine = "";                // make a String to hold incoming data from the client
  bool contentLengthHeader = false;
  int contentLength;
  while (client.connected()) {            // loop while the client's connected
    if (!client.available())  continue;  // if there's bytes to read from the client,                                         
    char c = client.read();             // read a byte, then
    Serial.write(c);                    // print it out to the serial monitor
    if (c == '\n') {                    // if the byte is a newline character
      // if the current line is blank, you got two newline characters in a row.
      // that's the end of the client HTTP request, so send a response:
      if (currentLine.length() == 0) {
        readBody(client, contentLength);
        response_WiFi(client);
        break;
      } else {    // if you got a newline, then clear currentLine:
        if (contentLengthHeader){
          contentLength = currentLine.substring(16).toInt();
          contentLengthHeader = false;
        }
        currentLine = "";
      }
    } else if (c != '\r') {  // if you got anything else but a carriage return character,
      currentLine += c;      // add it to the end of the currentLine
      if (currentLine == "Content-Length: "){
        contentLengthHeader = true;
      }
    }

    // Check to see if the client request was "GET /H" or "GET /L":
    //HIER MOET MIJN CODE VOOR SPECIFIEKE INFO OP TE VRAGEN
    //OF VOOR COMMANDO'S TE GEVEN (example: Go TO Safe-mode)
    // SET FLAGS? Print shit wanneer einde van request
    if (currentLine.endsWith("GET /H")) {
      digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
    }
    // Andere opties...
    //----
    //----
    if (currentLine.endsWith("GET /L")) {
      digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
    }
  }
  // close the connection:
  client.stop();
  Serial.println("client disconnected");
}

void readBody(WiFiClient client, int contentLength){
  // Example curl http://192.48.56.2/ -d "{\"hallo\":\"no\"}"
  String requestBody;
  if (!contentLength || contentLength < 1) {
    return;
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
}

void response_WiFi(WiFiClient client){
  print("responding to request..");
 // Making a JSON Object
  Serial.println(myObject);

  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:application/json");
  client.println();

  // the content of the HTTP response follows the header:
  client.println(myObject);
  
  // The HTTP response ends with another blank line:
  client.println();
}

void mqtt_Setup(){

  const char broker[] = "192.168.0.205";
  int port = 49111;

 if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
  } else {
    Serial.println("You're connected to the MQTT broker!");
  };
  Serial.println();
};

bool retryConnection(void *){
  if(wifiConnected == false){
    stopMomevement()
    wifi_Setup();
  }
  if(mqttConnected == false){
    mqtt_Setup();
  }
  return true;
}