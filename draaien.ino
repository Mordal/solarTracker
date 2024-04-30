
void linksDraaien_activate(){

  if(antiPendel_Draaien){
    return;
  }

  if(einde_Linksdraaien){
    print("Trying to turn LEFT but allready on the end");
    return;
  };

  print("TURN LEFT");
  linksDraaien = true;
  set_antiPendel_Draaien();
}

void rechtsDraaien_activate(){

  if(antiPendel_Draaien){
    return;
  }

  if(einde_Rechtsdraaien){
    print("Trying to turn RIGHT but allready on the end");
    return;
  };

  print("TURN RIGHT");
  rechtsDraaien = true;
  set_antiPendel_Draaien();
}

void deactivate_Draaien(){
  linksDraaien = false;
  rechtsDraaien = false;
  draaien_TimeOut.cancel();
}

void set_antiPendel_Draaien(){
  antiPendel_Draaien = true;
  myObject["TURN_LEFT"]["AntiPendel"] = antiPendel_Draaien;
  myObject["TURN_RIGHT"]["AntiPendel"] = antiPendel_Draaien;

  antiPendel_Draaien_Timer.in(antiPendelTime, reset_antiPendel_Draaien); 
  draaien_TimeOut.in(maxMovementTime, draaienTimeOutAlarm);
}

bool reset_antiPendel_Draaien(void *){
   antiPendel_Draaien = false;
   myObject["TURN_LEFT"]["AntiPendel"] = antiPendel_Draaien;
   myObject["TURN_RIGHT"]["AntiPendel"] = antiPendel_Draaien;
   return false;
}

void read_EindeLoop_Draaien(){
  einde_Linksdraaien = digitalRead(PIN_Einde_Linksdraaien);
  einde_Rechtsdraaien = digitalRead(PIN_Einde_Rechtsdraaien);
  myObject["TURN_LEFT"]["Einde_Loop"] = einde_Linksdraaien;
  myObject["TURN_RIGHT"]["Einde_Loop"] = einde_Rechtsdraaien;
}

bool draaienTimeOutAlarm(void *){
  print("ALARM: draaien TimeOut!");
  draaienTooLong = true;
  return false;
}