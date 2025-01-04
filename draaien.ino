

void gotoTurnPercentage(int percentage = -1) {
   if (percentage != -1) {
      gotoTurnPosition = true;
      wantedTurnPercentage = percentage;
   }
   if ((wantedTurnPercentage * 100) > currentTurnPercentage + 200) {
      rechtsDraaien_FORCE = true;
   } else if ((wantedTurnPercentage * 100) < currentTurnPercentage - 200) {
      linksDraaien_FORCE = true;
   } else {
      linksDraaien_FORCE = false;
      rechtsDraaien_FORCE = false;
      gotoTurnPosition = false;
   }
}

void setDraaien() {
   // DRAAIEN //
   if (gotoTurnPosition) {
      gotoTurnPercentage();
   }
   if (draaienForceMode()) {
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
   } else if (needToTurnRight()) {
      if (linksDraaien) {
         deactivate_Draaien();
      }
      rechtsDraaien_activate();
   } else {
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
   setCurrentTurnPercentage(nullptr);
   turnStartTime = 0;
   if (linksDraaien || rechtsDraaien) {
      set_antiPendel_Draaien();
   }
   linksDraaien = false;
   rechtsDraaien = false;
   draaien_TimeOut.cancel();
   Serial.println("Draaien deactivated");
   Serial.print("Current timeOutDraaien:");
   Serial.println(draaien_TimeOut_Remaining);
   Serial.print("Aantal Active Tasks timeOutKantelen: ");
   Serial.println(draaien_TimeOut.size());
}

// Timer: setTurnPercentageTimer
bool setCurrentTurnPercentage(void *) {
   const unsigned int percentageTurned =
       getPercentageTurned();  // moet nog delen door 100 = 2 decimalen
   if (linksDraaien) {
      currentTurnPercentage = currentTurnPercentage - percentageTurned;
   } else if (rechtsDraaien) {
      currentTurnPercentage = currentTurnPercentage + percentageTurned;
   } else {
      return true;
   }

   const float floatPercentage = (float)currentTurnPercentage / 100.0;

   Serial.print("Current Turn Percentage: ");
   Serial.println(floatPercentage, 4);

   // start from current time again
   turnStartTime = millis();
   return true;
}

float getPercentageTurned() {
   const unsigned long timeDifference = millis() - turnStartTime;
   return ((float)timeDifference / (float)timeNeededToTurn) * 10000.0;
}

void set_draaienTimeOut() {
   draaien_TimeOut.in(maxMovementTime, draaienTimeOutAlarm);
}

void set_antiPendel_Draaien() {
   antiPendel_Draaien = true;
   antiPendel_Draaien_Timer.in(antiPendelTime, reset_antiPendel_Draaien);
}

bool reset_antiPendel_Draaien(void *) {
   antiPendel_Draaien = false;
   return false;
}

void read_EindeLoop_Draaien() {
   // print("read eindeloop Draaien");
   if (!einde_Linksdraaien) {
      // print("NOT einde LINKS");
      einde_Linksdraaien = digitalRead(PIN_Einde_Linksdraaien);
      if (einde_Linksdraaien) {
         // print("IS einde LINKS");
         linksDraaien = false;
         currentTurnPercentage = 0;
         turnRightWhenEindeLoopLeft();
      }
   }
   if (!einde_Rechtsdraaien) {
      // print("NOT einde RECHTS");
      einde_Rechtsdraaien = digitalRead(PIN_Einde_Rechtsdraaien);
      if (einde_Rechtsdraaien) {
         // print("IS einde RECHTS");
         rechtsDraaien = false;
         currentTurnPercentage = 10000;
         turnLeftWhenEindeLoopRight();
      }
   }
}

bool draaienTimeOutAlarm(void *) {
   print("ALARM: draaien TimeOut!");
   draaienTooLong = true;
   deactivate_Draaien();
   return false;
}
