void forceRechtsDraaien() {
   linksDraaien_FORCE = false;
   if (linksDraaien == true) {
      deactivate_Draaien();
   }
   if (!rechtsDraaien_activate()) {
      deactivate_Draaien();
   }
}

bool needToTurnRight() {
   if (lichtSensors_Rechts > (lichtSensors_Links + licht_marge)) {
      rechtsDraaien_Sensors = true;
      linksDraaien_Sensors = false;
      return true;
   }
   rechtsDraaien_Sensors = false;
   return false;
}

bool rechtsDraaien_activate() {
   if (SAFE_MODE) {
      return false;
   }

   if (draaienTooLong && !rechtsDraaien_FORCE) {
      // print("Draaien in ALARM - RETURN");
      return false;
   }

   if (antiPendel_Draaien && !rechtsDraaien_FORCE) {
      // print("ANTI-PENDEL Draaien ACTIVE - RETURN");
      return false;
   }

   if (einde_Rechtsdraaien) {
      // print("EindeLoop RECHTS activated");
      return false;
   };

   if (STOP_MODE && !rechtsDraaien_FORCE) {
      // print("STOP_MODE activated");
      return false;
   }

   // print("TURN RIGHT - After checks");
   rechtsDraaien = true;
   einde_Linksdraaien = false;
   set_draaienTimeOut();

   if (turnStartTime == 0) {
      turnStartTime = millis();
   }
   return true;
}

void goToRightEnd(bool includeTurnLeft = true) {
   print("goToRightEnd");
   turnStartTime = millis();

   while (!digitalRead(PIN_Einde_Rechtsdraaien)) {
      rechtsDraaien = true;
      einde_Linksdraaien = false;
      digitalWrite(PIN_RechtsDraaien, !true);
      delay(100);
   }

   rechtsDraaien = false;
   digitalWrite(PIN_RechtsDraaien, !false);
   einde_Rechtsdraaien = true;
   currentTurnPercentage = 10000;

   if (includeTurnLeft) {
      turnLeftWhenEindeLoopRight();
   }
}

void turnLeftWhenEindeLoopRight() {
   digitalWrite(PIN_RechtsDraaien, !false);
   delay(1000);
   digitalWrite(PIN_LinksDraaien, true);
   while (digitalRead(PIN_Einde_Rechtsdraaien)) {
      delay(100);
   }
   digitalWrite(PIN_LinksDraaien, false);
}
