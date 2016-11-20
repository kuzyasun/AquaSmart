#include "ESPIot.h"

void setupWifi()
{
	unsigned long startTime = millis();
	Serial.println("----Configuring WIFI----");
	Serial.println("Configuring access point...");

	WiFi.disconnect(0);
	/* You can remove the password parameter if you want the AP to be open. */
	//	WiFi.hostname(hostName);
	//	wifi_station_set_hostname((char *)hostName.c_str());
	//	WiFi.mode(WIFI_STA);
	WiFi.mode(WIFI_AP_STA);
	WiFi.softAP(ac_ssid.c_str(), ac_pwd.c_str());
	IPAddress myIP = WiFi.softAPIP();
	Serial.println("AP IP address: ");
	Serial.println(myIP);
	//
	softap_config config;
	wifi_softap_get_config(&config);
	print_softap_config(Serial, config);

	// set_event_handler_cb_stream(Serial);
	wifi_set_event_handler_cb(wifi_event_handler_cb);

	WiFi.begin((char *)ssid.c_str(), (char *)password.c_str());
	// Wait for connection

	while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_CONNECT_TIMEOUT)
	{
		// Serial.print(WiFi.status());
		Serial.println("Connection status:" + WiFi.isConnected());
	}

	if(millis() - startTime > WIFI_CONNECT_TIMEOUT)
		Serial.println("Connection timeout!");

	if (WiFi.status() == WL_CONNECTED)
	{
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
		//	print_wifi_general(Serial);
	}
	else
	{
		Serial.println();
		Serial.println("Cant connect to WIFI station");
	}
}

void setupConfig()
{
	Serial.println("Mounting FS...");

	if (!SPIFFS.begin())
	{
		Serial.println("Failed to mount file system");
		return;
	}

#ifdef PIN_RESET_TO_DEFAULT

	pinMode(PIN_RESET_TO_DEFAULT, INPUT);
	int reset = digitalRead(PIN_RESET_TO_DEFAULT);
	if(reset == HIGH)
	{
		Serial.println("Reseting device to default settings");
		if (!saveConfig())
			Serial.println("Failed to save config");
		else
			Serial.println("Config saved");
	}
#endif

	if (!saveConfig())
	{
		Serial.println("Failed to save config");
	}
	else
	{
		Serial.println("Config saved");
	}

	if (!loadConfig())
	{
		Serial.println("Failed to load config");
	}
	else
	{
		Serial.println("Config loaded");
	}
}
