#define AQUASMART_H
#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif

#include <RtcDateTime.h>
#include <RtcDS1307.h>
#include <RtcTemperature.h>
#include <RtcUtility.h>
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
#include "Constants.hpp"
#include <time.h>

#define countof(a) (sizeof(a) / sizeof(a[0]))

const char* DEVICE_ID = "aquasmart";
const char* FIRMWARE_VERSION = "1.0.0";

char* http_username = "admin";
char* http_password = "admin";

char* ssid = "902studio";
char* password = "Emc902NetWifiNew";

char* ac_ssid = "AqvaLightAP";
char* ac_pwd = "12345678";
char* hostName = "aquamart-host";
const int totalExtDigitalPins = 16;
const byte red_signal_pin = 0;
const byte green_signal_pin = 8;

//extended digital pins with 74HC595N
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
//VCC of relay module
const byte RELAY_VCC_PIN = 15;
const byte PIN_RTC_SDA = 4;
const byte PIN_RTC_SCL = 5;

RtcDS1307 Rtc;
int i = 0;
ExtDigitalOutput output(13, 12, 14, 3); // (dataPin/DS, latchPin/STCP, clockPin/SHCP, number of chip),
AqvaServer server(http_username, http_password, 80);

void printDateTime(const RtcDateTime& dt)
{
	char datestring[20];

	snprintf_P(datestring,
		countof(datestring),
		PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
		dt.Month(),
		dt.Day(),
		dt.Year(),
		dt.Hour(),
		dt.Minute(),
		dt.Second());
	Serial.println(datestring);
}

void turnOfAllExtDigitalPins() {
  for(int i=0;i<=totalExtDigitalPins;i++)
    {
      output.write(i, 0);
    }
}

void setupWifi(){
	Serial.println("----Configuring WIFI----");
  Serial.println("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.hostname(ac_ssid);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ac_ssid, ac_pwd);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(myIP);

	softap_config config;
	wifi_softap_get_config(&config);
	print_softap_config(Serial, config);

	WiFi.begin(ssid, password);
	// Wait for connection

	while (WiFi.status() != WL_CONNECTED) {
		output.write(red_signal_pin, 0);
		i++;
		delay(250);
		output.write(red_signal_pin, 1);
		delay(250);
    Serial.print(WiFi.status());
		if (i > 20) {
			Serial.println("Cant connect to WIFI station");
			break;
		}
	}

	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
	// print the received signal strength:
	long rssi = WiFi.RSSI();
	Serial.print("signal strength (RSSI):");
	Serial.print(rssi);
	Serial.println(" dBm");// - See more at: http://www.esp8266.com/viewtopic.php?f=29&t=4209#sthash.qoaknUwV.dpuf
	print_wifi_general(Serial);
}

void setupRtc() {
	//--------RTC SETUP ------------
  Serial.print("Compiled date:");
  Serial.println(__DATE__ );
	//SDA SCL
	Rtc.Begin(PIN_RTC_SDA, PIN_RTC_SCL);
	// if you are using ESP-01 then uncomment the line below to reset the pins to
	// the available pins for SDA, SCL
	// Wire.begin(0, 2); // due to limited pins, use pin 0 and 2 for SDA, SCL
	RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
	printDateTime(compiled);

	if (!Rtc.IsDateTimeValid())
	{
		// Common Cuases:
		//    1) first time you ran and the device wasn't running yet
		//    2) the battery on the device is low or even missing
		Serial.println("RTC lost confidence in the DateTime! Setting compilation time.");
		// following line sets the RTC to the date & time this sketch was compiled
		// it will also reset the valid flag internally unless the Rtc device is
		// having an issue
		Rtc.SetDateTime(compiled);
	}

	if (!Rtc.GetIsRunning())
	{
		Serial.println("RTC was not actively running, starting now");
		Rtc.SetIsRunning(true);
	}

	RtcDateTime now = Rtc.GetDateTime();
	Serial.print("RTC time:  ");
	printDateTime(now);
	if (now < compiled)
	{
		Serial.println("RTC is older than compile time!  (Updating DateTime)");
		Rtc.SetDateTime(compiled);
	}
	else if (now > compiled)
	{
		Serial.println("RTC is newer than compile time. (this is expected)");
	}
	else if (now == compiled)
	{
		Serial.println("RTC is the same as compile time! (not expected but all is fine)");
	}
	// never assume the Rtc was last configured by you, so
	// just clear them to your needed state
	Rtc.SetSquareWavePin(DS1307SquareWaveOut_Low);
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

	setupRtc();
	setupWifi();
	server.setupServer();

	output.write(red_signal_pin, 1);
	output.write(green_signal_pin, 0);

	//turn on relay module
	//output.write(RELAY_VCC_PIN, 1);
}

void loop(void) {
	server.loopServer();

	//testing ext channels
	//TestChannelsExtChannels(output);
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

		if(ideSize != realSize) {
			 Serial.println("Flash Chip configuration wrong!\n");
		} else {
			 Serial.println("Flash Chip configuration ok.\n");
		}
		Serial.println("-------------------------------");
		print_system_info(Serial);
}
