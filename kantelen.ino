
void uitschuiven_activate(){

  if(antiPendel_Kantelen && !uitschuiven_FORCE){
    return;
  }

  if(einde_Uitschuiven){
    print("Trying to EXTEND but allready on the end");
    uitschuiven = false;
    return;
  };

  print("EXTEND");S
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
}

void deactivate_Kantelen(){
  uitschuiven = false;
  inschuiven = false;
  kantelen_TimeOut.cancel();
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
      inschuivenWhenEindeLoopUitschuiven()
    }
  }
  
  if(!einde_Inschuiven){
    einde_Inschuiven = digitalRead(PIN_Einde_Inschuiven);
    if(einde_Inschuiven){
      uitschuivenWhenEindeLoopInschuiven()
    }
  }

  myObject["EXTEND"]["Einde_Loop"] = einde_Uitschuiven;
  myObject["RETRACT"]["Einde_Loop"] = einde_Inschuiven;
}

void uitschuivenWhenEindeLoopInschuiven(){
  digitalWrite(PIN_Inschuiven, false);
  delay(500)
  digitalWrite(PIN_Uitschuiven, true);
  while (digitalRead(PIN_Einde_Inschuiven)){
    delay(100);
  }
 digitalWrite(PIN_Uitschuiven, false);
}

void inschuivenWhenEindeLoopUitschuiven(){
  digitalWrite(PIN_Uitschuiven, false);
  delay(500)
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