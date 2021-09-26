#include <Arduino.h>

#include "net/net.h"
#include "webapp/webapp_handlers.h"

//using namespace::placeholders;

// Define shorthand for common types
using RichHttpConfig = RichHttp::Generics::Configs::AsyncWebServer;
using RequestContext = RichHttpConfig::RequestContextType;

bool _webAppServerIsOnWifi = false;
bool _webAppServerIsOnConfigAp = false;


// Use the default auth provider
SimpleAuthProvider authProvider;

// Use the builtin server (ESP8266WebServer for ESP8266, WebServer for ESP32).
// Listen on port 80.
RichHttpServer<RichHttpConfig> _webAppServer(81, authProvider);

void webapp_setup()
{
  _webAppServer
    .buildHandler("/about")
    .on(HTTP_GET, webapp_handleGetAbout);

  _webAppServer
    .buildHandler("/")
    .on(HTTP_GET, webapp_handleHtmlFile);

  _webAppServer
    .buildHandler("/html/:filename")
    .on(HTTP_GET, webapp_handleHtmlFile);

  _webAppServer
    .buildHandler("/js/:filename")
    .on(HTTP_GET, webapp_handleJsFile);

  _webAppServer
    .buildHandler("/css/:filename")
    .on(HTTP_GET, webapp_handleCssFile);

  _webAppServer
    .buildHandler("/api/keycfg")
    .on(HTTP_GET, webapp_handleKeyConfigGet);

  // Builders are cached and should be cleared after we're finished
  _webAppServer.clearBuilders();    
}

void webapp_task(void* parameters)
{
    while(true)
    {
        bool wifiIsConnected = net_wifiIsConnected();
        bool configApIsActive = net_configApIsActive();
        if((_webAppServerIsOnWifi && !wifiIsConnected) || (_webAppServerIsOnConfigAp && !configApIsActive))
        {
            _webAppServer.end();
            Serial.println("[WebApp]::Stopped. Network changed.");
        }

        if(!_webAppServerIsOnWifi && !_webAppServerIsOnConfigAp)
        {
            _webAppServer.begin();

            _webAppServerIsOnWifi = wifiIsConnected;
            _webAppServerIsOnConfigAp = configApIsActive;
            Serial.println("[WebApp]::Server started.");
        }
        
        if(_webAppServerIsOnConfigAp || _webAppServerIsOnWifi)
        {
            //_webAppServer.handleClient();
        }
        vTaskDelay(50);
    }
}