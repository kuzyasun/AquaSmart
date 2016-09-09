#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include <Hash.h>
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include <RtcDS1307.h>
#include "Constants.hpp"
#include <ArduinoJson.h>
#include "ExtDigitalOutput.h"
#include <RtcDS1307.h>

static const String API_VERSION = "v1";
static const String API_URI = "/api/" + API_VERSION + "/";
static const byte startIndex = API_URI.length() - 1;

class AqvaServerApiHandler : public AsyncWebHandler {
private:
	String _username;
	String _password;
	bool _authenticated;
	uint32_t _startTime;
	StaticJsonBuffer<200>* jsonBuffer = new StaticJsonBuffer<200>;

public:
	AqvaServerApiHandler(String username, String password, ExtDigitalOutput* out, RtcDS1307* rtc)
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
		root["id"] = DEVICE_ID;
		root["version"] = FIRMWARE_VERSION;
		root.printTo(*response);
		request->send(response);
	}


	void handleRequest(AsyncWebServerRequest *request) {
		if (_username.length() && _password.length() && !request->authenticate(_username.c_str(), _password.c_str()))
			return request->requestAuthentication();
		String command = request->url().substring(startIndex);

		Serial.println(command);

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

	void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
		if (!index) {
			if (!_username.length() || request->authenticate(_username.c_str(), _password.c_str())) {
				_authenticated = true;
				request->_tempFile = SPIFFS.open(filename, "w");
				_startTime = millis();
			}
		}
		if (_authenticated && request->_tempFile) {
			if (len) {
				request->_tempFile.write(data, len);
			}
			if (final) {
				request->_tempFile.close();
			}
		}
	}
};
