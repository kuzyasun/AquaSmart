
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "AqvaServer.h"
#include <functional>
#include "SPIFFSEditor.cpp"
#include "AqvaServerApiHandler.cpp"
#include <ArduinoJson.h>
#include "Constants.h"
#include <RtcDS1307.h>

AqvaServer::AqvaServer(String username, String password, uint16_t listen_port, ExtDigitalOutput* out) :
	http_username(username),
	http_password(password),
	port(listen_port),
	output(out)
{}

void AqvaServer::loopServer() {
	ArduinoOTA.handle();
}

void AqvaServer::setupServer() {
	ws = new AsyncWebSocket("/ws");
	events = new AsyncEventSource("/events");
	server = new AsyncWebServer(port);
	jsonBuffer = new StaticJsonBuffer<200>;

	//Send OTA events to the browser
	ArduinoOTA.onStart([&]() { events->send("Update Start", "ota"); });
	ArduinoOTA.onEnd([&]() { events->send("Update End", "ota"); });
	ArduinoOTA.onProgress([&](unsigned int progress, unsigned int total) {
		char p[32];
		sprintf(p, "Progress: %u%%\n", (progress / (total / 100)));
		events->send(p, "ota");
	});

	ArduinoOTA.onError([&](ota_error_t error) {
		if (error == OTA_AUTH_ERROR) events->send("Auth Failed", "ota");
		else if (error == OTA_BEGIN_ERROR) events->send("Begin Failed", "ota");
		else if (error == OTA_CONNECT_ERROR) events->send("Connect Failed", "ota");
		else if (error == OTA_RECEIVE_ERROR) events->send("Recieve Failed", "ota");
		else if (error == OTA_END_ERROR) events->send("End Failed", "ota");
	});

	ArduinoOTA.setHostname(hostName);
	ArduinoOTA.begin();

	MDNS.addService("http", "tcp", 80);

	SPIFFS.begin();

	ws->onEvent([&](AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {

	});

	server->addHandler(ws);

	events->onConnect([](AsyncEventSourceClient *client) {
		client->send("hello!", NULL, millis(), 1000);
	});
	server->addHandler(events);

	server->addHandler(new SPIFFSEditor(http_username, http_password));
	server->addHandler(new AqvaServerApiHandler(http_username, http_password, output));

	server->on("/heap", HTTP_GET, [&](AsyncWebServerRequest *request) {
		request->send(200, "text/plain", String(ESP.getFreeHeap()));
	});

	server->on("/iot_device_id", HTTP_GET, [&](AsyncWebServerRequest *request) {
		AsyncResponseStream *response = request->beginResponseStream("text/json");
		DynamicJsonBuffer jsonBuffer;
		JsonObject &root = jsonBuffer.createObject();
		root["id"] = DEVICE_ID;
		root["version"] = FIRMWARE_VERSION;
		root.printTo(*response);
		request->send(response);
	});

	server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.htm");

	server->onNotFound([](AsyncWebServerRequest *request) {
		Serial.printf("NOT_FOUND: ");
		if (request->method() == HTTP_GET)
			Serial.printf("GET");
		else if (request->method() == HTTP_POST)
			Serial.printf("POST");
		else if (request->method() == HTTP_DELETE)
			Serial.printf("DELETE");
		else if (request->method() == HTTP_PUT)
			Serial.printf("PUT");
		else if (request->method() == HTTP_PATCH)
			Serial.printf("PATCH");
		else if (request->method() == HTTP_HEAD)
			Serial.printf("HEAD");
		else if (request->method() == HTTP_OPTIONS)
			Serial.printf("OPTIONS");
		else
			Serial.printf("UNKNOWN");
		Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

		if (request->contentLength()) {
			Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
			Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
		}

		int headers = request->headers();
		int i;
		for (i = 0; i < headers; i++) {
			AsyncWebHeader* h = request->getHeader(i);
			Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
		}

		int params = request->params();
		for (i = 0; i < params; i++) {
			AsyncWebParameter* p = request->getParam(i);
			if (p->isFile()) {
				Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
			}
			else if (p->isPost()) {
				Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
			}
			else {
				Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
			}
		}

		request->send(404);
	});

	server->onFileUpload([](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
		if (!index)
			Serial.printf("UploadStart: %s\n", filename.c_str());
		Serial.printf("%s", (const char*)data);
		if (final)
			Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index + len);
	});
	server->onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
		if (!index)
			Serial.printf("BodyStart: %u\n", total);
		Serial.printf("%s", (const char*)data);
		if (index + len == total)
			Serial.printf("BodyEnd: %u\n", total);
	});
	server->begin();

	Serial.println("HTTP server started");
}

void AqvaServer::onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {

}
