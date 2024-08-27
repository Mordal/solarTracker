void testMode(){
}

void getTimeDraaien(){
    setMillis(0)
    goToRightEnd()
    unsigned long startingTime = millis();
    goToLeftEnd(false)
    unsigned long endingTime = millis();
    timeNeededToTurn = endingTime - startingTime;
    turnRightWhenEindeLoopLeft()
}

void getTimeKantelen(){
    setMillis(0)
    goToTopEnd()
    unsigned long startingTime = millis();
    goToBottomEnd(false)
    unsigned long endingTime = millis();
    timeNeededToTilt = endingTime - startingTime;
    tiltUpWhenEindeLoopDown()
}