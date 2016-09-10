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


int i = 0;
ExtDigitalOutput output(13, 12, 14, 3); // (dataPin/DS, latchPin/STCP, clockPin/SHCP, number of chip),
AqvaServer server(http_username, http_password, 80, &output);
TimeUtil* timeModule = new TimeUtil();

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
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.hostname(hostName);
	wifi_station_set_hostname(hostName);
	WiFi.mode(WIFI_STA);
	/*WiFi.mode(WIFI_AP_STA);
   WiFi.softAP(ac_ssid, ac_pwd);
   IPAddress myIP = WiFi.softAPIP();
   Serial.println("AP IP address: ");
   Serial.println(myIP);
   //
   softap_config config;
   wifi_softap_get_config(&config);
   print_softap_config(Serial, config);*/


   // set_event_handler_cb_stream(Serial);
	wifi_set_event_handler_cb(wifi_event_handler_cb);

	WiFi.begin(ssid, password);
	// Wait for connection

	while (WiFi.status() != WL_CONNECTED) {
		output.write(red_signal_pin, 0);
		i++;
		delay(250);
		output.write(red_signal_pin, 1);
		delay(250);
		// Serial.print(WiFi.status());
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
	// print the received signal strength:
	long rssi = WiFi.RSSI();
	Serial.print("signal strength (RSSI):");
	Serial.print(rssi);
	Serial.println(" dBm");// - See more at: http://www.esp8266.com/viewtopic.php?f=29&t=4209#sthash.qoaknUwV.dpuf
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

	//turn on relay module
	//output.write(RELAY_VCC_PIN, 1);

	Serial.println(timeModule->getNtpTimeString());
}

void loop(void) {
	//server.loopServer();
//	Serial.println(timeModule->getNtpTimeString());
//	delay(1000);
	//testing ext channels
	//TestChannelsExtChannels(output);


}
