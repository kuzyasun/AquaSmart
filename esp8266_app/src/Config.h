#include "Arduino.h"
#include "Constants.h"
#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include <FS.h>

extern "C" {
  #include <user_interface.h>
  #include <umm_malloc/umm_malloc.h>
}

/*
 * Configuration
 */
#define DEBUG
#define BROWSER_EVENTS

#ifdef DEBUG
void debug_plain(const char *msg);
void debug_message(const char *module, const char *format, ...);

//browser_event("debug", __VA_ARGS__);

#define DEBUG_PLAIN(msg) debug_plain(msg)
#define DEBUG_MSG(module, format, ...) debug_message(module, format, ##__VA_ARGS__ )
#else
#define DEBUG_PLAIN(msg)
#define DEBUG_MSG(...) if (ESP.getFreeHeap() < g_minFreeHeap) { g_minFreeHeap = ESP.getFreeHeap(); }
#endif

/*
 * Plugins
 */
#define OTA_SERVER
#define CAPTIVE_PORTAL
#define PLUGIN_ONEWIRE
#define PLUGIN_DHT
#define PLUGIN_ANALOG
#define PLUGIN_WIFI

// #define SPIFFS_EDITOR





extern bool loadConfig();
extern bool saveConfig();
extern MD5Builder getHashBuilder();
extern String getHash();
