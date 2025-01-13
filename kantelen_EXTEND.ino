
void forceExtend() {
   inschuiven_FORCE = false;
   if (inschuiven) {
      deactivate_Kantelen();
   }
   uitschuiven_activate();
}

bool needToExtend() {
   if (lichtSensors_Boven > (lichtSensors_Onder + licht_marge)) {
      uitschuiven_Sensors = true;
      inschuiven_Sensors = false;
      return true;
   }
   uitschuiven_Sensors = false;
   return false;
}

void uitschuiven_activate() {
   if (kantelenTooLong && !uitschuiven_FORCE) {
      // print("Kantelen in ALARM - RETURN");
      return;
   }

   if (antiPendel_Kantelen && !uitschuiven_FORCE) {
      // print("ANTI-PENDEL kantelen ACTIVE - RETURN");
      return;
   }

   if (einde_Uitschuiven) {
      // print("Eindeloop UITSCHUIVEN activated");
      uitschuiven = false;
      return;
   };

   if (STOP_MODE && !uitschuiven_FORCE) {
      // print("STOP_MODE activated");
      uitschuiven = false;
      return;
   }

   // print("EXTEND");
   uitschuiven = true;
   einde_Inschuiven = false;
   set_kantelenTimeOut();

   if (tiltStartTime == 0) {
      tiltStartTime = millis();
   }
}

void goToTopEnd(bool includeInschuiven = true) {
   tiltStartTime = millis();
   while (!digitalRead(PIN_Einde_Uitschuiven)) {
      uitschuiven = true;
      einde_Inschuiven = false;
      digitalWrite(PIN_Uitschuiven, !true);
      delay(100);
   }
   uitschuiven = false;
   digitalWrite(PIN_Uitschuiven, !false);
   einde_Uitschuiven = true;
   currentTiltPercentage = 100;

   if (includeInschuiven) {
      inschuivenWhenEindeLoopUitschuiven();
   }
}

void inschuivenWhenEindeLoopUitschuiven() {
   digitalWrite(PIN_Uitschuiven, !false);
   delay(1000);
   digitalWrite(PIN_Inschuiven, !true);
   while (digitalRead(PIN_Einde_Uitschuiven)) {
      delay(100);
   }
   digitalWrite(PIN_Inschuiven, !false);
}
