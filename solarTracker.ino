
// TODO

// isNight() -> als het donker is
// --> ga naar begin positie -> stop met draaien en kantelen

#include <NTPClient.h>
#include <RTC.h>
#include <TimeLib.h>
#include <WiFiUdp.h>

#include "ArduinoMqttClient.h"
#include "Arduino_JSON.h"
#include "WiFiS3.h"
#include "arduino-timer.h"
#include "wifi_secrets.h"

// Alle variabelen //

// LICHTSENSOREN //
const int PIN_lichtSensor_LB = A0;  // INPUT
const int PIN_lichtSensor_RB = A1;  // INPUT
const int PIN_lichtSensor_LO = A2;  // INPUT
const int PIN_lichtSensor_RO = A3;  // INPUT
int lichtSensor_LB = 0;             // from 0 - 1023
int lichtSensor_RB = 0;             // from 0 - 1023
int lichtSensor_LO = 0;             // from 0 - 1023
int lichtSensor_RO = 0;             // from 0 - 1023
int licht_marge = 10;

// offsets
int lichtSensor_LB_offset = 0;
int lichtSensor_RB_offset = 0;
int lichtSensor_LO_offset = 0;
int lichtSensor_RO_offset = 0;

// calculated values
int lichtSensors_Links = 0;
int lichtSensors_Rechts = 0;
int lichtSensors_Boven = 0;
int lichtSensors_Onder = 0;

// MOVEMENT //
// DRAAIEN //
// pins
const int PIN_LinksDraaien = 2;          // OUTPUT
const int PIN_RechtsDraaien = 3;         // OUTPUT
const int PIN_Einde_Linksdraaien = 4;    // INPUT
const int PIN_Einde_Rechtsdraaien = 5;   // INPUT
const int PIN_Force_Linksdraaien = 10;   // INPUT
const int PIN_Force_Rechtsdraaien = 11;  // INPUT

// turning activated
bool linksDraaien = false;
bool linksDraaien_FORCE = false;
bool rechtsDraaien = false;
bool rechtsDraaien_FORCE = false;
bool gotoTurnPosition = false;

// sensors
bool linksDraaien_Sensors = false;
bool rechtsDraaien_Sensors = false;

bool einde_Linksdraaien = false;
bool einde_Rechtsdraaien = false;

// antiPendel
bool antiPendel_Draaien = false;

// turning percentage
unsigned long timeNeededToTurn = 0;
unsigned int currentTurnPercentage =
    0;  // moet nog delen door 100 = 2 decimalen
unsigned long turnStartTime = 0;
byte wantedTurnPercentage = 0;

// KANTELEN //
// pins
const int PIN_Uitschuiven = 6;         // OUTPUT
const int PIN_Inschuiven = 7;          // OUTPUT
const int PIN_Einde_Uitschuiven = 8;   // INPUT
const int PIN_Einde_Inschuiven = 9;    // INPUT
const int PIN_Force_Uitschuiven = 12;  // INPUT
const int PIN_Force_Inschuiven = 13;   // INPUT

// kantelen activated
bool uitschuiven = false;
bool uitschuiven_FORCE = false;
bool inschuiven = false;
bool inschuiven_FORCE = false;
bool gotoTiltPosition = false;

// sensors
bool uitschuiven_Sensors = false;
bool inschuiven_Sensors = false;

bool einde_Uitschuiven = false;
bool einde_Inschuiven = false;

// antiPendel
bool antiPendel_Kantelen = false;

// tilt percentage
unsigned long timeNeededToTilt = 0;
unsigned int currentTiltPercentage =
    0;  // moet nog delen door 100 = 2 decimalen
unsigned long tiltStartTime = 0;
byte wantedTiltPercentage = 0;

// TIME-OUTS
// Settings
unsigned int antiPendelTime = 5000;        //= 5 sec. = 5000 ms  ;  MAX = 65535
unsigned int maxMovementTime = 45000;      // 45 sec. = 45000 ms
unsigned long retryTime = 300000;          // 5 min. = 300000 ms
unsigned int logBook_Timer_delay = 10000;  // 10 sec. = 10000 ms

// Draaien
Timer<1> antiPendel_Draaien_Timer;
Timer<1> draaien_TimeOut;
auto setTurnPercentageTimer = timer_create_default();

// Kantelen
Timer<1> antiPendel_Kantelen_Timer;
Timer<1> kantelen_TimeOut;
auto setTiltPercentageTimer = timer_create_default();

// Other Timers
auto logBook_Timer = timer_create_default();
auto retryTimer = timer_create_default();
auto settingsUnlockedTimer = timer_create_default();
auto gotoPositionTimer = timer_create_default();

// Time Remaining
unsigned int antiPendel_Draaien_Timer_Remaining = 0;
unsigned int antiPendel_Kantelen_Timer_Remaining = 0;
unsigned int draaien_TimeOut_Remaining = 0;
unsigned int kantelen_TimeOut_Remaining = 0;
unsigned int logBook_Timer_Remaining = 0;
unsigned long retryTimer_Remaining = 0;
unsigned long settingsUnlockedTimer_Remaining = 0;
unsigned long gotoPositionTimer_Remaining = 0;

// FLAGS
bool wifiConnected = false;
bool mqttConnected = false;
bool resetHappend = false;
bool TEST_MODE = false;
bool SAFE_MODE = false;
bool STOP = false;
bool draaienTooLong = false;
bool kantelenTooLong = false;
bool settingsUnlocked = false;

// WIFI CONFIG
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int wifi_status = WL_IDLE_STATUS;
WiFiServer server(80);
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

// INTERNET TIME - NTP client
WiFiUDP ntpUDP;
NTPClient timeClient(
    ntpUDP, "pool.ntp.org", 0,
    60000);  // 0 offset initially, we will calculate it dynamically
byte previousMonth = 0;

void setup() {
   resetHappend = true;
   // declare OUTPUT PINS
   pinMode(PIN_LinksDraaien, OUTPUT);
   pinMode(PIN_RechtsDraaien, OUTPUT);
   pinMode(PIN_Uitschuiven, OUTPUT);
   pinMode(PIN_Inschuiven, OUTPUT);

   // declare INPUT PINS
   pinMode(PIN_Einde_Linksdraaien, INPUT);
   pinMode(PIN_Einde_Rechtsdraaien, INPUT);
   pinMode(PIN_Einde_Uitschuiven, INPUT);
   pinMode(PIN_Einde_Inschuiven, INPUT);

   pinMode(PIN_Force_Linksdraaien, INPUT);
   pinMode(PIN_Force_Rechtsdraaien, INPUT);
   pinMode(PIN_Force_Uitschuiven, INPUT);
   pinMode(PIN_Force_Inschuiven, INPUT);

   // START INITIALIZE
   Serial.begin(9600);

   // setup WIFI
   wifi_Setup();
   mqtt_Setup();

   // setup RTC - internal clock
   RTC.begin();
   setTimeFromNet();
   delay(1000);  // Need to do 2 times to get the correct time
   setTimeFromNet();

   // Initialize needed time
   initializeNeededTime();

   // setup Timers
   setTimers();

   print("Setup done!");
}

void loop() {
   tickTimers();
   readLichtSensors();
   readEindeloop();
   readForceSignals();
   set_MoveDirection();
   set_Outputs();
   wiFiLoop();
}

void tickTimers() {
   antiPendel_Draaien_Timer_Remaining = antiPendel_Draaien_Timer.tick();
   antiPendel_Kantelen_Timer_Remaining = antiPendel_Kantelen_Timer.tick();
   draaien_TimeOut_Remaining = draaien_TimeOut.tick();
   kantelen_TimeOut_Remaining = kantelen_TimeOut.tick();
   logBook_Timer_Remaining = logBook_Timer.tick();
   retryTimer_Remaining = retryTimer.tick();
   setTurnPercentageTimer.tick();
   setTiltPercentageTimer.tick();
   settingsUnlockedTimer_Remaining = settingsUnlockedTimer.tick();
   gotoPositionTimer_Remaining = gotoPositionTimer.tick();
}

void set_Outputs() {
   digitalWrite(PIN_LinksDraaien, linksDraaien);
   digitalWrite(PIN_RechtsDraaien, rechtsDraaien);
   digitalWrite(PIN_Uitschuiven, uitschuiven);
   digitalWrite(PIN_Inschuiven, inschuiven);
}

void print(const char *text) { Serial.println(text); }

void print(const String text) { Serial.println(text); }

void print(JSONVar json) { Serial.println(json); }

// set permanent timers
void setTimers() {
   retryTimer.every(retryTime, retryConnection);
   logBook_Timer.every(logBook_Timer_delay,
                       setLogbook);  // 10 sec ----- //every minute -> voor een
                                     // week: 6 keer per uur
   setTurnPercentageTimer.every(1000, setCurrentTurnPercentage);  // 1 sec
   setTiltPercentageTimer.every(1000, setCurrentTiltPercentage);  // 1 sec
   gotoPositionTimer.every(3600000, gotoPresetPosition);          // 1 uur
}
