#include "Arduino.h"
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
/*
 * Sleep mode
 */
// #define DEEP_SLEEP
// wakeup 5 seconds earlier
#define SLEEP_SAFETY_MARGIN 1 * 1000
// minimum deep sleep duration (must be bigger than SLEEP_SAFETY_MARGIN)
#define MIN_SLEEP_DURATION_MS 20 * 1000
// duration after boot during which no deep sleep can happen
#define STARTUP_ONLINE_DURATION_MS 120 * 1000
// client disconnect timeout
#define WIFI_CLIENT_TIMEOUT 120 * 1000
// memory management
#define HTTP_MIN_HEAP 4096
#define WIFI_CONNECT_TIMEOUT 10000
#define OPTIMISTIC_YIELD_TIME 10000


extern const char* DEVICE_ID;
extern const char* FIRMWARE_VERSION;
extern String g_middleware;

extern String http_username;
extern String http_password;

extern String ssid;
extern String password;

extern String ac_ssid;
extern String ac_pwd;
extern String hostName;
extern String accessToken;
extern String configFileUrl;

extern const int totalExtDigitalPins;
extern const byte red_signal_pin;
extern const byte green_signal_pin;

extern bool loadConfig();
extern bool saveConfig();
extern MD5Builder getHashBuilder();
extern String getHash();
