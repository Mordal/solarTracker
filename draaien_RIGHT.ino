void forceRechtsDraaien() {
   linksDraaien_FORCE = false;
   if (linksDraaien == true) {
      deactivate_Draaien();
   }
   rechtsDraaien_activate();
}

bool needToTurnRight() {
   if (lichtSensors_Rechts > lichtSensors_Links + licht_marge) {
      rechtsDraaien_Sensors = true;
      return true;
   }
   rechtsDraaien_Sensors = false;
   return false;
}

void rechtsDraaien_activate() {
   if (draaienTooLong && !rechtsDraaien_FORCE) {
      print("Draaien in ALARM - RETURN");
      return;
   }

   if (antiPendel_Draaien && !rechtsDraaien_FORCE) {
      print("ANTI-PENDEL Draaien ACTIVE - RETURN");
      return;
   }

   if (einde_Rechtsdraaien) {
      print("EindeLoop RECHTS activated");
      rechtsDraaien = false;
      return;
   };

   if (STOP_MODE && !rechtsDraaien_FORCE) {
      print("STOP_MODE activated");
      rechtsDraaien = false;
      return;
   }

   print("TURN RIGHT");
   rechtsDraaien = true;
   einde_Linksdraaien = false;
   set_draaienTimeOut();

   if (turnStartTime = 0) {
      turnStartTime = millis();
   }
}

void goToRightEnd(bool includeTurnLeft = true) {
  print("goToRightEnd");
   turnStartTime = millis();
   print("PIN_Einde_Rechts:");
   print(digitalRead(PIN_Einde_Rechtsdraaien));
   while (!digitalRead(PIN_Einde_Rechtsdraaien)) {
      rechtsDraaien = true;
      digitalWrite(PIN_RechtsDraaien, true);
      delay(100);
   }
   print("zou helemaal rechts moeten zijn..");
   print(digitalRead(PIN_Einde_Rechtsdraaien));
   rechtsDraaien = false;
   digitalWrite(PIN_RechtsDraaien, false);
   einde_Rechtsdraaien = true;
   currentTurnPercentage = 100;

   if (includeTurnLeft) {
      turnLeftWhenEindeLoopRight();
   }
}

void turnLeftWhenEindeLoopRight() {
   digitalWrite(PIN_RechtsDraaien, false);
   delay(1000);
   digitalWrite(PIN_LinksDraaien, true);
   while (digitalRead(PIN_Einde_Rechtsdraaien)) {
      delay(100);
   }
   digitalWrite(PIN_LinksDraaien, false);
}
