void testMode() {
   while (true) {
      testMode_LichtSensoren();
      delay(1000);
   }
}

void testMode_Output() {
   // linksDraaien = true;
   // digitalWrite(PIN_LinksDraaien, linksDraaien);
   // Serial.println("Links draaien");
   // delay(5000);
   // linksDraaien = false;
   // digitalWrite(PIN_LinksDraaien, linksDraaien);

   // delay(1000);

   rechtsDraaien = true;
   digitalWrite(PIN_RechtsDraaien, rechtsDraaien);
   Serial.println("Rechts draaien");
   delay(5000);
   rechtsDraaien = false;
   digitalWrite(PIN_RechtsDraaien, rechtsDraaien);

   delay(1000);

   // uitschuiven = true;
   // digitalWrite(PIN_Uitschuiven, uitschuiven);
   // Serial.println("Uitschuiven");
   // delay(1000);
   // uitschuiven = false;
   // digitalWrite(PIN_Uitschuiven, uitschuiven);

   // delay(200);

   // inschuiven = true;
   // digitalWrite(PIN_Inschuiven, inschuiven);
   // Serial.println("Inschuiven");
   // delay(1000);
   // inschuiven = false;
   // digitalWrite(PIN_Inschuiven, inschuiven);
}

void testMode_LichtSensoren() {
   lichtSensor_LB = analogRead(PIN_lichtSensor_LB) + lichtSensor_LB_offset;
   lichtSensor_RB = analogRead(PIN_lichtSensor_RB) + lichtSensor_RB_offset;
   lichtSensor_LO = analogRead(PIN_lichtSensor_LO) + lichtSensor_LO_offset;
   lichtSensor_RO = analogRead(PIN_lichtSensor_RO) + lichtSensor_RO_offset;
   lichtSensors_Links = lichtSensor_LB + lichtSensor_LO;
   lichtSensors_Rechts = lichtSensor_RB + lichtSensor_RO;
   lichtSensors_Boven = lichtSensor_LB + lichtSensor_RB;
   lichtSensors_Onder = lichtSensor_LO + lichtSensor_RO;

   Serial.print("lichtSensor_LB: ");
   Serial.println(lichtSensor_LB);
   Serial.print("lichtSensor_RB: ");
   Serial.println(lichtSensor_RB);
   Serial.print("lichtSensor_LO: ");
   Serial.println(lichtSensor_LO);
   Serial.print("lichtSensor_RO: ");
   Serial.println(lichtSensor_RO);
   Serial.print("lichtSensors_Links: ");
   Serial.println(lichtSensors_Links);
   Serial.print("lichtSensors_Rechts: ");
   Serial.println(lichtSensors_Rechts);
   Serial.print("lichtSensors_Boven: ");
   Serial.println(lichtSensors_Boven);
   Serial.print("lichtSensors_Onder: ");
   Serial.println(lichtSensors_Onder);
}

void testMode_EindeLoop() {
   einde_Linksdraaien = digitalRead(PIN_Einde_Linksdraaien);
   einde_Rechtsdraaien = digitalRead(PIN_Einde_Rechtsdraaien);
   einde_Uitschuiven = digitalRead(PIN_Einde_Uitschuiven);
   einde_Inschuiven = digitalRead(PIN_Einde_Inschuiven);

   Serial.print("Einde_Linksdraaien: ");
   Serial.println(einde_Linksdraaien);
   Serial.print("Einde_Rechtsdraaien: ");
   Serial.println(einde_Rechtsdraaien);
   Serial.print("Einde_Uitschuiven: ");
   Serial.println(einde_Uitschuiven);
   Serial.print("Einde_Inschuiven: ");
   Serial.println(einde_Inschuiven);
}

void testMode_ForceSignals() {
   linksDraaien_FORCE = digitalRead(PIN_Force_Linksdraaien);
   rechtsDraaien_FORCE = digitalRead(PIN_Force_Rechtsdraaien);
   uitschuiven_FORCE = digitalRead(PIN_Force_Uitschuiven);
   inschuiven_FORCE = digitalRead(PIN_Force_Inschuiven);

   Serial.print("LinksDraaien_FORCE: ");
   Serial.println(linksDraaien_FORCE);
   Serial.print("RechtsDraaien_FORCE: ");
   Serial.println(rechtsDraaien_FORCE);
   Serial.print("Uitschuiven_FORCE: ");
   Serial.println(uitschuiven_FORCE);
   Serial.print("Inschuiven_FORCE: ");
   Serial.println(inschuiven_FORCE);
}

void initializeNeededTime() {
   print("Initialiseer benodigde tijd");
   getTimeDraaien();
   getTimeKantelen();
   print("Benodigde tijd geinitialiseerd");
}

void getTimeDraaien() {
   goToRightEnd();
   const unsigned long startingTime = millis();
   goToLeftEnd(false);
   const unsigned long endingTime = millis();
   timeNeededToTurn = endingTime - startingTime;
   turnRightWhenEindeLoopLeft();
}

void getTimeKantelen() {
   goToTopEnd();
   const unsigned long startingTime = millis();
   goToBottomEnd(false);
   const unsigned long endingTime = millis();
   timeNeededToTilt = endingTime - startingTime;
   uitschuivenWhenEindeLoopInschuiven();
}