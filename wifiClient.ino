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
  bool unlockRequest = false;  
  bool settingsRequest = false;
  int contentLength;
  while (client.connected()) {            // loop while the client's connected
    if (!client.available())  continue;  // if there's bytes to read from the client,                                         
    char c = client.read();             // read a byte, then
    // Serial.write(c);                    // print it out to the serial monitor
    if (c == '\n') {                    // if the byte is a newline character
      // if the current line is blank, you got two newline characters in a row.
      // that's the end of the client HTTP request 
      if (currentLine.length() == 0) {
        if (contentLength > 1 && contentLength < 1025) { // body lezen als er een Content-Length is
          String body = readBody(client, contentLength);

         if (unlockRequest) {
          unlockSettings(client, body);    // Passeer de body naar je unlock functie
        }

        if (settingsRequest) {
          setValues(client, body); 
        }

          // response_WiFi_BASIC(client);  //send a response:
        } ;
        break;
      } else {    // if you got a newline, then clear currentLine:
        if (contentLengthHeader){
          contentLength = currentLine.substring(16).toInt();
          contentLengthHeader = false;
        }
        // print(currentLine);
        currentLine = "";
      }
    } else if (c != '\r') {  // if you got anything else but a carriage return character,
      currentLine += c;      // add it to the end of the currentLine
      if (currentLine == "Content-Length: ") contentLengthHeader = true;
    }


    if (currentLine.indexOf("GET /API") == 0){
      while (client.available() && (c = client.read()) != '\n') {
        currentLine += c;
      }
      
      print(currentLine);
      response_API_Request(client, currentLine);
      break; // don't check for body
    }

    if (currentLine.endsWith("POST /SETTINGS")){
      settingsRequest = true; // Don't break, check for body
    }


    if (currentLine.endsWith("POST /UNLOCK")){
      unlockRequest = true;  // Don't break, check for body
    }

    // if (currentLine.endsWith("GET /API/")){
    //   response_WiFi_JSON(client);
    //   break;
    // }

    // if(currentLine.endsWith("GET /TIMERS")){
    //   response_WiFi_Timers(client);
    //   break;
    // }
  }
  // close the connection:
  client.stop();
  Serial.println("client disconnected");
}


void mqtt_Setup(){
  const char broker[] = "192.168.0.205";
  int port = 49111;

 if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
  } else {
    Serial.println("You're connected to the MQTT broker!");
    mqttConnected = true;
  };
  Serial.println();
};

bool retryConnection(void *){
  if(wifiConnected == false){
    stopMomevement();
    wifi_Setup();
  }
  if(mqttConnected == false){
    mqtt_Setup();
  }
  return true;
}