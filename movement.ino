void set_MoveDirection(){
 // DRAAIEN //
  if(needToTurnLeft()){
    if(rechtsDraaien == true){
      deactivate_Draaien();
    }
    linksDraaien_activate();
  }

  else if(needToTurnRight()){
    if(linksDraaien == true){
      deactivate_Draaien();
    }
    rechtsDraaien_activate();
  }

  else{
    deactivate_Draaien();
  }

 // KANTELEN //
  if(needToExtend()){
    if(inschuiven == true){
      deactivate_Kantelen();
    }
    uitschuiven_activate();
  }

  else if(needToRetract()){
    if(uitschuiven == true){
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


bool needToTurnLeft(){
  int lichtLinks = lichtSensor_LB + lichtSensor_LO;
  int lichtRechts = lichtSensor_RB + lichtSensor_RO;

  myObject["TURN_LEFT"]["licht_links"] = lichtLinks;
  myObject["TURN_LEFT"]["licht_rechts"] = lichtRechts + marge;

  if(lichtLinks > lichtRechts + marge){
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
  myObject["TURN_RIGHT"]["licht_links"] = lichtLinks + marge;

  if(lichtRechts > lichtLinks + marge){
    myObject["TURN_RIGHT"]["Sensors"] = true;
    return true;
  }
  myObject["TURN_RIGHT"]["Sensors"] = false;
  return false;
}

bool needToExtend(){
  int lichtBoven = lichtSensor_LB + lichtSensor_RB;
  int lichtOnder = lichtSensor_LO + lichtSensor_RO;

  myObject["EXTEND"]["licht_boven"] = lichtBoven;
  myObject["EXTEND"]["licht_onder"] = lichtOnder + marge;
 
  if(lichtBoven > lichtOnder + marge){
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
  myObject["RETRACT"]["licht_boven"] = lichtBoven + marge;

  if(lichtOnder > lichtBoven + marge){
    myObject["RETRACT"]["Sensors"] = true;
    return true;
  }
  myObject["RETRACT"]["Sensors"] = false;
  return false;
}

void stopMomevement(){
  deactivate_Kantelen()
  deactivate_Draaien()
  set_Outputs()
}