
//TODO
//voeg timer toe die telt hoe lang de beweging LINKS->RECHTS duurt
//voeg timer toe die telt hoe lang de beweging ONDER->BOVEN duurt

//voeg 2 variabelen toe die het percentage van de beweging bijhouden

//voeg functie toe die de eindeloop een klein beetje terugdraait maar wel de flag eindeloop behoudt

//Functie die een schatting maakt van verwachte positie obv de tijd
//vergelijk met effectieve positie en corrigeer indien nodig




//#include "Arduino_LED_Matrix.h" // Include the LED_Matrix library
#include "WiFiS3.h"
#include "wifi_secrets.h"
#include "arduino-timer.h"
#include "Arduino_JSON.h"
#include "ArduinoMqttClient.h"

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <RTC.h>

#include <util/atomic.h>


//Alle variabelen
  // ArduinoLEDMatrix matrix;
  // byte ledMatrix[8][12]{
  //   { 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0 },
  //   { 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0 },
  //   { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0 },
  //   { 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
  //   { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },
  //   { 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 },
  //   { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
  //   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  // };
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
  const int PIN_Einde_Linksdraaien = 4; //INPUT
  const int PIN_Einde_Rechtsdraaien = 5; //INPUT
  const int PIN_Force_Linksdraaien = 10; //INPUT
  const int PIN_Force_Rechtsdraaien = 11; //INPUT
  bool linksDraaien = false;
  bool linksDraaien_FORCE = false;
  bool rechtsDraaien = false;
  bool rechtsDraaien_FORCE = false;
  bool einde_Linksdraaien = false;
  bool einde_Rechtsdraaien = false ;
  bool antiPendel_Draaien = false;
  const unsigned long timeNeededToTurn = 0

//Kantelen
  const int PIN_Uitschuiven = 6; //OUTPUT
  const int PIN_Inschuiven = 7; //OUTPUT
  const int PIN_Einde_Uitschuiven = 8; //INPUT
  const int PIN_Einde_Inschuiven = 9;//INPUT
  const int PIN_Force_Uitschuiven = 12;//INPUT
  const int PIN_Force_Inschuiven = 13; //INPUT
  bool uitschuiven = false;
  bool uitschuiven_FORCE = false;
  bool inschuiven = false;
  bool inschuiven_FORCE = false;
  bool einde_Uitschuiven = false;
  bool einde_Inschuiven = false;
  bool antiPendel_Kantelen = false;
  const unsigned long timeNeededToTilt = 0

//TimeOuts
  int antiPendelTime = 5000;  //5 min. = 300000 ms
  auto antiPendel_Draaien_Timer = timer_create_default();
  auto antiPendel_Kantelen_Timer = timer_create_default();

  int maxMovementTime = 30000; //30 sec. = 30000 ms
  auto draaien_TimeOut = timer_create_default();
  auto kantelen_TimeOut = timer_create_default();

  int logBook_Timer_delay = 10000; //10 sec. = 10000 ms
  auto logBook_Timer = timer_create_default();
  int retryTime = 300000; //5 min. = 300000 ms
  auto retryTimer = timer_create_default();
  

  //Other Timers


// TimeRemaining
  int antiPendel_Draaien_Timer_Remaining = 0;
  int antiPendel_Kantelen_Timer_Remaining = 0;
  int draaien_TimeOut_Remaining = 0;
  int kantelen_TimeOut_Remaining = 0;
  int logBook_Timer_Remaining = 0;
  int retryTimer_Remaining = 0;


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

// Definieer NTP client to get time
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);  // 0 offset initially, we will calculate it dynamically


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
  
  pinMode(PIN_Force_Linksdraaien, INPUT);
  pinMode(PIN_Force_Rechtsdraaien, INPUT);
  pinMode(PIN_Force_Uitschuiven, INPUT);
  pinMode(PIN_Force_Inschuiven, INPUT);
  
//START INITIALIZE
  Serial.begin(9600);
  // matrix.begin();

//setup WIFI
  wifi_Setup();
  mqtt_Setup();


//setup RTC
  
  RTC.begin(); 

  setTimeFromNet();
  delay(1000); //Need to do 2 times to get the correct time
  setTimeFromNet();

  setTimers();

  print("Setup done!");
  // printLedMatrix();

}

void loop() {
  tickTimers();
  readLichtSensors();
  readEindeloop();
  readForceSignals();
  set_MoveDirection();
  set_Outputs();
  wiFiLoop();
  const test = millis()
}


void tickTimers(){
  antiPendel_Draaien_Timer_Remaining = antiPendel_Draaien_Timer.tick();
  antiPendel_Kantelen_Timer_Remaining = antiPendel_Kantelen_Timer.tick();
  draaien_TimeOut_Remaining = draaien_TimeOut.tick();
  kantelen_TimeOut_Remaining = kantelen_TimeOut.tick();
  logBook_Timer_Remaining = logBook_Timer.tick();
  retryTimer_Remaining = retryTimer.tick();
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

void print(const String text){
  Serial.println(text);
}

void print (JSONVar json){
  Serial.println(json);
}

//set permanent timers
void setTimers(){
  //timer setup
  retryTimer.every(retryTime, retryConnection);
  logBook_Timer.every(logBook_Timer_delay, setLogbook); //10 sec ----- //every minute -> voor een week: 6 keer per uur
}

// void printLedMatrix(){
//   matrix.renderBitmap(ledMatrix, 8, 12);
// }




