
void tickTimers() {
    antiPendel_Draaien_Timer_Remaining = antiPendel_Draaien_Timer.tick();
    antiPendel_Kantelen_Timer_Remaining = antiPendel_Kantelen_Timer.tick();
    draaien_TimeOut_Remaining = draaien_TimeOut.tick();
    kantelen_TimeOut_Remaining = kantelen_TimeOut.tick();
    logBook_Timer_Remaining = logBook_Timer.tick();
    clientConnectedTimer_Remaining = clientConnectedTimer.tick();
    periodicalTimer_Remaining = periodicalTimer.tick();
    setTurnPercentage_Timer.tick();
    setTiltPercentage_Timer.tick();
    settingsUnlockedTimer_Remaining = settingsUnlockedTimer.tick();
    gotoPosition_Timer_Remaining = gotoPosition_Timer.tick();
    sendAllData_Timer.tick();
}


// set permanent timers
void setTimers() {
    periodicalTimer.every(periodicalTime, periodicalFunction);
    start_Logbook_Timer();
    setTurnPercentage_Timer.every(1000, setCurrentTurnPercentage);  // 1 sec
    setTiltPercentage_Timer.every(1000, setCurrentTiltPercentage);  // 1 sec
    gotoPosition_Timer.cancel();  // reset gotoPosition_Timer
}

void restartTimers() {
    periodicalTimer.cancel();
    setTurnPercentage_Timer.cancel();
    setTiltPercentage_Timer.cancel();
    gotoPosition_Timer.cancel();
    setTimers();
    resetLogbookTimers();

}

void resetLogbookTimers() {
    if (clientConnectedTimer.size() > 0) {
        sendAllData_Timer.cancel();
        setClientConnectedTimer();
    }
    else {
        stop_Logbook_Timer();
        start_Logbook_Timer();
    }
}

void start_Logbook_Timer() {
    logBook_Timer.every(logBook_Timer_delay, setLogbook);
    // 10 sec ----- //every minute -> voor een
    // week: 6 keer per uur
}

void stop_Logbook_Timer() { logBook_Timer.cancel(); }

void start_sendAllData_Timer() {
    sendAllData_Timer.every(sendAllData_Timer_delay, sendAllPageData);
}


bool periodicalFunction(void*) {

    //Retry Connections
    if (wifiConnected == false) {
        stopMomevement();
        wifi_Setup();
        RTC_Setup();
    }
    if (mqttConnected == false) {
        mqtt_Setup();
    }

    //SET Preset Positions Timer
    if (gotoPosition_Timer.size() == 0 && getMinuteNumber() >= 27) {
        gotoPosition_Timer.every(3600000, gotoPresetPosition);          // 1 uur
    }

    if (isNight() && !NIGHT_MODE) {
        NIGHT_MODE = true;
        gotoNightPosition();
    }
    else if (!isNight() && NIGHT_MODE) {
        gotoTiltPercentage(0);
        NIGHT_MODE = false;
        stopMomevement();
    }
    return true;
}