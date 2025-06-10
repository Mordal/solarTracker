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

   if (STOP_MODE && !rechtsDraaien_FORCE) {
      return false;
   }

   if (draaienTooLong && !rechtsDraaien_FORCE) {
      return false;
   }

   if (antiPendel_Draaien && !rechtsDraaien_FORCE) {
      return false;
   }

   if (einde_Rechtsdraaien) {
      return false;
   };

   rechtsDraaien = true;
   einde_Linksdraaien = false;
   set_draaienTimeOut();

   if (turnStartTime == 0) {
      turnStartTime = millis();
   }
   return true;
}

void goToRightEnd(bool includeTurnLeft = true) {
   if (draaienTooLong) return;
   turnStartTime = millis();

   while (!digitalRead(PIN_Einde_Rechtsdraaien)) {
      rechtsDraaien = true;
      einde_Linksdraaien = false;
      digitalWrite(PIN_RechtsDraaien, !true);
      delay(100);

      //SAFETY CHECK
      if (movementTooLong(turnStartTime)) {
         draaienTooLong = true;
         digitalWrite(PIN_RechtsDraaien, !false);
         deactivate_Draaien();
         return;
      }
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
   unsigned long turnTime = millis();
   while (digitalRead(PIN_Einde_Rechtsdraaien)) {
      delay(100);

      //SAFETY CHECK
      if (movementTooLong(turnTime)) {
         draaienTooLong = true;
         digitalWrite(PIN_LinksDraaien, false);
         deactivate_Draaien();
         return;
      }
   }
   digitalWrite(PIN_LinksDraaien, false);
}
