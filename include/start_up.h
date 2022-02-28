void StartUp()
{
    const char *compile_date = __DATE__;
    debugln(F("Ready......."));
    debugln("SW: " + String(_VERSION));
    debugln("DT: " + String(compile_date));
    debugln("MaxDistance: " + String(MaxDistance));
    debugln("MinDistance: " + String(MinDistance));
    debugln("start at: " + String(MotorStartThreshold));
    debugln("ManualOff: " + String(EEPROM.read(manualOff_mem)));
    debugln("MotorState: " + String(EEPROM.read(motorState_mem)));
    debugln("Mode: " + String(AutoMode));
    debug(F("Starting."));
    lcd.clear();
    lcd.setCursor(11, 0);
    lcd.print("v" + String(_VERSION));
    lcd.setCursor(0, 1);
    lcd.print(compile_date);
    pinMode(buzz, OUTPUT);
    for (int l = 0; l < int(sizeof(brand) - 1); l++)
    {
        lcd.setCursor(l, 0);
        lcd.print(brand[l]);
        tone(buzz, 4500, 150);
        digitalWrite(led, !digitalRead(led));
        debug(F("."));
        delay(300);
    }
    debugln();
    delay(500);
    char showVersion[14];
    char showData[16];
    sprintf(showVersion, "HW:%s", _HARDWARE);
    sprintf(showData, "L:%d H:%d P:%d", MaxDistance, MinDistance, MotorStartThreshold);
    lcd.clear();
    lcd.print(showVersion);
    lcd.setCursor(0, 1);
    lcd.print(showData);
    lcd.setCursor(14, 0);
#if SENSOR_1
    lcd.print("1");
#elif SENSOR_2
    lcd.print("2");
#elif SENSOR_3
    lcd.print("3");
#endif
    lcd.setCursor(15, 0);
    lcd.print(STATOR_TYPE);
    delay(2000);
    lcd.clear();
    digitalWrite(led, LOW);
}