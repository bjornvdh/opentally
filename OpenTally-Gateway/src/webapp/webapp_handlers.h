#ifndef __WEBAPP_HANDLERS_H
#define __WEBAPP_HANDLERS_H


#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <RichHttpServer.h>
#include <functional>


// Define shorthand for common types
using RichHttpConfig = RichHttp::Generics::Configs::AsyncWebServer;
using RequestContext = RichHttpConfig::RequestContextType;

void webapp_handleGetAbout(RequestContext& context);
void webapp_handleHtmlFile(RequestContext& context);
void webapp_handleJsFile(RequestContext& context);
void webapp_handleCssFile(RequestContext& context);
void webapp_handleKeyConfigGet(RequestContext& context);
#endif