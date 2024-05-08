
#include "Arduino_LED_Matrix.h" // Include the LED_Matrix library
#include "WiFiS3.h"
#include "wifi_secrets.h"
#include "arduino-timer.h"
#include "Arduino_JSON.h"
#include "ArduinoMqttClient.h"


//Alle variabelen
  ArduinoLEDMatrix matrix;
  byte ledMatrix[8][12]{
    { 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0 },
    { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0 },
    { 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  };
//Lichtsensors
  const int PIN_lichtSensor_LB = A0; //INPUT
  const int PIN_lichtSensor_RB = A1; //INPUT
  const int PIN_lichtSensor_LO = A2; //INPUT
  const int PIN_lichtSensor_RO = A3; //INPUT
  int lichtSensor_LB = 0;  //from 0 - 1023
  int lichtSensor_RB = 0;  //from 0 - 1023
  int lichtSensor_LO = 0;  //from 0 - 1023
  int lichtSensor_RO = 0;  //from 0 - 1023
  int licht_marge = 10;

//Draaien
  const int PIN_LinksDraaien = 2; //OUTPUT
  const int PIN_RechtsDraaien = 3; //OUTPUT
  const int PIN_Einde_Linksdraaien = 4; //OUTPUT
  const int PIN_Einde_Rechtsdraaien = 5; //OUTPUT
  bool linksDraaien = false;
  bool linksDraaien_FORCE = false;
  bool rechtsDraaien = false;
  bool rechtsDraaien_FORCE = false;
  bool einde_Linksdraaien = false;
  bool einde_Rechtsdraaien = false ;
  bool antiPendel_Draaien = false;

//Kantelen
  const int PIN_Uitschuiven = 6; //OUTPUT
  const int PIN_Inschuiven = 7; //OUTPUT
  const int PIN_Einde_Uitschuiven = 8; //OUTPUT
  const int PIN_Einde_Inschuiven = 9;//OUTPUT
  bool uitschuiven = false;
  bool uitschuiven_FORCE = false;
  bool inschuiven = false;
  bool inschuiven_FORCE = false;
  bool einde_Uitschuiven = false;
  bool einde_Inschuiven = false;
  bool antiPendel_Kantelen = false;

//TimeOuts
  int antiPendelTime = 5000;  //5 min. = 300000 ms
  auto antiPendel_Draaien_Timer = timer_create_default();
  auto antiPendel_Kantelen_Timer = timer_create_default();

  int maxMovementTime = 30000;
  auto draaien_TimeOut = timer_create_default();
  auto kantelen_TimeOut = timer_create_default();

  auto logBook_Timer = timer_create_default();
  int retryTime = 300000;
  auto retryTimer = timer_create_default();
  

//Flags
  bool wifiConnected = false;
  bool mqttConnected = false;
  bool TEST_MODE = false;
  bool SAFE_MODE = false;
  bool draaienTooLong = false;
  bool kantelenTooLong = false;

//WIFI
  char ssid[] = SECRET_SSID;   
  char pass[] = SECRET_PASS;  
  int wifi_status = WL_IDLE_STATUS;
  WiFiServer server(80);
  WiFiClient wifiClient;
  MqttClient mqttClient(wifiClient);  

//JSON
  JSONVar myObject;



void setup() {

//declare OUTPUT PINS
  pinMode(PIN_LinksDraaien, OUTPUT);
  pinMode(PIN_RechtsDraaien, OUTPUT);
  pinMode(PIN_Uitschuiven, OUTPUT);
  pinMode(PIN_Inschuiven, OUTPUT);

//declare INPUT PINS
  pinMode(PIN_Einde_Linksdraaien, INPUT);
  pinMode(PIN_Einde_Rechtsdraaien, INPUT);
  pinMode(PIN_Einde_Uitschuiven, INPUT);
  pinMode(PIN_Einde_Inschuiven, INPUT);

//START INITIALIZE
  Serial.begin(9600);
  matrix.begin();
  printLedMatrix();

//setup WIFI
  
  logBook_Timer.every(10000, setLogbook); //10 sec ----- //every minute -> voor een week: 6 keer per uur
  wifi_Setup();
  mqtt_Setup();


//timer setup
  retryTimer.every(retryTime, retryConnection);
  
  
  print("Setup done!");
  printLedMatrix();

}

void loop() {
  tickTimers();
  readLichtSensors();
  readEindeloop();
  set_MoveDirection();
  set_Outputs();
  wiFiLoop();
  
}


void tickTimers(){
  antiPendel_Draaien_Timer.tick();
  antiPendel_Kantelen_Timer.tick();
  draaien_TimeOut.tick();
  kantelen_TimeOut.tick();
  logBook_Timer.tick();
  retryTimer.tick();
}


bool set_Outputs(){
  digitalWrite(PIN_LinksDraaien, linksDraaien);
  digitalWrite(PIN_RechtsDraaien, rechtsDraaien);
  digitalWrite(PIN_Uitschuiven, uitschuiven);
  digitalWrite(PIN_Inschuiven, inschuiven);
  myObject["TURN_LEFT"]["Moving"] = linksDraaien;
  myObject["TURN_RIGHT"]["Moving"] = rechtsDraaien;
  myObject["EXTEND"]["Moving"] = uitschuiven;
  myObject["RETRACT"]["Moving"] = inschuiven;

}


void print(const char *text){
  Serial.println(text);
}

void printLedMatrix(){
  matrix.renderBitmap(ledMatrix, 8, 12);
}




