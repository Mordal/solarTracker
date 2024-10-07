void set_MoveDirection(){
 // DRAAIEN //
  if(needToTurnLeft() || linksDraaien_FORCE){
    if(rechtsDraaien == true){
      rechtsDraaien_FORCE = false;
      deactivate_Draaien();
    }
    linksDraaien_activate();
  }

  else if(needToTurnRight() || rechtsDraaien_FORCE){
    if(linksDraaien == true){
      linksDraaien_FORCE = false;
      deactivate_Draaien();
    }
    rechtsDraaien_activate();
  }

  else{
    deactivate_Draaien();
  }

 // KANTELEN //
  if(needToExtend() || uitschuiven_FORCE){
    if(inschuiven == true){
      inschuiven_FORCE = false;
      deactivate_Kantelen();
    }
    uitschuiven_activate();
  }

  else if(needToRetract() || inschuiven_FORCE){
    if(uitschuiven == true){
      uitschuiven_FORCE = false;
      deactivate_Kantelen();
    }
    inschuiven_activate();
  }

  else{
    deactivate_Kantelen();
  }
}

void readEindeloop(){
  read_EindeLoop_Draaien();
  read_EindeLoop_Kantelen();
}

void readForceSignals(){
  linksDraaien_FORCE = digitalRead(PIN_Force_Linksdraaien);
  rechtsDraaien_FORCE = digitalRead(PIN_Force_Rechtsdraaien);
  uitschuiven_FORCE = digitalRead(PIN_Force_Uitschuiven);
  inschuiven_FORCE = digitalRead(PIN_Force_Inschuiven);
}

bool needToTurnLeft(){
  int lichtLinks = lichtSensor_LB + lichtSensor_LO;
  int lichtRechts = lichtSensor_RB + lichtSensor_RO;

  myObject["TURN_LEFT"]["licht_links"] = lichtLinks;
  myObject["TURN_LEFT"]["licht_rechts"] = lichtRechts + licht_marge;

  if(lichtLinks > lichtRechts + licht_marge){
    myObject["TURN_LEFT"]["Sensors"] = true;
    return true;
  }
  myObject["TURN_LEFT"]["Sensors"] = false;
  return false;
}

bool needToTurnRight(){
  int lichtLinks = lichtSensor_LB + lichtSensor_LO;
  int lichtRechts = lichtSensor_RB + lichtSensor_RO;

  myObject["TURN_RIGH"]["licht_rechts"] = lichtRechts;
  myObject["TURN_RIGHT"]["licht_links"] = lichtLinks + licht_marge;

  if(lichtRechts > lichtLinks + licht_marge){
    rechtsDraaien_Sensors = true;
    return true;
  }
  rechtsDraaien_Sensors = false;
  return false;
}

bool needToExtend(){
  int lichtBoven = lichtSensor_LB + lichtSensor_RB;
  int lichtOnder = lichtSensor_LO + lichtSensor_RO;

  myObject["EXTEND"]["licht_boven"] = lichtBoven;
  myObject["EXTEND"]["licht_onder"] = lichtOnder + licht_marge;
 
  if(lichtBoven > lichtOnder + licht_marge){
    myObject["EXTEND"]["Sensors"] = true;
    return true;
  }
  myObject["EXTEND"]["Sensors"] = false;
  return false;
}

bool needToRetract(){
  int lichtBoven = lichtSensor_LB + lichtSensor_RB;
  int lichtOnder = lichtSensor_LO + lichtSensor_RO;

  myObject["RETRACT"]["licht_onder"] = lichtOnder;
  myObject["RETRACT"]["licht_boven"] = lichtBoven + licht_marge;

  if(lichtOnder > lichtBoven + licht_marge){
    myObject["RETRACT"]["Sensors"] = true;
    return true;
  }
  myObject["RETRACT"]["Sensors"] = false;
  return false;
}

void stopMomevement(){
  deactivate_Kantelen();
  deactivate_Draaien();
  set_Outputs();
}



