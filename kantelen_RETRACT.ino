
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

   if (kantelenTooLong && !inschuiven_FORCE) {
      return false;
   }

   if (antiPendel_Kantelen && !inschuiven_FORCE) {
      return false;
   }

   if (einde_Inschuiven) {
      return false;
   };

   if (STOP_MODE && !inschuiven_FORCE) {
      return false;
   }

   inschuiven = true;
   einde_Uitschuiven = false;
   set_kantelenTimeOut();

   if (tiltStartTime == 0) {
      tiltStartTime = millis();
   }
   return true;
}

void goToRetractEnd(bool includeUitschuiven = true) {
   tiltStartTime = millis();
   while (!digitalRead(PIN_Einde_Inschuiven)) {
      inschuiven = true;
      einde_Uitschuiven = false;
      digitalWrite(PIN_Inschuiven, !true);
      delay(100);
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
   while (digitalRead(PIN_Einde_Inschuiven)) {
      delay(100);
   }
   digitalWrite(PIN_Uitschuiven, !false);
}
