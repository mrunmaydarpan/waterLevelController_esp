#if DryRun
#define DryRunTime 120000
#endif

#if debugData
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <AsyncElegantOTA.h>
#include <DNSServer.h>
// #include <ArduinoHA.h>
#include <JC_Button.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Timer.h>
#if SENSOR_1
#include <Ultrasonic.h>
#endif
#include <Smoothed.h>
#include <pins.h>

#if SENSOR_1
Ultrasonic Sonar(TriggerPin, EchoPin);
#elif SENSOR_2
SoftwareSerial sensorSerial(sensorRX, sensorTX);
#elif SENSOR_3
#endif
LiquidCrystal_I2C lcd(0x27, 16, 2);
Button button(PB);
Button modeButton(_Mode);
Timer t;
Smoothed<uint8_t> mySensor;
AsyncWebServer server(80);
DNSServer dns;
WiFiClient client;