#include "AquaSmart.h"

const char* DEVICE_ID = "aquasmart";
const char* FIRMWARE_VERSION = "1.0.0";

String http_username = "admin";
String http_password = "admin";

String ssid = "902studio";
String password = "Emc902NetWifiNew";

String ac_ssid = "AqvaLightAP";
String ac_pwd = "12345678";
String hostName = "aquamart.local";
String accessToken = "128du9as7du12goue8wa98h12fueh9h98";
String configFileUrl = "/config.json";

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
//TODO change to rx or tx;
#define PIN_RESET_TO_DEFAULT = 4;




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


void setup(void) {
	Serial.begin(115200);

	delay(3000);
	Serial.println("AqvaLight: Hello World:)");
	//printChipDetails();

	output.begin();
	turnOfAllExtDigitalPins();
	output.write(RELAY_VCC_PIN, 0);
	output.write(red_signal_pin, 0);
	output.write(green_signal_pin, 1);

	setupConfig();
	timeModule->setup();
	setupWifi();
	server.setupServer();

	output.write(red_signal_pin, 1);
	output.write(green_signal_pin, 0);

	//turn on relay module
	output.write(RELAY_VCC_PIN, 1);
	// timeModule->updateNtpTime();
	Serial.println(timeModule->getNtpTimeString());
}

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

	// output.write(E14, 1);
	// delay(500);
	// output.write(E14, 0);
}

void loop(void) {

	//server.loopServer();
//	Serial.println(timeModule->getNtpTimeString());
//	delay(1000);
	//testing ext channels
	TestChannelsExtChannels(output);
	timeModule->handle();
}
