void forceLinksDraaien() {
   rechtsDraaien_FORCE = false;
   if (rechtsDraaien) {
      deactivate_Draaien();
   }
   if (!linksDraaien_activate()) {
      deactivate_Draaien();
   };
}

bool needToTurnLeft() {
   if (ignoreLightSensors || forceIgnoreLightSensors) {
      return false;
   }
   if (lichtSensors_Links > (lichtSensors_Rechts + licht_marge)) {
      linksDraaien_Sensors = true;
      rechtsDraaien_Sensors = false;
      return true;
   }
   linksDraaien_Sensors = false;
   return false;
}

bool linksDraaien_activate() {
   if (SAFE_MODE) {
      return false;
   }
   if (STOP_MODE && !linksDraaien_FORCE) {
      // print("STOP_MODE activated");
      return false;
   }

   if (draaienTooLong && !linksDraaien_FORCE) {
      // print("Draaien in ALARM - RETURN");
      return false;
   }

   if (antiPendel_Draaien && !linksDraaien_FORCE) {
      // print("ANTI-PENDEL Draaien ACTIVE - RETURN");
      return false;
   }

   if (einde_Linksdraaien) {
      // print("Eindeloop LINKS activated");
      return false;
   };

   // print("TURN LEFT");
   linksDraaien = true;
   einde_Rechtsdraaien = false;
   set_draaienTimeOut();

   if (turnStartTime == 0) {
      turnStartTime = millis();
   }
   return true;
}

void goToLeftEnd(bool includeTurnRight = true) {
   if (draaienTooLong) return;
   unsigned long turnTime = millis();

   linksDraaien = true;
   einde_Rechtsdraaien = false;
   digitalWrite(PIN_LinksDraaien, true);
   while (!digitalRead(PIN_Einde_Linksdraaien)) {
      delay(100);

      //SAFETY CHECK
      if (movementTooLong(turnTime)) {
         draaienTooLong = true;
         digitalWrite(PIN_LinksDraaien, false);
         deactivate_Draaien();
         return;
      }
   }
   linksDraaien = false;
   digitalWrite(PIN_LinksDraaien, false);
   einde_Linksdraaien = true;
   currentTurnPercentage = 0;

   if (includeTurnRight) {
      turnRightWhenEindeLoopLeft();
   }
}

void turnRightWhenEindeLoopLeft() {
   digitalWrite(PIN_LinksDraaien, false);
   delay(1000);
   digitalWrite(PIN_RechtsDraaien, !true);
   unsigned long turnTime = millis();
   while (digitalRead(PIN_Einde_Linksdraaien)) {
      delay(100);

      //SAFETY CHECK
      if (movementTooLong(turnTime)) {
         draaienTooLong = true;
         digitalWrite(PIN_RechtsDraaien, !false);
         deactivate_Draaien();
         return;
      }

   }
   digitalWrite(PIN_RechtsDraaien, !false);
}
