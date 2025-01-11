
void gotoTiltPercentage(int percentage = -1) {
   if (percentage != -1) {
      gotoTiltPosition = true;
      wantedTiltPercentage = percentage;
   }
   if ((wantedTiltPercentage * 100) > currentTiltPercentage + 200) {
      uitschuiven_FORCE = true;
   } else if ((wantedTiltPercentage * 100) < currentTiltPercentage - 200) {
      inschuiven_FORCE = true;
   } else {
      uitschuiven_FORCE = false;
      inschuiven_FORCE = false;
      gotoTiltPosition = false;
   }
}

void setKantelen() {
   if (gotoTiltPosition) {
      gotoTiltPercentage();
   }
   if (kantelenForceMode()) {
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
   } else if (needToRetract()) {
      if (uitschuiven) {
         deactivate_Kantelen();
      }
      inschuiven_activate();
   } else {
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
   setCurrentTiltPercentage(nullptr);
   tiltStartTime = 0;
   if (uitschuiven || inschuiven) {
      set_antiPendel_Kantelen();
   }
   uitschuiven = false;
   inschuiven = false;
   kantelen_TimeOut.cancel();
}

// Timer: setTiltPercentageTimer
bool setCurrentTiltPercentage(void *) {
   const unsigned int percentageTilted =
       getPercentageTilted();  // moet nog delen door 100 = 2 decimalen
   if (uitschuiven) {
      if (percentageTilted > currentTiltPercentage) {
         currentTiltPercentage = 0;
      } else {
         currentTiltPercentage -= percentageTilted;
      }

   } else if (inschuiven) {
      currentTiltPercentage = currentTiltPercentage - percentageTilted;
   } else {
      return true;
   }

   const float floatPercentage = (float)currentTiltPercentage / 100.0;

   Serial.print("Current Tilt Percentage: ");
   Serial.println(floatPercentage, 4);

   // start from current time again
   tiltStartTime = millis();
   return true;
}

unsigned int getPercentageTilted() {
   const unsigned long timeDifference = millis() - tiltStartTime;
   return ((float)timeDifference / (float)timeNeededToTilt) *
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

bool reset_antiPendel_Kantelen(void *) {
   antiPendel_Kantelen = false;
   return false;
}

void read_EindeLoop_Kantelen() {
   if (!einde_Uitschuiven) {
      einde_Uitschuiven = digitalRead(PIN_Einde_Uitschuiven);
      if (einde_Uitschuiven) {
         deactivate_Kantelen();
         currentTiltPercentage = 10000;
         inschuivenWhenEindeLoopUitschuiven();
      };
   }

   if (!einde_Inschuiven) {
      einde_Inschuiven = digitalRead(PIN_Einde_Inschuiven);
      if (einde_Inschuiven) {
         deactivate_Kantelen();
         currentTiltPercentage = 0;
         uitschuivenWhenEindeLoopInschuiven();
      };
   }
}

bool kantelenTimeOutAlarm(void *) {
   print("ALARM: kantelen TimeOut!");
   kantelenTooLong = true;
   deactivate_Kantelen();
   return false;
}
