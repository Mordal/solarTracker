
void forceExtend() {
   inschuiven_FORCE = false;
   if (inschuiven) {
      deactivate_Kantelen();
   }

   if (!uitschuiven_activate()) {
      deactivate_Kantelen();
   };
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

bool uitschuiven_activate() {
   if (SAFE_MODE) {
      return false;
   }

   if (kantelenTooLong && !uitschuiven_FORCE) {
      // print("Kantelen in ALARM - RETURN");
      return false;
   }

   if (antiPendel_Kantelen && !uitschuiven_FORCE) {
      // print("ANTI-PENDEL kantelen ACTIVE - RETURN");
      return false;
   }

   if (einde_Uitschuiven) {
      // print("Eindeloop UITSCHUIVEN activated");
      return false;
   };

   if (STOP_MODE && !uitschuiven_FORCE) {
      // print("STOP_MODE activated");
      return false;
   }

   // print("EXTEND");
   uitschuiven = true;
   einde_Inschuiven = false;
   set_kantelenTimeOut();

   if (tiltStartTime == 0) {
      tiltStartTime = millis();
   }
   return true;
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
   currentTiltPercentage = 10000;

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
