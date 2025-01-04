void forceLinksDraaien() {
   rechtsDraaien_FORCE = false;
   if (rechtsDraaien) {
      deactivate_Draaien();
   }
   linksDraaien_activate();
}

bool needToTurnLeft() {
   if (lichtSensors_Links > lichtSensors_Rechts + licht_marge) {
      linksDraaien_Sensors = true;
      return true;
   }
   linksDraaien_Sensors = false;
   return false;
}

void linksDraaien_activate() {
   if (draaienTooLong && !linksDraaien_FORCE) {
      print("Draaien in ALARM - RETURN");
      return;
   }

   if (antiPendel_Draaien && !linksDraaien_FORCE) {
      print("ANTI-PENDEL Draaien ACTIVE - RETURN");
      return;
   }

   if (einde_Linksdraaien) {
      print("Eindeloop LINKS activated");
      linksDraaien = false;
      return;
   };

   if (STOP_MODE && !linksDraaien_FORCE) {
      print("STOP_MODE activated");
      linksDraaien = false;
      return;
   }

   print("TURN LEFT");
   linksDraaien = true;
   einde_Rechtsdraaien = false;
   set_draaienTimeOut();

   if (turnStartTime == 0) {
      turnStartTime = millis();
   }
}

void goToLeftEnd(bool includeTurnRight = true) {
   turnStartTime = millis();
   while (!digitalRead(PIN_Einde_Linksdraaien)) {
      linksDraaien = true;
      einde_Rechtsdraaien = false;
      digitalWrite(PIN_LinksDraaien, true);
      delay(100);
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
   while (digitalRead(PIN_Einde_Linksdraaien)) {
      delay(100);
   }
   digitalWrite(PIN_RechtsDraaien, !false);
}
