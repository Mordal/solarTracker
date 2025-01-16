
// mss enkel doen wanneer de positie lang niet is veranderd
// http://www.weerstationuithoorn.nl/weer/zonnestand.htm

// Zomertijd: Van de laatste zondag van maart tot de laatste zondag van oktober.
// Wintertijd: Van de laatste zondag van oktober tot de laatste zondag van maart
// (het daaropvolgende jaar).

byte turnPercentage_Presets[14] = {0,  5,  10, 19, 28, 35, 42,
                                   52, 61, 70, 75, 80, 88, 100};

// byte[14] getTurnPercentage_Presets() { return turnPercentage_Presets; };

// void setTurnPercentage_Presets(byte[14] ...newTurnPercentagePresets) {
//    turnPercentage_Presets = ...newTurnPercentagePresets;
// };

byte getExpectedTurnPosition(byte hour) {
   // Winter:
   // 6h = 0%
   // 7h = 5%
   // 8h = 10%
   // 9h = 19%
   // 10h = 28%
   // 11h = 35%
   // 12h = 42%
   // 13h = 52%
   // 14h = 61%
   // 15h = 70%
   // 16h = 75%
   // 17h = 80%
   // 18h = 88%
   // 19h = 100%
   //!! Zomer: - Alles +1u !!!

   if (isSummerTimeRTC()) {
      hour--;
   }

   // Controleer of het uur binnen het bereik ligt (6 tot 19)
   if (hour < 6) {
      return 0;  // Voor 6 uur is het percentage 0%
   } else if (hour > 19) {
      return 100;  // Na 19 uur is het percentage 100%
   }

   byte turnPercentage = turnPercentage_Presets[hour - 6];
   return turnPercentage;
}

byte tiltPercentage_Presets[12][14] = {
    // Januari  (0)
    {0, 0, 0, 0, 0, 0, 0, 10, 10, 0, 0, 0, 0, 0},
    // Februari (1)
    {0, 0, 0, 0, 0, 0, 0, 10, 17, 25, 20, 12, 0, 0},
    // Maart    (2)
    {0, 0, 0, 0, 0, 0, 12, 27, 39, 48, 49, 42, 35, 0},
    // April    (3)
    {0, 0, 0, 0, 0, 14, 32, 50, 63, 72, 75, 69, 55, 0},
    // Mei      (4)
    {0, 10, 28, 48, 63, 80, 89, 90, 86, 72, 52, 37, 15, 0},
    // Juni     (5)
    {0, 13, 35, 52, 70, 87, 95, 100, 90, 76, 57, 38, 23, 0},
    // Juli     (6)
    {0, 10, 28, 48, 63, 80, 89, 90, 86, 72, 52, 37, 15, 0},
    // Augustus (7)
    {0, 0, 0, 0, 0, 14, 32, 50, 63, 72, 75, 69, 55, 0},
    // September (8)
    {0, 0, 0, 0, 0, 0, 12, 27, 39, 48, 49, 42, 35, 0},
    // Oktober   (9)
    {0, 0, 0, 0, 0, 0, 10, 17, 25, 20, 12, 0, 0, 0},
    // November  (10)
    {0, 0, 0, 0, 0, 0, 0, 10, 10, 0, 0, 0, 0, 0},
    // December  (11)
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

// byte getTiltPercentage_Presets() { return tiltPercentage_Presets; };
// void setTiltPercentage_Presets(byte newTiltPercentagePresets[12][14]) {
//    tiltPercentage_Presets = newTiltPercentagePresets;
// }

byte getExpectedTiltPosition(byte hour, byte month) {
   // Winter:
   // januari + november:
   // 6-11h = 0%
   // 12h-13h = 10%
   // 14h-19h = 0%

   // februari + oktober:
   // 6h-9h = 0%
   // 10h = 10%
   // 11h = 17%
   // 12h-13h = 25%
   // 14h = 20%
   // 15h = 12%
   // 16h-19h = 0%

   // maart + september:
   // 6h-8h = 0%
   // 9h = 12%
   // 10h = 27%
   // 11h = 39%
   // 12h = 48%
   // 13h = 49%
   // 14h = 42%
   // 15h = 35%
   // 16h = 19%
   // 17h-19h = 0%

   // april + augustus:
   // 6h-7h = 0%
   // 8h = 14%
   // 9h = 32%
   // 10h = 50%
   // 11h = 63%
   // 12h = 72%
   // 13h = 75%
   // 14h = 69%
   // 15h = 55%
   // 16h = 39%
   // 17h = 23%
   // 18h-19h = 0%

   // mei + juli:
   // 6h = 0%
   // 7h = 10%
   // 8h = 28%
   // 9h = 48%
   // 10h = 63%
   // 11h = 80%
   // 12h = 89%
   // 13h = 90%
   // 14h = 86%
   // 15h = 72%
   // 16h = 52%
   // 17h = 37%
   // 18h = 15%
   // 19h = 0%

   // juni:
   // 6h = 0%
   // 7h = 13%
   // 8h = 35%
   // 9h = 52%
   // 10h = 70%
   // 11h = 87%
   // 12h = 95%
   // 13h = 100%
   // 14h = 90%
   // 15h = 76%
   // 16h = 57%
   // 17h = 38%
   // 18h = 23%
   // 19h = 0%

   if (isSummerTimeRTC()) {
      hour--;
   }

   // Controleer of het uur geldig is (6-19)
   if (hour < 6) {
      return 0;
   }

   if (hour > 19) {
      return 100;
   }

   month -= 1;

   // Controleer of de maand geldig is (0-11)
   if (month < 0 || month > 11) {
      return 0;  // Ongeldige maand, retourneer 0%
   }

   // Retourneer het bijbehorende percentage
   return tiltPercentage_Presets[month]
                                [hour - 6];  // Indexering voor uur 6 tot 19
}

bool gotoPresetPosition(void *) {
   if (isNight()) {
      return true;
   }

   if (previousMonth != getMonthNumber()) {
      previousMonth = getMonthNumber();
      setTimeFromNet();
   }

   byte expectedTurnPosition = getExpectedTurnPosition(getHourNumber());
   gotoTurnPercentage(expectedTurnPosition * 100);

   byte expectedTiltPosition =
       getExpectedTiltPosition(getHourNumber(), getMonthNumber());
   gotoTiltPercentage(expectedTiltPosition * 100);
   return true;
}
