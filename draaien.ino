
void gotoTurnPercentage(int percentage = -1) {
   if (percentage != -1) {
      // when not called from loop
      gotoTurnPosition = true;
      wantedTurnPercentage = percentage;
   }

   if ((wantedTurnPercentage) > (currentTurnPercentage + 200)) {
      if (linksDraaien_FORCE) {
         deactivate_Draaien();
         linksDraaien_FORCE = false;
      }
      rechtsDraaien_FORCE = true;
      return;

   }
   else if ((wantedTurnPercentage) < (currentTurnPercentage - 200)) {
      if (rechtsDraaien_FORCE) {
         deactivate_Draaien();
         rechtsDraaien_FORCE = false;
      }
      linksDraaien_FORCE = true;
      return;
   }

   deactivate_Draaien();
   linksDraaien_FORCE = false;
   rechtsDraaien_FORCE = false;
   gotoTurnPosition = false;
}

void setDraaien() {
   // DRAAIEN //
   if (gotoTurnPosition) {
      gotoTurnPercentage();
   }
   if (draaienForceMode()) return;

   if (NIGHT_MODE || STOP_MODE || SAFE_MODE) {
      deactivate_Draaien();
      return;
   }

   draaienNormalMode();
}

void draaienNormalMode() {
   if (needToTurnLeft()) {
      if (rechtsDraaien) {
         deactivate_Draaien();
      }
      linksDraaien_activate();
   }
   else if (needToTurnRight()) {
      if (linksDraaien) {
         deactivate_Draaien();
      }
      rechtsDraaien_activate();
   }
   else {
      deactivate_Draaien();
   }
}

bool draaienForceMode() {
   if (rechtsDraaien_FORCE) {
      forceRechtsDraaien();
      return true;
   }
   if (linksDraaien_FORCE) {
      forceLinksDraaien();
      return true;
   }
   return false;
}

void deactivate_Draaien() {
   setCurrentTurnPercentage();
   turnStartTime = 0;
   if (linksDraaien || rechtsDraaien) {
      set_antiPendel_Draaien();
   }
   linksDraaien = false;
   rechtsDraaien = false;
   draaien_TimeOut.cancel();
}

void setCurrentTurnPercentage() {
   if (linksDraaien) {
      const unsigned int percentageTurned =
         getPercentageTurned();  // moet nog delen door 100 = 2 decimalen
      if (percentageTurned > currentTurnPercentage) {
         currentTurnPercentage = 0;
      }
      else {
         currentTurnPercentage -= percentageTurned;
      }

   }
   else if (rechtsDraaien) {
      const unsigned int percentageTurned =
         getPercentageTurned();  // moet nog delen door 100 = 2 decimalen
      if (percentageTurned + currentTurnPercentage > 11000) {
         currentTurnPercentage = 10000;
      }
      else {
         currentTurnPercentage += percentageTurned;
      }
   }
   else {
      return;
   }

   // start from current time again
   turnStartTime = millis();
}

int getPercentageTurned() {
   const unsigned long timeDif = millis() - turnStartTime;
   const float calculatedPercentage = (float)timeDif / (float)timeNeededToTurn;
   return calculatedPercentage * 10000.0;
}

void set_draaienTimeOut() {
   draaien_TimeOut.in(maxMovementTime, draaienTimeOutAlarm);
}

void set_antiPendel_Draaien() {
   antiPendel_Draaien = true;
   antiPendel_Draaien_Timer.in(antiPendelTime, reset_antiPendel_Draaien);
}

bool reset_antiPendel_Draaien(void*) {
   antiPendel_Draaien = false;
   return false;
}

void read_EindeLoop_Draaien() {

   if (linksDraaien || !einde_Linksdraaien) {
      einde_Linksdraaien = digitalRead(PIN_Einde_Linksdraaien);
      if (einde_Linksdraaien) {
         deactivate_Draaien();
         currentTurnPercentage = 0;
         turnRightWhenEindeLoopLeft();
         return;
      }
   }

   if (rechtsDraaien || !einde_Rechtsdraaien) {
      einde_Rechtsdraaien = digitalRead(PIN_Einde_Rechtsdraaien);

      if (einde_Rechtsdraaien) {
         deactivate_Draaien();
         currentTurnPercentage = 10000;
         turnLeftWhenEindeLoopRight();
         return;
      }
   }
}

bool draaienTimeOutAlarm(void*) {
   print("ALARM: draaien TimeOut!");
   draaienTooLong = true;
   deactivate_Draaien();
   return false;
}

