#include <Arduino.h>
#include <Stream.h>
#include <HardwareSerial.h>

#define AQUASMART_H
#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif
extern void printChipDetails();
extern void wifi_event_handler_cb(System_Event_t * event);
extern void print_softap_config(Stream & consolePort, softap_config const& config);
extern void print_system_info(Stream & consolePort);
extern void print_wifi_general(Stream & consolePort);
