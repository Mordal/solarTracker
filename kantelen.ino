
void uitschuiven_activate(){

  if(antiPendel_Kantelen){
    return;
  }

  if(einde_Uitschuiven){
    print("Trying to EXTEND but allready on the end");
    return;
  };

  print("EXTEND");
  uitschuiven = true;
  set_antiPendel_Kantelen();
}

void inschuiven_activate(){

  if(antiPendel_Kantelen){
    return;
  }

  if(einde_Inschuiven){
    print("Trying to RETRACT but allready on the end");
    return;
  };

  print("RETRACT");
  inschuiven = true;
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
  einde_Uitschuiven = digitalRead(PIN_Einde_Uitschuiven);
  einde_Inschuiven = digitalRead(PIN_Einde_Inschuiven);
  myObject["EXTEND"]["Einde_Loop"] = einde_Uitschuiven;
  myObject["RETRACT"]["Einde_Loop"] = einde_Inschuiven;
}

bool kantelenTimeOutAlarm(void *){
  print("ALARM: kantelen TimeOut!");
  kantelenTooLong = true;
  return false;
}