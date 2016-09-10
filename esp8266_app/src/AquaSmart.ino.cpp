# 1 "c:\\users\\kuzya\\appdata\\local\\temp\\tmpblbpki"
#include <Arduino.h>
# 1 "D:/projects/esp8266/AquaSmart/esp8266_app/src/AquaSmart.ino"
#define AQUASMART_H 
#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include "ExtDigitalOutput.h"
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include <Hash.h>
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "AqvaServer.h"
#include "Constants.h"
#include "Modules.h"
#include <time.h>
#include "TimeUtil.h"

const char* DEVICE_ID = "aquasmart";
const char* FIRMWARE_VERSION = "1.0.0";

char* http_username = "admin";
char* http_password = "admin";

char* ssid = "902studio";
char* password = "Emc902NetWifiNew";

char* ac_ssid = "AqvaLightAP";
char* ac_pwd = "12345678";
char* hostName = "aquamart.local";
const int totalExtDigitalPins = 16;
const byte red_signal_pin = 0;
const byte green_signal_pin = 8;


const byte E1 = 1;
const byte E2 = 2;
const byte E3 = 3;
const byte E4 = 4;
const byte E5 = 5;
const byte E6 = 6;
const byte E7 = 7;
const byte E8 = 9;
const byte E9 = 10;
const byte E10 = 11;
const byte E11 = 12;
const byte E12 = 13;
const byte E13 = 14;
const byte E14 = 15;

const byte RELAY_VCC_PIN = 15;
const byte PIN_RTC_SDA = 4;
const byte PIN_RTC_SCL = 5;


int i = 0;
ExtDigitalOutput output(13, 12, 14, 3);
AqvaServer server(http_username, http_password, 80, &output);
TimeUtil* timeModule = new TimeUtil();
void turnOfAllExtDigitalPins();
void setupWifi();
void setup(void);
void loop(void);
void wifi_event_handler_cb(System_Event_t * event);
void print_softap_config(Stream & consolePort, softap_config const& config);
void print_system_info(Stream & consolePort);
void print_wifi_general(Stream & consolePort);
void printChipDetails();
void TestChannelsExtChannels(ExtDigitalOutput output);
#line 68 "D:/projects/esp8266/AquaSmart/esp8266_app/src/AquaSmart.ino"
void turnOfAllExtDigitalPins() {
 for (int i = 0; i <= totalExtDigitalPins; i++)
 {
  output.write(i, 0);
 }
}

void setupWifi() {
 Serial.println("----Configuring WIFI----");
 Serial.println("Configuring access point...");
 WiFi.disconnect(0);

 WiFi.hostname(hostName);
 wifi_station_set_hostname(hostName);
 WiFi.mode(WIFI_STA);
# 94 "D:/projects/esp8266/AquaSmart/esp8266_app/src/AquaSmart.ino"
 wifi_set_event_handler_cb(wifi_event_handler_cb);

 WiFi.begin(ssid, password);


 while (WiFi.status() != WL_CONNECTED) {
  output.write(red_signal_pin, 0);
  i++;
  delay(250);
  output.write(red_signal_pin, 1);
  delay(250);

  Serial.println(WiFi.isConnected());
  if (i > 30) {
   Serial.println();
   Serial.println("Cant connect to WIFI station");
   break;
  }
 }

 Serial.println("");
 Serial.print("Connected to ");
 Serial.println(ssid);
 Serial.print("IP address: ");
 Serial.println(WiFi.localIP());

 long rssi = WiFi.RSSI();
 Serial.print("signal strength (RSSI):");
 Serial.print(rssi);
 Serial.println(" dBm");
 print_wifi_general(Serial);
}


void setup(void) {
 Serial.begin(115200);

 delay(3000);
 Serial.println("AqvaLight: Hello World:)");
 printChipDetails();

 output.begin();
 turnOfAllExtDigitalPins();
 output.write(RELAY_VCC_PIN, 0);

 output.write(red_signal_pin, 0);
 output.write(green_signal_pin, 1);

 timeModule->setup();
 setupWifi();
 server.setupServer();

 output.write(red_signal_pin, 1);
 output.write(green_signal_pin, 0);




 Serial.println(timeModule->getNtpTimeString());
}

void loop(void) {







}
# 1 "D:/projects/esp8266/AquaSmart/esp8266_app/src/Info.ino"
# 23 "D:/projects/esp8266/AquaSmart/esp8266_app/src/Info.ino"
#ifdef ESP8266

extern "C" {

#include "user_interface.h"

#include "Arduino.h"

}

#endif
# 43 "D:/projects/esp8266/AquaSmart/esp8266_app/src/Info.ino"
Stream& ehConsolePort(Serial);
# 59 "D:/projects/esp8266/AquaSmart/esp8266_app/src/Info.ino"
const uint8_t LED_PIN = 1;



const char * const RST_REASONS[] =

{

    "REASON_DEFAULT_RST",

    "REASON_WDT_RST",

    "REASON_EXCEPTION_RST",

    "REASON_SOFT_WDT_RST",

    "REASON_SOFT_RESTART",

    "REASON_DEEP_SLEEP_AWAKE",

    "REASON_EXT_SYS_RST"

};



const char * const FLASH_SIZE_MAP_NAMES[] =

{

    "FLASH_SIZE_4M_MAP_256_256",

    "FLASH_SIZE_2M",

    "FLASH_SIZE_8M_MAP_512_512",

    "FLASH_SIZE_16M_MAP_512_512",

    "FLASH_SIZE_32M_MAP_512_512",

    "FLASH_SIZE_16M_MAP_1024_1024",

    "FLASH_SIZE_32M_MAP_1024_1024"

};



const char * const OP_MODE_NAMES[]

{

    "NULL_MODE",

    "STATION_MODE",

    "SOFTAP_MODE",

    "STATIONAP_MODE"

};



const char * const AUTH_MODE_NAMES[]

{

    "AUTH_OPEN",

    "AUTH_WEP",

    "AUTH_WPA_PSK",

    "AUTH_WPA2_PSK",

    "AUTH_WPA_WPA2_PSK",

    "AUTH_MAX"

};



const char * const PHY_MODE_NAMES[]

{

    "",

    "PHY_MODE_11B",

    "PHY_MODE_11G",

    "PHY_MODE_11N"

};
# 183 "D:/projects/esp8266/AquaSmart/esp8266_app/src/Info.ino"
const char * const EVENT_NAMES[]

{

 "EVENT_STAMODE_CONNECTED",

 "EVENT_STAMODE_DISCONNECTED",

 "EVENT_STAMODE_AUTHMODE_CHANGE",

 "EVENT_STAMODE_GOT_IP",

 "EVENT_STAMODE_DHCP_TIMEOUT",

 "EVENT_SOFTAPMODE_STACONNECTED",

 "EVENT_SOFTAPMODE_STADISCONNECTED",

 "EVENT_SOFTAPMODE_PROBEREQRECVED",

 "EVENT_MAX"

};





const char * const EVENT_REASONS[]

{

    "",

    "REASON_UNSPECIFIED",

    "REASON_AUTH_EXPIRE",

    "REASON_AUTH_LEAVE",

    "REASON_ASSOC_EXPIRE",

    "REASON_ASSOC_TOOMANY",

    "REASON_NOT_AUTHED",

    "REASON_NOT_ASSOCED",

    "REASON_ASSOC_LEAVE",

    "REASON_ASSOC_NOT_AUTHED",

    "REASON_DISASSOC_PWRCAP_BAD",

    "REASON_DISASSOC_SUPCHAN_BAD",

    "REASON_IE_INVALID",

    "REASON_MIC_FAILURE",

    "REASON_4WAY_HANDSHAKE_TIMEOUT",

    "REASON_GROUP_KEY_UPDATE_TIMEOUT",

    "REASON_IE_IN_4WAY_DIFFERS",

    "REASON_GROUP_CIPHER_INVALID",

    "REASON_PAIRWISE_CIPHER_INVALID",

    "REASON_AKMP_INVALID",

    "REASON_UNSUPP_RSN_IE_VERSION",

    "REASON_INVALID_RSN_IE_CAP",

    "REASON_802_1X_AUTH_FAILED",

    "REASON_CIPHER_SUITE_REJECTED",

};



const char * const EVENT_REASONS_200[]

{

    "REASON_BEACON_TIMEOUT",

    "REASON_NO_AP_FOUND"

};



void wifi_event_handler_cb(System_Event_t * event)

{

    ehConsolePort.print(" (");

    ehConsolePort.print(EVENT_NAMES[event->event] + event->event);



    switch (event->event)

    {

        case EVENT_STAMODE_CONNECTED:

            break;

        case EVENT_STAMODE_DISCONNECTED:

            break;

        case EVENT_STAMODE_AUTHMODE_CHANGE:

            break;

        case EVENT_STAMODE_GOT_IP:

            break;

        case EVENT_SOFTAPMODE_STACONNECTED:

        case EVENT_SOFTAPMODE_STADISCONNECTED:

            {

                char mac[32] = {0};

                snprintf(mac, 32, MACSTR ", aid: %d" , MAC2STR(event->event_info.sta_connected.mac), event->event_info.sta_connected.aid);



                ehConsolePort.print(mac);

            }

            break;

    }



    ehConsolePort.println(")");

}



void print_softap_config(Stream & consolePort, softap_config const& config)

{

    consolePort.println();

    consolePort.println(F("SoftAP Configuration"));

    consolePort.println(F("--------------------"));



    consolePort.print(F("ssid:            "));

    consolePort.println((char *) config.ssid);



    consolePort.print(F("password:        "));

    consolePort.println((char *) config.password);



    consolePort.print(F("ssid_len:        "));

    consolePort.println(config.ssid_len);



    consolePort.print(F("channel:         "));

    consolePort.println(config.channel);



    consolePort.print(F("authmode:        "));

    consolePort.println(AUTH_MODE_NAMES[config.authmode]);



    consolePort.print(F("ssid_hidden:     "));

    consolePort.println(config.ssid_hidden);



    consolePort.print(F("max_connection:  "));

    consolePort.println(config.max_connection);



    consolePort.print(F("beacon_interval: "));

    consolePort.print(config.beacon_interval);

    consolePort.println("ms");



    consolePort.println(F("--------------------"));

    consolePort.println();

}



void print_system_info(Stream & consolePort)

{

    const rst_info * resetInfo = system_get_rst_info();

    consolePort.print(F("system_get_rst_info() reset reason: "));

    consolePort.println(RST_REASONS[resetInfo->reason]);



    consolePort.print(F("system_get_free_heap_size(): "));

    consolePort.println(system_get_free_heap_size());



    consolePort.print(F("system_get_os_print(): "));

    consolePort.println(system_get_os_print());

    system_set_os_print(1);

    consolePort.print(F("system_get_os_print(): "));

    consolePort.println(system_get_os_print());



    system_print_meminfo();



    consolePort.print(F("system_get_chip_id(): 0x"));

    consolePort.println(system_get_chip_id(), HEX);



    consolePort.print(F("system_get_sdk_version(): "));

    consolePort.println(system_get_sdk_version());



    consolePort.print(F("system_get_boot_version(): "));

    consolePort.println(system_get_boot_version());



    consolePort.print(F("system_get_userbin_addr(): 0x"));

    consolePort.println(system_get_userbin_addr(), HEX);



    consolePort.print(F("system_get_boot_mode(): "));

    consolePort.println(system_get_boot_mode() == 0 ? F("SYS_BOOT_ENHANCE_MODE") : F("SYS_BOOT_NORMAL_MODE"));



    consolePort.print(F("system_get_cpu_freq(): "));

    consolePort.println(system_get_cpu_freq());



    consolePort.print(F("system_get_flash_size_map(): "));

    consolePort.println(FLASH_SIZE_MAP_NAMES[system_get_flash_size_map()]);

}



void print_wifi_general(Stream & consolePort)

{

    consolePort.print(F("wifi_get_channel(): "));

    consolePort.println(wifi_get_channel());



    consolePort.print(F("wifi_get_phy_mode(): "));

    consolePort.println(PHY_MODE_NAMES[wifi_get_phy_mode()]);

}



void printChipDetails() {

 Serial.println("--------Module details--------");

 Serial.print(F("system_get_time(): "));

 Serial.println(system_get_time());



 uint32_t realSize = ESP.getFlashChipRealSize();

 uint32_t ideSize = ESP.getFlashChipSize();

 FlashMode_t ideMode = ESP.getFlashChipMode();



 Serial.printf("Flash real id:   %08X\n", ESP.getFlashChipId());

 Serial.printf("Flash real size: %u\n\n", realSize);



 Serial.printf("Flash ide  size: %u\n", ideSize);

 Serial.printf("Flash ide speed: %u\n", ESP.getFlashChipSpeed());

 Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));



 if (ideSize != realSize) {

  Serial.println("Flash Chip configuration wrong!\n");

 }

 else {

  Serial.println("Flash Chip configuration ok.\n");

 }

 Serial.println("-------------------------------");

 print_system_info(Serial);

}
# 1 "D:/projects/esp8266/AquaSmart/esp8266_app/src/Test.ino"
#include "ExtDigitalOutput.h"

#include "Constants.h"



void TestChannelsExtChannels(ExtDigitalOutput output) {



 output.write(E1, 1);

 delay(500);

 output.write(E1, 0);



 output.write(E2, 1);

 delay(500);

 output.write(E2, 0);



 output.write(E3, 1);

 delay(500);

 output.write(E3, 0);



 output.write(E4, 1);

 delay(500);

 output.write(E4, 0);



 output.write(E5, 1);

 delay(500);

 output.write(E5, 0);



 output.write(E6, 1);

 delay(500);

 output.write(E6, 0);



 output.write(E7, 1);

 delay(500);

 output.write(E7, 0);



 output.write(E8, 1);

 delay(500);

 output.write(E8, 0);



 output.write(E9, 1);

 delay(500);

 output.write(E9, 0);



 output.write(E10, 1);

 delay(500);

 output.write(E10, 0);



 output.write(E11, 1);

 delay(500);

 output.write(E11, 0);



 output.write(E12, 1);

 delay(500);

 output.write(E12, 0);



 output.write(E13, 1);

 delay(500);

 output.write(E13, 0);
# 121 "D:/projects/esp8266/AquaSmart/esp8266_app/src/Test.ino"
}