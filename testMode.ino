void testMode(){
}

void getTimeDraaien(){
    setMillis(0);
    goToRightEnd();
    const unsigned long startingTime = millis();
    goToLeftEnd(false);
    const unsigned long endingTime = millis();
    timeNeededToTurn = endingTime - startingTime;
    turnRightWhenEindeLoopLeft();
}

void getTimeKantelen(){
    setMillis(0);
    goToTopEnd();
    const unsigned long startingTime = millis();
    goToBottomEnd(false);
    const unsigned long endingTime = millis();
    timeNeededToTilt = endingTime - startingTime;
    tiltUpWhenEindeLoopDown();
}