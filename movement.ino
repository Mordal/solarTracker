void set_MoveDirection() {
   setDraaien();
   setKantelen();
}

void readEindeloop() {
   read_EindeLoop_Draaien();
   read_EindeLoop_Kantelen();
}

void readForceSignals() {
   linksDraaien_FORCE = digitalRead(PIN_Force_Linksdraaien);
   rechtsDraaien_FORCE = digitalRead(PIN_Force_Rechtsdraaien);
   uitschuiven_FORCE = digitalRead(PIN_Force_Uitschuiven);
   inschuiven_FORCE = digitalRead(PIN_Force_Inschuiven);
}

void stopMomevement() {
   deactivate_Kantelen();
   deactivate_Draaien();
   set_Outputs();
}

void gotoNightPosition() {
  print("IS NIGHT");
   if (isNight()) {
      goToLeftEnd();
      gotoTiltPercentage(20);
      stopMomevement();
      return;
   }
}