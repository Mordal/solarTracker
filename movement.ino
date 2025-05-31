void set_MoveDirection() {
   setDraaien();
   setKantelen();
}

void readEindeloop() {
   read_EindeLoop_Draaien();
   read_EindeLoop_Kantelen();
}

void readForceSignals() {
   linksDraaien_FORCE = digitalRead(PIN_Force_Linksdraaien) || linksDraaien_FORCE_EXT;
   rechtsDraaien_FORCE = digitalRead(PIN_Force_Rechtsdraaien) || rechtsDraaien_FORCE_EXT;
   uitschuiven_FORCE = digitalRead(PIN_Force_Uitschuiven) || uitschuiven_FORCE_EXT;
   inschuiven_FORCE = digitalRead(PIN_Force_Inschuiven) || inschuiven_FORCE_EXT;
}

void stopMomevement() {
   deactivate_Draaien();
   deactivate_Kantelen();
   set_Outputs();
}

void clearForceSignals() {
   linksDraaien_FORCE = false;
   rechtsDraaien_FORCE = false;
   uitschuiven_FORCE = false;
   inschuiven_FORCE = false;
   gotoTurnPosition = false;
   gotoTiltPosition = false;
}

void gotoSafePosition(int turnPercentage = 20, int tiltPercentage = 50) {
   SAFE_MODE = true;
   gotoTurnPercentage(turnPercentage);
   gotoTiltPercentage(tiltPercentage);
   stopMomevement();
   return;
}

void gotoNightPosition() {
   initializeNeededTime();
   gotoTiltPercentage(20);  // 0% tilt
   stopMomevement();
   return;
}

unsigned int getMovementTime(const unsigned long& startTime) {
   unsigned long currentTime = millis();
   return (unsigned int)(currentTime - startTime);
}

bool movementTooLong(const unsigned long& startTime) {
   unsigned int timeMoved = getMovementTime(startTime);
   return timeMoved > maxMovementTime;
}