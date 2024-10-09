
void linksDraaien_activate(){
  if (draaienTooLong && !linksDraaien_FORCE){
    print("Draaien in ALARM - RETURN");
    return;
  }

  if(antiPendel_Draaien && !linksDraaien_FORCE){
    print("ANTI-PENDEL Draaien ACTIVE - RETURN");
    return;
  }

  if(einde_Linksdraaien){
    print("Eindeloop LINKS activated");
    linksDraaien = false;
    return;
  };

  if (STOP && !linksDraaien_FORCE){
    print("STOP activated");
    linksDraaien = false;
    return;
  }

  print("TURN LEFT");
  linksDraaien = true;
  einde_Rechtsdraaien = false;
  set_draaienTimeOut();
  // set_antiPendel_Draaien();

  if (turnStartTime = 0){
    turnStartTime = millis();
  }

}

void rechtsDraaien_activate(){
  if (draaienTooLong && !rechtsDraaien_FORCE){
    print("Draaien in ALARM - RETURN");
    return;
  }

  if(antiPendel_Draaien && !rechtsDraaien_FORCE){
    print("ANTI-PENDEL Draaien ACTIVE - RETURN");
    return;
  }

  if(einde_Rechtsdraaien){
    print("EindeLoop RECHTS activated");
    rechtsDraaien = false;
    return;
  };

  if (STOP && !rechtsDraaien_FORCE){
    print("STOP activated");
    rechtsDraaien = false;
    return;
  }

  print("TURN RIGHT");
  rechtsDraaien = true;
  einde_Linksdraaien = false;
  set_draaienTimeOut();
  // set_antiPendel_Draaien();
  
  if (turnStartTime = 0){
    turnStartTime = millis();
  }
}

void deactivate_Draaien(){
  bool setAntipendel = false;

  setCurrentTurnPercentage(nullptr);
  turnStartTime = 0;

  if(linksDraaien || rechtsDraaien){
    setAntipendel = true;
  }

  linksDraaien = false;
  rechtsDraaien = false;

  if (setAntipendel){
    set_antiPendel_Draaien();
  }
  draaien_TimeOut.cancel();
}

//Timer: setTurnPercentageTimer
bool setCurrentTurnPercentage(void *){
  const unsigned long percentageTurned = getPercentageTurned(); //moet nog delen door 10000 = 4 decimalen
  if (linksDraaien){
    currentTurnPercentage = currentTurnPercentage - percentageTurned ;
  }
  else if (rechtsDraaien){
    currentTurnPercentage = currentTurnPercentage + percentageTurned ;
  }
  else{
    return true;
  }

  const float floatPercentage = (float)currentTurnPercentage / 10000.0;

  Serial.print("Current Turn Percentage: ");
  Serial.println(floatPercentage, 4);

  // start from current time again
  turnStartTime = millis();
  return true;
}

float getPercentageTurned(){
  const unsigned long timeDifference = millis() - turnStartTime;
   return ((float)timeDifference / (float)timeNeededToTurn) * 1000000.0;
}

void set_draaienTimeOut(){
  draaien_TimeOut.in(maxMovementTime, draaienTimeOutAlarm);
}

void set_antiPendel_Draaien(){
  antiPendel_Draaien = true;
  antiPendel_Draaien_Timer.in(antiPendelTime, reset_antiPendel_Draaien); 
}

bool reset_antiPendel_Draaien(void *){
   antiPendel_Draaien = false;
   return false;
}

void read_EindeLoop_Draaien(){
  if (!einde_Linksdraaien){
    einde_Linksdraaien = digitalRead(PIN_Einde_Linksdraaien);
    if (einde_Linksdraaien){
      linksDraaien = false;
      currentTurnPercentage = 0;
      turnRightWhenEindeLoopLeft();
    }
  }
  if (!einde_Rechtsdraaien){
    einde_Rechtsdraaien = digitalRead(PIN_Einde_Rechtsdraaien);
    if (einde_Rechtsdraaien){
      rechtsDraaien = false;
      currentTurnPercentage = 100;
      turnLeftWhenEindeLoopRight();
    }
  }
}

void turnRightWhenEindeLoopLeft(){
  digitalWrite(PIN_LinksDraaien, false);
  delay(1000);
  digitalWrite(PIN_RechtsDraaien, true);
  while (digitalRead(PIN_Einde_Linksdraaien)){
    delay(100);
  }
  digitalWrite(PIN_RechtsDraaien, false);
}

void turnLeftWhenEindeLoopRight(){
  digitalWrite(PIN_RechtsDraaien, false);
  delay(1000);
  digitalWrite(PIN_LinksDraaien, true);
  while (digitalRead(PIN_Einde_Rechtsdraaien)){
    delay(100);
  }
  digitalWrite(PIN_LinksDraaien, false);
}

bool draaienTimeOutAlarm(void *){
  print("ALARM: draaien TimeOut!");
  draaienTooLong = true;
  deactivate_Draaien();
  return false;
}

void goToLeftEnd(bool includeTurnRight= true){
  turnStartTime = millis();
  while (!digitalRead(PIN_Einde_Linksdraaien)){
    linksDraaien = true;
    digitalWrite(PIN_LinksDraaien, true);
    delay(100);
  }
  linksDraaien = false;
  digitalWrite(PIN_LinksDraaien, false);
  einde_Linksdraaien = true;
  currentTurnPercentage = 0;

  if(includeTurnRight){
    turnRightWhenEindeLoopLeft();
  }
}

void goToRightEnd(bool includeTurnLeft= true){
  turnStartTime = millis();
  while (!digitalRead(PIN_Einde_Rechtsdraaien)){
    rechtsDraaien = true;
    digitalWrite(PIN_RechtsDraaien, true);
    delay(100);
  }
  rechtsDraaien = false;
  digitalWrite(PIN_RechtsDraaien, false);
  einde_Rechtsdraaien = true;
  currentTurnPercentage = 100;

  if(includeTurnLeft){
    turnLeftWhenEindeLoopRight();
  }
}