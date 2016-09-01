
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include <Hash.h>
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"

class AqvaServer;

class AqvaServer {
  private:
  protected:
    void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
  public:
    String http_username;
    String http_password;
    AqvaServer(String username, String password, uint16_t listenPort);
    uint16_t port;
    AsyncWebSocket *ws;
    AsyncEventSource *events;
    AsyncWebServer *server;
    void loopServer();
    void setupServer();
};
