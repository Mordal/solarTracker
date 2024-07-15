
void linksDraaien_activate(){

  if(antiPendel_Draaien && !linksDraaien_FORCE){
    return;
  }

  if(einde_Linksdraaien){
    print("Trying to turn LEFT but allready on the end");
    linksDraaien = false;
    return;
  };

  print("TURN LEFT");
  linksDraaien = true;
  einde_Rechtsdraaien = false;
  set_antiPendel_Draaien();
}

void rechtsDraaien_activate(){

  if(antiPendel_Draaien && !rechtsDraaien_FORCE){
    return;
  }

  if(einde_Rechtsdraaien){
    print("Trying to turn RIGHT but allready on the end");
    rechtsDraaien = false;
    return;
  };

  print("TURN RIGHT");
  rechtsDraaien = true;
  einde_Linksdraaien = false;
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
  if (!einde_Linksdraaien){
    einde_Linksdraaien = digitalRead(PIN_Einde_Linksdraaien);
    if (einde_Linksdraaien){
      turnRightWhenEindeLoopLeft();
    }
  }
  if (!einde_Rechtsdraaien){
    einde_Rechtsdraaien = digitalRead(PIN_Einde_Rechtsdraaien);
    if (einde_Rechtsdraaien){
      turnLeftWhenEindeLoopRight();
    }
  }

  myObject["TURN_LEFT"]["Einde_Loop"] = einde_Linksdraaien;
  myObject["TURN_RIGHT"]["Einde_Loop"] = einde_Rechtsdraaien;
}

void turnRightWhenEindeLoopLeft(){
  digitalWrite(PIN_LinksDraaien, false);
  delay(500)
  digitalWrite(PIN_RechtsDraaien, true);
  while digitalRead(PIN_Einde_Linksdraaien){
    delay(100);
  }
  digitalWrite(PIN_RechtsDraaien, false);
}

void turnLeftWhenEindeLoopRight(){
  digitalWrite(PIN_RechtsDraaien, false);
  delay(500)
  digitalWrite(PIN_LinksDraaien, true);
  while digitalRead(PIN_Einde_Rechtsdraaien){
    delay(100);
  }
  digitalWrite(PIN_LinksDraaien, false);
}

bool draaienTimeOutAlarm(void *){
  print("ALARM: draaien TimeOut!");
  draaienTooLong = true;
  return false;
}

void goToLeftEnd(includeTurnRight= true){
  while (!digitalRead(PIN_Einde_Linksdraaien)){
    digitalWrite(PIN_LinksDraaien, true)
    delay(100);
  }
  if(includeTurnRight){
    turnRightWhenEindeLoopLeft()
  }
  digitalWrite(PIN_LinksDraaien, false)
}

void goToRightEnd(includeTurnLeft= true){
  while (!digitalRead(PIN_Einde_Rechtsdraaien)){
    digitalWrite(PIN_RechtsDraaien, true)
    delay(100);
  }
  if(includeTurnLeft){
    turnLeftWhenEindeLoopRight()
  }
  digitalWrite(PIN_RechtsDraaien, false)
}