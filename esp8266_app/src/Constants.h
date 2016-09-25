
#include "Arduino.h"

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

// extended digital pins with 74HC595N
extern const byte E1;
extern const byte E2;
extern const byte E3;
extern const byte E4;
extern const byte E5;
extern const byte E6;
extern const byte E7;
extern const byte E8;
extern const byte E9;
extern const byte E10;
extern const byte E11;
extern const byte E12;
extern const byte E13;
extern const byte E14;
extern const byte RELAY_VCC_PIN;
extern const byte RELAY_VCC_PIN;
extern const byte PIN_RTC_SDA;
extern const byte PIN_RTC_SCL;
extern const byte PIN_RESET_TO_DEFAULT;
