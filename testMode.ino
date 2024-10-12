void testMode() {}

void initializeNeededTime() {
   getTimeDraaien();
   getTimeKantelen();
}

void getTimeDraaien() {
   goToRightEnd();
   const unsigned long startingTime = millis();
   goToLeftEnd(false);
   const unsigned long endingTime = millis();
   timeNeededToTurn = endingTime - startingTime;
   turnRightWhenEindeLoopLeft();
}

void getTimeKantelen() {
   goToTopEnd();
   const unsigned long startingTime = millis();
   goToBottomEnd(false);
   const unsigned long endingTime = millis();
   timeNeededToTilt = endingTime - startingTime;
   uitschuivenWhenEindeLoopInschuiven();
}