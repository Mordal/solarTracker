
void uitschuiven_activate(){
  if(kantelenTooLong && !uitschuiven_FORCE){
    print("Kantelen in ALARM - RETURN");
    return;
  }

  if(antiPendel_Kantelen && !uitschuiven_FORCE){
    print("ANTI-PENDEL kantelen ACTIVE - RETURN");
    return;
  }

  if(einde_Uitschuiven){
    print("Eindeloop UITSCHUIVEN activated");
    uitschuiven = false;
    return;
  };

  if(STOP && !uitschuiven_FORCE){
    print("STOP activated");
    uitschuiven = false;
    return;
  }

  print("EXTEND");
  uitschuiven = true;
  einde_Inschuiven = false;
  set_kantelenTimeOut();

  if(tiltStartTime = 0){
    tiltStartTime = millis();
  }
}

void inschuiven_activate(){
  if(kantelenTooLong && !inschuiven_FORCE){
    print("Kantelen in ALARM - RETURN");
    return;
  }

  if(antiPendel_Kantelen && !inschuiven_FORCE){
    print("2- ANTI-PENDEL kantelen ACTIVE - RETURN");
    return;
  }

  if(einde_Inschuiven){
    print("EindeLoop INSCHUIVEN activated");
    inschuiven = false;
    return;
  };

  if(STOP && !inschuiven_FORCE){
    print("STOP activated");
    inschuiven = false;
    return;
  }


  print("RETRACT");
  inschuiven = true;
  einde_Uitschuiven = false;
  set_kantelenTimeOut();

  if(tiltStartTime = 0){
    tiltStartTime = millis();
  }
}

void deactivate_Kantelen(){
  bool setAntipendel = false;

  setCurrentTiltPercentage(nullptr);
  tiltStartTime = 0;

  if(uitschuiven || inschuiven){
    setAntipendel = true;
  }

  uitschuiven = false; 
  inschuiven = false;
  
  if(setAntipendel){
    set_antiPendel_Kantelen();
  }
  kantelen_TimeOut.cancel();
}

//Timer: setTiltPercentageTimer
bool setCurrentTiltPercentage(void *){
  const unsigned long percentageTilted = getPercentageTilted(); //moet nog delen door 10000 = 4 decimalen
  if (uitschuiven){
    currentTiltPercentage = currentTiltPercentage + percentageTilted ;
  }
  else if (inschuiven){
    currentTiltPercentage = currentTiltPercentage - percentageTilted ;
  }
  else{
    return true;
  }

  const float floatPercentage = (float)currentTiltPercentage / 10000.0;

  Serial.print("Current Tilt Percentage: ");
  Serial.println(floatPercentage, 4);

  // start from current time again
  tiltStartTime = millis();
  return true;
}

float getPercentageTilted(){
  const unsigned long timeDifference = millis() - tiltStartTime;
  return ((float)timeDifference / (float)timeNeededToTilt) * 1000000.0;
}

void set_kantelenTimeOut(){
  kantelen_TimeOut.in(maxMovementTime, kantelenTimeOutAlarm);
}

void set_antiPendel_Kantelen(){
  antiPendel_Kantelen = true;
  antiPendel_Kantelen_Timer.in(antiPendelTime, reset_antiPendel_Kantelen); //5 min. = 300000 ms
}

bool reset_antiPendel_Kantelen(void *){
   antiPendel_Kantelen = false;
   return false;
}

void read_EindeLoop_Kantelen(){
  if(!einde_Uitschuiven){
    einde_Uitschuiven = digitalRead(PIN_Einde_Uitschuiven);
    if(einde_Uitschuiven){
      uitschuiven = false;
      currentTiltPercentage = 100;
      inschuivenWhenEindeLoopUitschuiven();
    };
  }
  
  if(!einde_Inschuiven){
    einde_Inschuiven = digitalRead(PIN_Einde_Inschuiven);
    if(einde_Inschuiven){
      inschuiven = false;
      currentTiltPercentage = 0;
      uitschuivenWhenEindeLoopInschuiven();
    };
  }
}

void uitschuivenWhenEindeLoopInschuiven(){
  digitalWrite(PIN_Inschuiven, false);
  delay(1000);
  digitalWrite(PIN_Uitschuiven, true);
  while (digitalRead(PIN_Einde_Inschuiven)){
    delay(100);
  }
 digitalWrite(PIN_Uitschuiven, false);
}

void inschuivenWhenEindeLoopUitschuiven(){
  digitalWrite(PIN_Uitschuiven, false);
  delay(1000);
  digitalWrite(PIN_Inschuiven, true);
  while (digitalRead(PIN_Einde_Uitschuiven)){
    delay(100);
    }
  digitalWrite(PIN_Inschuiven, false);
}

bool kantelenTimeOutAlarm(void *){
  print("ALARM: kantelen TimeOut!");
  kantelenTooLong = true;
  deactivate_Kantelen();
  return false;
}

void goToTopEnd(bool includeInschuiven = true){
  tiltStartTime = millis();
  while (!digitalRead(PIN_Einde_Uitschuiven)){
    uitschuiven = true;
    digitalWrite(PIN_Uitschuiven, true);
    delay(100);
  }
  uitschuiven = false;
  digitalWrite(PIN_Uitschuiven, false);
  einde_Uitschuiven = true;
  currentTiltPercentage = 100;

  if(includeInschuiven){
    inschuivenWhenEindeLoopUitschuiven();
  }
}

void goToBottomEnd(bool includeUitschuiven = true){
  tiltStartTime = millis();
  while (!digitalRead(PIN_Einde_Inschuiven)){
    inschuiven = true;
    digitalWrite(PIN_Inschuiven, true);
    delay(100);
  }
  inschuiven = false;
  digitalWrite(PIN_Inschuiven, false);
  einde_Inschuiven = true;
  currentTiltPercentage = 0;

  if(includeUitschuiven){
    uitschuivenWhenEindeLoopInschuiven();
  }
}