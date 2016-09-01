
// #include <ESP8266HTTPClient.h>
#include <RtcDateTime.h>
#include <RtcDS1307.h>
#include <RtcTemperature.h>
#include <RtcUtility.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
// #include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "ExtDigitalOutput.h"
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include <Hash.h>
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "AqvaServer.h"

RtcDS1307 Rtc;
//ESP8266WebServer server(80);
int i = 0;
// initialize the library with the numbers of the interface pins
ExtDigitalOutput output(13, 12, 14, 3); // (dataPin/DS, latchPin/STCP, clockPin/SHCP, number of chip),
const char* http_username = "admin";
const char* http_password = "admin";

const char* ssid = "902studio";
const char* password = "Emc902NetWifiNew";

const char* ac_ssid = "AqvaLightAP";
const char* ac_pwd = "12345678";

//const char * hostName = "esp-async";

const int totalExtDigitalPins = 16;
const byte red_signal_pin = 14;
const byte green_signal_pin = 15;

#define countof(a) (sizeof(a) / sizeof(a[0]))

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
	Serial.println("configuring WIFI");

    Serial.println("Configuring access point...");
    /* You can remove the password parameter if you want the AP to be open. */
    WiFi.hostname(ac_ssid);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ac_ssid, ac_pwd);
    IPAddress myIP = WiFi.softAPIP();
    Serial.println("AP IP address: ");
    Serial.println(myIP);

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
}

void setupRtc() {
	//--------RTC SETUP ------------
  Serial.println("compiled date:");
  Serial.println(__DATE__ );
	//SDA SCL
	Rtc.Begin(4, 5);
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

		Serial.println("RTC lost confidence in the DateTime!");

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
	Serial.println("RTC time:");
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

AqvaServer server(http_username, http_password, 80);

void setup(void) {
	Serial.begin(115200);

	output.begin();
  turnOfAllExtDigitalPins();
  //serverSetup();
	output.write(red_signal_pin, 0);
	output.write(green_signal_pin, 1);

	Serial.println("AqvaLight: Hello World:)");

	setupRtc();
	setupWifi();
	server.setupServer();


	output.write(red_signal_pin, 1);
	output.write(green_signal_pin, 0);
}

void loop(void) {
	//server.handleClient();
  //loopServer();
	//TestChannels();
	server.loopServer();
}
