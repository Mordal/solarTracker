void testMode(){
}

void getTimeDraaien(){
    resetMillis();
    goToRightEnd();
    const unsigned long startingTime = millis();
    goToLeftEnd(false);
    const unsigned long endingTime = millis();
    timeNeededToTurn = endingTime - startingTime;
    turnRightWhenEindeLoopLeft();
}

void getTimeKantelen(){
    resetMillis();
    goToTopEnd();
    const unsigned long startingTime = millis();
    goToBottomEnd(false);
    const unsigned long endingTime = millis();
    timeNeededToTilt = endingTime - startingTime;
    tiltUpWhenEindeLoopDown();
}