
#define AQUASMART_H

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include <Hash.h>
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include <RtcDS1307.h>
#include "Constants.h"
#include "Modules.h"
#include <ArduinoJson.h>
#include "ExtDigitalOutput.h"
#include "TimeUtil.h"
#include "Ticker.h"

static const String API_VERSION = "v1";
static const String API_URI = "/api/" + API_VERSION + "/";
static const byte startIndex = API_URI.length();

static const String GET_RTC_TIME = "get_rtc_time";
static const String GET_NTP_TIME = "get_ntp_time";
static const String RESTART = "restart";
static const String RESET = "reset";
static const String SYSTEM_SETTINGS = "system_settings";

#define DELAY 1000 /* milliseconds */

class AqvaServerApiHandler : public AsyncWebHandler {
private:
	String _username;
	String _password;
	bool _authenticated;
	uint32_t _startTime;
	Ticker* ntpUpdateTimer;
	StaticJsonBuffer<200>* jsonBuffer = new StaticJsonBuffer<200>;

public:
	AqvaServerApiHandler(String username, String password, ExtDigitalOutput* out)
		:_username(username),
		_password(password),
		_authenticated(false),
		_startTime(0)
	{

	}

	bool canHandle(AsyncWebServerRequest *request) {
		if (request->url().indexOf(API_URI) < 0)
			return false;
		return true;
		/* if(request->url().equalsIgnoreCase("/api/" + API_VERSION + "/")){
		   if(request->method() == HTTP_GET){
			 if(request->hasParam("list"))
			   return true;
			 if(request->hasParam("edit")){
			   request->_tempFile = SPIFFS.open(request->arg("edit"), "r");
			   if(!request->_tempFile)
				 return false;
			 }
			 if(request->hasParam("download")){
			   request->_tempFile = SPIFFS.open(request->arg("download"), "r");
			   if(!request->_tempFile)
				 return false;
			 }
			 return true;
		   }
		   else if(request->method() == HTTP_POST)
			 return true;
		   else if(request->method() == HTTP_DELETE)
			 return true;
		   else if(request->method() == HTTP_PUT)
			 return true;

		 }
		 return false;*/
	}

	void handleGetRtcTime(AsyncWebServerRequest *request)
	{
		AsyncResponseStream *response = request->beginResponseStream("text/json");
		DynamicJsonBuffer jsonBuffer;
		JsonObject &root = jsonBuffer.createObject();
		root["time"] = timeModule->getRtcTimeString();
		root.printTo(*response);
		request->send(response);
	}
	
	void handleSystemSettings(AsyncWebServerRequest *request)
	{
		if (request->method() == HTTP_GET) {			
			AsyncWebServerResponse *response = request->beginResponse(SPIFFS, configFileUrl, "text/json");
			request->send(response);
			return;
		}else if (request->method() == HTTP_POST)
		{
			Serial.println( request->contentType());
			Serial.println( request->contentLength());
			request->send(500);
		}

		request->send(500);
	}

	void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
		if (!index) {
			Serial.printf("BodyStart: %u B\n", total);
		}
		for (size_t i = 0; i<len; i++) {
			Serial.write(data[i]);
		}
		if (index + len == total) {
			Serial.printf("BodyEnd: %u B\n", total);
		}
	}

	void handleGetNtpTime(AsyncWebServerRequest *request)
	{
		//TODO with async udp
		AsyncResponseStream *response = request->beginResponseStream("text/json");
		DynamicJsonBuffer jsonBuffer;
		JsonObject &root = jsonBuffer.createObject();
		root["time"] = timeModule->getDateTimeString(timeModule->ntpTime);
		root.printTo(*response);
		request->send(response);

		timeModule->updateNtpTime();
	}


	void handleRequest(AsyncWebServerRequest *request) {
		if (_username.length() && _password.length() && !request->authenticate(_username.c_str(), _password.c_str()))
			return request->requestAuthentication();
		String command = request->url().substring(startIndex);

		Serial.println("Api command: " + command);

		if (command.equalsIgnoreCase(RESTART))
		{
			request->send(200);
			delay(200);
			ESP.restart();
			return;
		}

		if (command.equalsIgnoreCase(RESET))
		{
			request->send(200);
			delay(200);
			ESP.reset();
			return;
		}

		if(command.equalsIgnoreCase(GET_NTP_TIME))
		{
			handleGetNtpTime(request);
			return;
		}

		if (command.equalsIgnoreCase(GET_RTC_TIME))
		{
			handleGetRtcTime(request);
			return;
		}
		
		if (command.equalsIgnoreCase(SYSTEM_SETTINGS))
		{
			handleSystemSettings(request);
			return;
		}

		request->send(200);
		/*if (request->method() == HTTP_GET) {
			if (request->hasParam("list")) {
				String path = request->getParam("list")->value();
				Dir dir = SPIFFS.openDir(path);
				path = String();
				String output = "[";
				while (dir.next()) {
					fs::File entry = dir.openFile("r");
					if (output != "[") output += ',';
					bool isDir = false;
					output += "{\"type\":\"";
					output += (isDir) ? "dir" : "file";
					output += "\",\"name\":\"";
					output += String(entry.name()).substring(1);
					output += "\"}";
					entry.close();
				}
				output += "]";
				request->send(200, "text/json", output);
				output = String();
			}
			else if (request->hasParam("edit") || request->hasParam("download")) {
				request->send(request->_tempFile, request->_tempFile.name(), String(), request->hasParam("download"));
			}
		}
		else if (request->method() == HTTP_DELETE) {
			if (request->hasParam("path", true)) {
				SPIFFS.remove(request->getParam("path", true)->value());
				request->send(200, "", "DELETE: " + request->getParam("path", true)->value());
			}
			else
				request->send(404);
		}
		else if (request->method() == HTTP_POST) {
			if (request->hasParam("data", true, true) && SPIFFS.exists(request->getParam("data", true, true)->value()))
				request->send(200, "", "UPLOADED: " + request->getParam("data", true, true)->value());
			else
				request->send(500);
		}
		else if (request->method() == HTTP_PUT) {
			if (request->hasParam("path", true)) {
				String filename = request->getParam("path", true)->value();
				if (SPIFFS.exists(filename)) {
					request->send(200);
				}
				else {
					fs::File f = SPIFFS.open(filename, "w");
					if (f) {
						f.write((uint8_t)0x00);
						f.close();
						request->send(200, "", "CREATE: " + filename);
					}
					else {
						request->send(500);
					}
				}
			}
			else
				request->send(400);
		}*/
	}

};
