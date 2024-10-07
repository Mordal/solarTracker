void set_MoveDirection(){
  setDraaien();
  setKantelen();
}

void setDraaien(){
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
}

void setKantelen(){
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
  if(lichtSensors_Links > lichtSensors_Rechts + licht_marge){
    linksDraaien_Sensors = true;
    return true;
  }
  linksDraaien_Sensors = false;
  return false;
}

bool needToTurnRight(){
  if(lichtSensors_Rechts > lichtSensors_Links + licht_marge){
    rechtsDraaien_Sensors = true;
    return true;
  }
  rechtsDraaien_Sensors = false;
  return false;
}

bool needToExtend(){
  if(lichtSensors_Boven > lichtSensors_Onder + licht_marge){
    uitschuiven_Sensors = true;
    return true;
  }
  uitschuiven_Sensors = false;
  return false;
}

bool needToRetract(){

  if(lichtSensors_Onder > lichtSensors_Boven + licht_marge){
    inschuiven_Sensors = true;
    return true;
  }
  inschuiven_Sensors = false;
  return false;
}

void stopMomevement(){
  deactivate_Kantelen();
  deactivate_Draaien();
  set_Outputs();
}



