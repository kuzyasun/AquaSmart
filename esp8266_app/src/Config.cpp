#include "Config.h"
#define AQUASMART_H
#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif


bool loadConfig() {

	File configFile = SPIFFS.open("/config.json", "r");
	if (!configFile) {
		Serial.println("Failed to open config file");
		return false;
	}

	size_t size = configFile.size();
	if (size > 1024) {
		Serial.println("Config file size is too large");
		return false;
	}

	// Allocate a buffer to store contents of the file.
	std::unique_ptr<char[]> buf(new char[size]);

	// We don't use String here because ArduinoJson library requires the input
	// buffer to be mutable. If you don't use ArduinoJson, you may as well
	// use configFile.readString instead.
	configFile.readBytes(buf.get(), size);

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& json = jsonBuffer.parseObject(buf.get());

	if (!json.success()) {
		Serial.println("Failed to parse config file");
		return false;
	}

	http_username = json["http_username"].asString();
	http_password = json["http_password"].asString();

	ssid = json["ssid"].asString();
	password = json["password"].asString();

	ac_ssid = json["ac_ssid"].asString();
	ac_pwd = json["ac_pwd"].asString();
	hostName = json["hostName"].asString();
	accessToken = json["accessToken"].asString();

	Serial.print("Config loaded");
	json.printTo(Serial);

	return true;
}

bool saveConfig() {
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& json = jsonBuffer.createObject();

	json["http_username"] = http_username;
	json["http_password"] = http_password;

	json["ssid"] = ssid;
	json["password"] = password;

	json["ac_ssid"] = ac_ssid;
	json["ac_pwd"] = ac_pwd;
	json["hostName"] = hostName;
	json["accessToken"] = accessToken;

	File configFile = SPIFFS.open("/config.json", "w");
	if (!configFile) {
		Serial.println("Failed to open config file for writing");
		return false;
	}

	json.printTo(configFile);
	return true;
}