const char *min_ = "min";
const char *max_ = "max";
const char *thres_ = "threshold";
const char *stator_ = "stator";

String processor(const String &var)
{
    if (var == "setmin")
    {
        String sendValue = "";
        sendValue += "<input type=\"number\" name=\"min\" maxlength=\"2\" value=" + String(MinDistance) + " > ";
        return sendValue;
    }
    if (var == "setmax")
    {
        String sendValue = "";
        sendValue += "<input type=\"number\" name=\"max\" maxlength=\"3\" value=" + String(MaxDistance) + "> ";
        return sendValue;
    }
    if (var == "setthreshold")
    {
        String sendValue = "";
        sendValue += "<input type=\"number\" name=\"threshold\" maxlength=\"2\" value=" + String(MotorStartThreshold) + "> ";
        return sendValue;
    }
    if (var == "setstator")
    {
        String sendValue = "";
        sendValue += "<input type=\"number\" name=\"stator\" maxlength=\"1\" value=" + String(STATOR_TYPE) + " > ";
        return sendValue;
    }
    return String();
}
void WIFI_CONNECT()
{
#if WM_SET || ESP_CON
#if OLED
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setFont(NULL);
    display.setCursor(0, 16);
    display.println("CONNECTING");
    display.setCursor(0, 35);
    display.setTextSize(2);
    display.print("WiFi");
    display.display();
    delay(1000);
#else
    lcd.clear();
    lcd.print("connecting WiFi");
#endif
#endif
#if WM_SET
    WiFi.mode(WIFI_STA);
    // wm.resetSettings(); // wipe settings
    wm.setConfigPortalBlocking(false);
    if (wm.autoConnect("MDtronix-WTLC-setup"))
    {
#if OLED
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setFont(NULL);
        display.setCursor(0, 16);
        display.println("CONNECTED TO WIFI");
        display.setCursor(0, 35);
        display.setTextSize(1);
        display.print(WiFi.localIP());
        display.display();
        delay(1000);
#else
        lcd.clear();
        lcd.print("Connected");
        lcd.setCursor(0, 1);
        lcd.print(WiFi.localIP());
        Serial.println("connected...yeey :)");
#endif
        delay(1000);
    }
    else
    {
        Serial.println("Configportal running");
    }
#if OLED
#else
    lcd.clear();
#endif
#elif ESP_CON
    // ESPConnect.erase();  //in case to erase setting
    ESPConnect.autoConnect("MDtronix-WTLC-setup");
    if (ESPConnect.begin(&server))
    {
#if OLED
#else
        lcd.clear();
        lcd.print("Connected");
        lcd.setCursor(0, 1);
        lcd.print(WiFi.localIP());
#endif
        Serial.println("Connected to WiFi");
        Serial.println("IPAddress: " + WiFi.localIP().toString());
        delay(1000);
    }
    else
    {
        Serial.println("Failed to connect to WiFi");
    }
#if OLED
#else
    lcd.clear();
#endif
#endif
}

void setting_code()
{
#if AP_MODE
    WiFi.softAP("MDtronix-WTLC");
#endif
    server.onNotFound([](AsyncWebServerRequest *request)
                      { request->send_P(200, "text/html", index_html, processor); });
    server.on("/setting", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  debugln("setting pages");
                  request->send_P(200, "text/html", index_html, processor); });
    server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  int min;
                  int max;
                  int threshold;
                  int stator;
                  String message;
                  if (request->hasParam(min_) && request->hasParam(max_) && request->hasParam(thres_) && request->hasParam(stator_))
                  {
                      min = request->getParam(min_)->value().toInt();
                      max = request->getParam(max_)->value().toInt();
                      threshold = request->getParam(thres_)->value().toInt();
                      stator = request->getParam(stator_)->value().toInt();
                      if (min == 0 || max == 0 || threshold == 0)
                      {
                          message = "invalid values (\"0\") are not allowed";
                      }
                      else if (min < max && threshold <= 70 && threshold >= 20 && stator < 4 && stator != 0)
                      {
                          message = "min: ";
                          message += String(min) + '\n';
                          message += "max: ";
                          message += String(max) + '\n';
                          message += "start at: ";
                          message += String(threshold) + '\n';
                          message += "stator: ";
                          message += String(stator);
                          EEPROM.write(minDistance_mem, min);
                          EEPROM.write(maxDistance_mem, max);
                          EEPROM.write(MotorStartThreshold_mem, threshold);
                          EEPROM.write(StatorType_mem, stator);
                          EEPROM.commit();
                          MinDistance = EEPROM.read(minDistance_mem);
                          MaxDistance = EEPROM.read(maxDistance_mem);
                          MotorStartThreshold = EEPROM.read(MotorStartThreshold_mem);
                          STATOR_TYPE = EEPROM.read(StatorType_mem);
                          debugln("min: " + min ? min : MinDistance);
                      }
                  }
                  else
                  {
                      message = "No message sent";
                  }
                  request->send(200, "text/plain", message); });

    // dns.start(DNS_PORT, "*", IPAddress(WiFi.localIP()));
    if (!MDNS.begin("mdtronix-wtlc"))
    {
        Serial.println("Error setting up MDNS responder!");
        while (1)
        {
            delay(1000);
        }
    }
    Serial.println("mDNS responder started");
    AsyncElegantOTA.begin(&server); // Start ElegantOTA
    server.begin();
    MDNS.addService("http", "tcp", 80);
}
#if HA_INIT
void set_device()
{
    WiFi.macAddress(mac);
    device.setUniqueId(mac, sizeof(mac)); // Unique ID must be set!
    device.setName(DEVICE_NAME);
    device.setSoftwareVersion(DEVICE_VERSION);
    device.setManufacturer(DEVICE_MANUFACTURER);
    device.setModel(DEVICE_MODEL);
    device.enableSharedAvailability();
    device.enableLastWill();
    pump_HA.onStateChanged(pump_action); // handle switch state
    pump_HA.setName(pump_name);
    pump_HA.setIcon("mdi:water");
    value_HA.setName(level_name);
    value_HA.setIcon("mdi:waves");
    value_HA.setUnitOfMeasurement("%");
    distance_HA.setName(distance_name);
    distance_HA.setIcon("mdi:ruler");
    distance_HA.setUnitOfMeasurement("cm");
    mode_HA.onStateChanged(mode_action);
    mode_HA.setName(mode_name);
    mode_HA.setIcon("mdi:nintendo-switch");
    sensor_error_HA.setName(SensorError_name);
}

#endif