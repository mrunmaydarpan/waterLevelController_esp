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

void setting_code()
{
    server.onNotFound([](AsyncWebServerRequest *request)
                      { request->send_P(200, "text/html", index_html, processor); });
    server.on("settings/", HTTP_GET, [](AsyncWebServerRequest *request)
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

    dns.start(DNS_PORT, "*", IPAddress(WiFi.softAPIP()));
    AsyncElegantOTA.begin(&server); // Start ElegantOTA
    server.begin();
}