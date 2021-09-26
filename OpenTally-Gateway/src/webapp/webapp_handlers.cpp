#include "webapp_handlers.h"
#include <FS.h>
#include <SPIFFS.h>
#include "buildconfig.h"

void webapp_handleGetAbout(RequestContext& context)
{
    context.response.json["message"] = "Hello world!";
}

void webapp_handleHtmlFile(RequestContext& request)
{
    String pathFileName = "";
    try
    {
        pathFileName = request.pathVariables.get("filename");
    }
    catch(const std::exception& e)
    {
        //std::cerr << e.what() << '\n';
    }
    
    
    if(pathFileName.isEmpty()) pathFileName = "index.html";
    String filename = String("/html/") + pathFileName;

    if (SPIFFS.exists(filename)) {
        request.rawRequest->send(SPIFFS, filename, "text/html");
    } else {
        request.response.json["error"] = "Not found";
        request.response.setCode(404);
    } 
}

void webapp_handleJsFile(RequestContext& request)
{
    String pathFileName = request.pathVariables.get("filename");
    String filename = String("/js/") + request.pathVariables.get("filename");
    
    if (SPIFFS.exists(filename)) {
        request.rawRequest->send(SPIFFS, filename, "application/javascript");
    } else {
        request.response.json["error"] = "Not found";
        request.response.setCode(404);
    } 
}

void webapp_handleCssFile(RequestContext& request)
{
    String pathFileName = request.pathVariables.get("filename");
    String filename = String("/css/") + request.pathVariables.get("filename");
    
    if (SPIFFS.exists(filename)) {
        request.rawRequest->send(SPIFFS, filename, "text/css");
    } else {
        request.response.json["error"] = "Not found";
        request.response.setCode(404);
    } 
}

void webapp_handleKeyConfigGet(RequestContext& request)
{
    File jsonFile = SPIFFS.open(CONFIG_FILE_NAME);
    DynamicJsonDocument root(4096);
    deserializeJson(root, jsonFile);
    jsonFile.close();

    char response[4096];
    serializeJson(root["key"], response, 2048);
    request.response.sendRaw(200, "application/json", response);

}