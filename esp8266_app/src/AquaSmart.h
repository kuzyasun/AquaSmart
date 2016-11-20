#define AQUASMART_H
#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif

#include <ESPIot.h>
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <GDBStub.h>
#include "ExtDigitalOutput.h"
#include "AqvaServer.h"
#include "Constants.h"
#include "Modules.h"
#include "TimeUtil.h"

#include "Debug.h"
