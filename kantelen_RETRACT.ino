
void forceRetract() {
   uitschuiven_FORCE = false;
   if (uitschuiven) {
      deactivate_Kantelen();
   }
   if (!inschuiven_activate()) {
      deactivate_Kantelen();
   };
}

bool needToRetract() {
   if (ignoreLightSensors || forceIgnoreLightSensors) {
      uitschuiven_Sensors = false;
      inschuiven_Sensors = false;
      return false;
   }
   if (lichtSensors_Onder > (lichtSensors_Boven + licht_marge)) {
      inschuiven_Sensors = true;
      uitschuiven_Sensors = false;
      return true;
   }
   inschuiven_Sensors = false;
   return false;
}

bool inschuiven_activate() {
   if (SAFE_MODE) {
      return false;
   }
   if (STOP_MODE && !inschuiven_FORCE) {
      return false;
   }
   if (kantelenTooLong && !inschuiven_FORCE) {
      return false;
   }

   if (antiPendel_Kantelen && !inschuiven_FORCE) {
      return false;
   }

   if (einde_Inschuiven) {
      return false;
   };

   inschuiven = true;
   einde_Uitschuiven = false;
   set_kantelenTimeOut();

   if (tiltStartTime == 0) {
      tiltStartTime = millis();
   }
   return true;
}

void goToRetractEnd(bool includeUitschuiven = true) {
   if (kantelenTooLong) return;
   unsigned long tiltTime = millis();
   while (!digitalRead(PIN_Einde_Inschuiven)) {
      inschuiven = true;
      einde_Uitschuiven = false;
      digitalWrite(PIN_Inschuiven, !true);
      delay(100);

      //SAFETY CHECK
      if (movementTooLong(tiltTime)) {
         kantelenTooLong = true;
         digitalWrite(PIN_Inschuiven, !false);
         deactivate_Kantelen();
         return;
      }

   }
   inschuiven = false;
   digitalWrite(PIN_Inschuiven, !false);
   einde_Inschuiven = true;
   currentTiltPercentage = 0;

   if (includeUitschuiven) {
      uitschuivenWhenEindeLoopInschuiven();
   }
}

void uitschuivenWhenEindeLoopInschuiven() {
   digitalWrite(PIN_Inschuiven, !false);
   delay(1000);
   digitalWrite(PIN_Uitschuiven, !true);
   unsigned long tiltTime = millis();

   while (digitalRead(PIN_Einde_Inschuiven)) {
      delay(100);

      //SAFETY CHECK
      if (movementTooLong(tiltTime)) {
         kantelenTooLong = true;
         digitalWrite(PIN_Uitschuiven, !false);
         deactivate_Kantelen();
         return;
      }

   }
   digitalWrite(PIN_Uitschuiven, !false);
}
