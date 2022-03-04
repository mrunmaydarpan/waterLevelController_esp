void lcdDefault()
{
    lcd.setCursor(0, 0);
    lcd.print("PUMP:");
    lcd.setCursor(5, 0);
    lcd.print(MotorState ? "ON " : "OFF");
    lcd.setCursor(10, 0);
    lcd.print(AutoMode ? "  AUTO" : "MANUAL");
    lcd.setCursor(0, 1);
    lcd.print("Lv:");
}

void Debug()
{
    debug("Lv:" + String(value));
    debug("|");
    debug(AutoMode ? "A" : "M");
    debug("|");
    debug("R:" + String(MotorState));
    debug("|");
    debug("MOff:" + String(ManualOff));
    debug("|");
    debug("Distance:" + String(Distance));
    debug("|");
    debug("MotoMode:" + String(STATOR_TYPE));
    debug("|");
    debug("DistX:" + String(DistanceX));
    debug("|");
    debugln();
}

void buttonEvent()
{
    if (button.wasPressed())
    {
        if (MotorState == true)
        {
            if (value >= MotorStartThreshold)
            {
                ManualOff = false;
                EEPROM.write(manualOff_mem, 0);
                EEPROM.commit();
            }
            else
            {
                ManualOff = true;
                AutoMode = false;
                EEPROM.write(manualOff_mem, 1);
                EEPROM.commit();
            }
            PumpOFF_command();
        }
    }
    else if (button.pressedFor(LONG_PRESS))
    {
        ManualOff = false;
        DryRunState = false;
        errorCountState = false;
        EEPROM.write(manualOff_mem, 0);
        EEPROM.commit();
        PumpON_command();
        if (AutoMode == false && modeButton.isPressed())
        {
            AutoMode = true;
        }
    }
    if (modeButton.wasPressed() && ManualOff == false)
    {
        AutoMode = true;
#if HA_INIT
#else
        MODE_DASH.update(AutoMode);
        dashboard.sendUpdates();
#endif
    }
    else if (modeButton.wasReleased())
    {
        AutoMode = false;
#if HA_INIT
#else
        MODE_DASH.update(AutoMode);
        dashboard.sendUpdates();
#endif
    }
}

void OneTimeRun()
{
    if (MotorState != LastMotorState)
    {
        if (MotorState == true)
        {
            motor_on();
#if DryRun
            dryRun_LastDistance = Distance;
            EEPROM.write(dryRun_LastDistance_mem, dryRun_LastDistance);
            dryRun_timer = t.every(DryRunTime, DRY_RUN_CHECK);
#endif
        }
        else
        {
            tone(buzz, 4500, 300);
            motor_off();
#if DryRun
            t.stop(dryRun_timer);
#endif
        }
        debugln("MotorState Changed");
    }
    LastMotorState = MotorState;
    EEPROM.write(LastMotorState_mem, LastMotorState);
    EEPROM.commit();

    if (value != LastValue)
    {
    }
    LastValue = value;

    if (errorCountState != LasterrorCountState)
    {
        lcd.clear();
        if (errorCountState == true)
        {
            PumpOFF_command();
            debugln("ERROR");
            ManualOff = true;
            AutoMode = false;
            EEPROM.write(manualOff_mem, 1);
            EEPROM.commit();
            lcd.setCursor(3, 1);
            if (DryRunState)
            {
                lcd.print("DRY-RUN");
            }
            else
            {
                lcd.print("ERROR");
            }
            EEPROM.write(1, 0);
            ledBlink = t.oscillate(led, 500, HIGH);
        }
        else
        {
            t.stop(ledBlink);
            debugln("Error State Stop");
        }
    }
    LasterrorCountState = errorCountState;
}

void update_lcd()
{
    if (errorCountState == false)
    {
        lcd.setCursor(3, 1);
        lcd.print(value);
        lcd.print("%");
        lcd.print("   ");
    }
    lcd.setCursor(15, 1);
    if (blink_state == true)
    {
        blink_state = false;
        lcd.write(0);
    }
    else
    {
        lcd.write(1);
        blink_state = true;
    }
}
