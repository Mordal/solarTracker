
void gotoTiltPercentage(int percentage = -1) {
   if (percentage != -1) {
      // when not called from loop
      gotoTiltPosition = true;
      wantedTiltPercentage = percentage;
   }

   if ((wantedTiltPercentage) > (currentTiltPercentage + 200)) {
      if (inschuiven_FORCE) {
         deactivate_Kantelen();
         inschuiven_FORCE = false;
      }
      uitschuiven_FORCE = true;
      return;

   }
   else if ((wantedTiltPercentage) < (currentTiltPercentage - 200)) {
      if (uitschuiven_FORCE) {
         deactivate_Kantelen();
         uitschuiven_FORCE = false;
      }
      inschuiven_FORCE = true;
      return;
   }

   deactivate_Kantelen();
   uitschuiven_FORCE = false;
   inschuiven_FORCE = false;
   gotoTiltPosition = false;
}

void setKantelen() {
   if (gotoTiltPosition) {
      gotoTiltPercentage();
   }
   if (kantelenForceMode()) return;

   if (NIGHT_MODE) {
      deactivate_Kantelen();
      return;
   }

   if (STOP_MODE) {
      deactivate_Kantelen();
      return;
   }

   kantelenNormalMode();
}

void kantelenNormalMode() {
   if (needToExtend()) {
      if (inschuiven == true) {
         deactivate_Kantelen();
      }
      uitschuiven_activate();
   }
   else if (needToRetract()) {
      if (uitschuiven) {
         deactivate_Kantelen();
      }
      inschuiven_activate();
   }
   else {
      deactivate_Kantelen();
   }
}

bool kantelenForceMode() {
   if (uitschuiven_FORCE) {
      forceExtend();
      return true;
   }
   if (inschuiven_FORCE) {
      forceRetract();
      return true;
   }
   return false;
}

void deactivate_Kantelen() {
   setCurrentTiltPercentage();
   tiltStartTime = 0;
   if (uitschuiven || inschuiven) {
      set_antiPendel_Kantelen();
   }
   uitschuiven = false;
   inschuiven = false;
   kantelen_TimeOut.cancel();
}

void setCurrentTiltPercentage() {
   if (inschuiven) {
      const unsigned int percentageTilted =
         getPercentageTilted();  // moet nog delen door 100 = 2 decimalen
      if (percentageTilted > currentTiltPercentage) {
         currentTiltPercentage = 0;
      }
      else {
         currentTiltPercentage -= percentageTilted;
      }

   }
   else if (uitschuiven) {
      const unsigned int percentageTilted =
         getPercentageTilted();  // moet nog delen door 100 = 2 decimalen
      if (percentageTilted + currentTiltPercentage > 11000) {
         currentTiltPercentage = 10000;
      }
      else {
         currentTiltPercentage += percentageTilted;
      }
   }
   else {
      return;
   }

   tiltStartTime = millis();
}

int getPercentageTilted() {
   const unsigned long timeDif = millis() - tiltStartTime;
   const float calculatedPercentage = (float)timeDif / (float)timeNeededToTilt;

   // const float returnVal = calculatedPercentage * 10000.0;
   //  (float)timeDifference / (float)timeNeededToTilt;

   // int returnValue =
   //     (int)returnVal;  // percentage (*100) met 2 decimalen (*100)

   // Serial.print("Calculated TILT Percentage: ");
   // Serial.println(calculatedPercentage);

   // Serial.print("Return value");
   // Serial.println(returnValue);
   // return returnValue;

   return calculatedPercentage *
      10000.0;  // percentage (*100) met 2 decimalen (*100)
}

void set_kantelenTimeOut() {
   kantelen_TimeOut.in(maxMovementTime, kantelenTimeOutAlarm);
}

void set_antiPendel_Kantelen() {
   antiPendel_Kantelen = true;
   antiPendel_Kantelen_Timer.in(
      antiPendelTime, reset_antiPendel_Kantelen);  // 5 min. = 300000 ms
}

bool reset_antiPendel_Kantelen(void*) {
   antiPendel_Kantelen = false;
   return false;
}

void read_EindeLoop_Kantelen() {
   if (inschuiven || !einde_Inschuiven) {
      einde_Inschuiven = digitalRead(PIN_Einde_Inschuiven);
      if (einde_Inschuiven) {
         deactivate_Kantelen();
         currentTiltPercentage = 0;
         uitschuivenWhenEindeLoopInschuiven();
      }
   }

   if (uitschuiven || !einde_Uitschuiven) {
      einde_Uitschuiven = digitalRead(PIN_Einde_Uitschuiven);
      if (einde_Uitschuiven) {
         deactivate_Kantelen();
         currentTiltPercentage = 10000;
         inschuivenWhenEindeLoopUitschuiven();
      }
   }
}

bool kantelenTimeOutAlarm(void*) {
   print("ALARM: kantelen TimeOut!");
   kantelenTooLong = true;
   deactivate_Kantelen();
   return false;
}
