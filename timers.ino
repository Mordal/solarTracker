
void tickTimers() {
    antiPendel_Draaien_Timer_Remaining = antiPendel_Draaien_Timer.tick();
    antiPendel_Kantelen_Timer_Remaining = antiPendel_Kantelen_Timer.tick();
    draaien_TimeOut_Remaining = draaien_TimeOut.tick();
    kantelen_TimeOut_Remaining = kantelen_TimeOut.tick();
    logBook_Timer_Remaining = logBook_Timer.tick();
    clientConnectedTimer_Remaining = clientConnectedTimer.tick();
    periodicalTimer_Remaining = periodicalTimer.tick();
    setMovementPercentage_Timer.tick();
    settingsUnlockedTimer_Remaining = settingsUnlockedTimer.tick();
    gotoPosition_Timer_Remaining = gotoPosition_Timer.tick();
    sendAllData_Timer.tick();
    RTCcorrection_Timer_Remaining = RTCcorrection_Timer.tick();
}


// set permanent timers
void setTimers() {
    periodicalTimer.every(periodicalTime, periodicalFunction);
    start_Logbook_Timer();
    setMovementPercentage_Timer.every(1000, setCurrentMovementPercentage);  // 1 sec
    gotoPosition_Timer.cancel();  // reset gotoPosition_Timer
    RTCcorrection_Timer.every(RTCcorrectionTime, RTCcorrectionFunction);
}

void restartTimers() {
    periodicalTimer.cancel();
    setMovementPercentage_Timer.cancel();
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
    logBook_Timer.every(logbookTime, setLogbook);
    // 10 sec ----- //every minute -> voor een
    // week: 6 keer per uur
}

void stop_Logbook_Timer() { logBook_Timer.cancel(); }

void start_sendAllData_Timer() {
    sendAllData_Timer.every(sendAllDataTime, sendAllPageData);
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


bool RTCcorrectionFunction(void*) {
    //RTC runs to fast 
    //   -> (1,556 sec/min)
    //   -> (0,025933333 sec/sec

    unsigned long epochTime = getEpochTime();
    epochTime -= 7;
    setRTC(epochTime);
    return true;
}