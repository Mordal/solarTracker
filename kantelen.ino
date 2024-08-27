
void uitschuiven_activate(){

  if(antiPendel_Kantelen && !uitschuiven_FORCE){
    return;
  }

  if(einde_Uitschuiven){
    print("Trying to EXTEND but allready on the end");
    uitschuiven = false;
    return;
  };

  print("EXTEND");
  uitschuiven = true;
  einde_Inschuiven = false;
  set_antiPendel_Kantelen();

  if(tiltStartTime = 0){
    tiltStartTime = millis();
  }

}

void inschuiven_activate(){
  if(antiPendel_Kantelen && !inschuiven_FORCE){
    return;
  }

  if(einde_Inschuiven){
    print("Trying to RETRACT but allready on the end");
    inschuiven = false;
    return;
  };

  print("RETRACT");
  inschuiven = true;
  einde_Uitschuiven = false;
  set_antiPendel_Kantelen();

  if(tiltStartTime = 0){
    tiltStartTime = millis();
  }
}

void deactivate_Kantelen(){
  setCurrentTiltPercentage(void*);
  tiltStartTime = 0;

  uitschuiven = false;
  inschuiven = false;
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

  myObject["EXTEND"]["Percentage"] = String(floatPercentage);
  myObject["RETRACT"]["Percentage"] = String(floatPercentage);

  // start from current time again
  tiltStartTime = millis();
  return true;
}

float getPercentageTilted(){
  return ((float)millis() - (float)tiltStartTime) /(float)timeNeededToTilt * 1000000.0;
}



void set_antiPendel_Kantelen(){
  antiPendel_Kantelen = true;
  myObject["EXTEND"]["AntiPendel"] = antiPendel_Kantelen;
  myObject["RETRACT"]["AntiPendel"] = antiPendel_Kantelen;

  antiPendel_Kantelen_Timer.in(antiPendelTime, reset_antiPendel_Kantelen); //5 min. = 300000 ms
  kantelen_TimeOut.in(maxMovementTime, kantelenTimeOutAlarm);
}

bool reset_antiPendel_Kantelen(void *){
   antiPendel_Kantelen = false;
   myObject["EXTEND"]["AntiPendel"] = antiPendel_Kantelen;
   myObject["RETRACT"]["AntiPendel"] = antiPendel_Kantelen;
   return false;
}

void read_EindeLoop_Kantelen(){
  if(!einde_Uitschuiven){
    einde_Uitschuiven = digitalRead(PIN_Einde_Uitschuiven);
    if(einde_Uitschuiven){
      currentTiltPercentage = 100;
      inschuivenWhenEindeLoopUitschuiven();
    };
  }
  
  if(!einde_Inschuiven){
    einde_Inschuiven = digitalRead(PIN_Einde_Inschuiven);
    if(einde_Inschuiven){
      currentTiltPercentage = 0;
      uitschuivenWhenEindeLoopInschuiven();
    };
  }

  myObject["EXTEND"]["Einde_Loop"] = einde_Uitschuiven;
  myObject["RETRACT"]["Einde_Loop"] = einde_Inschuiven;
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
  return false;
}