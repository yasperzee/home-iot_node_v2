/*************************** node_handlers_client.h  **********************************

  Description:  

*******************************************************************************/
/*------------------------------------------------------------------------------
   
  Version 0.1     Yasperzee   12'22     Created
 
  #TODO:
------------------------------------------------------------------------------*/
#include "node_handlers_client.h"

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

///#include "node_handlers_server.h"
//#include <ESP8266WebServer.h> 

//ESP8266WebClient rest_client(HTTP_PORT);
// HTTPClient http_client;

//HTTPClient rest_client;
//WiFiClient wifi_client;
//buildJsonDocs  build_json_docs;

  WiFiClient wifi_client;
  HTTPClient http_client;

String serverName = "http://192.168.0.164:8082/node-data";
//String serverName = "http://192.168.0.164:8082/index.html";









void handle_iot_rest_remote_server() {
    Serial.print("[HTTP] begin...\n");
    if (http_client.begin(wifi_client,serverName.c_str()) ) { //Specify request destination	  
      http_client.addHeader("Content-Type", "text/json", false,true);
      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      //int httpCode = http_client.GET();
      int httpCode = http_client.POST("{\"TEMPERATURE\":\"some_temp_value\"}");

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http_client.getString();
          Serial.println(payload);
          }
        } 
      else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http_client.errorToString(httpCode).c_str());
      }
      http_client.end();
      }
    else {
      Serial.printf("[HTTP} Unable to connect\n");
      }
    }
/*
void getNodeData() {
  String temp = build_json_docs.build_json_getdata_html();
  rest_server.send(200, "text/json", temp);
  }
 
  void getNodeInfo() {
  String temp = build_json_docs.build_json_getinfo_html();
  rest_server.send(200, "text/json", temp);
  }

void getNodeDebug() {
  String temp = build_json_docs.build_json_getDebug_html();
  rest_server.send(200, "text/json", temp);
  }
  
void getNodeSettings() {
  String temp = build_json_docs.build_json_getSettings_html();
  rest_server.send(200, "text/json", temp);
  }

  void putNodeSettings() {
  String temp = build_json_docs.build_json_putSettings_html();
  rest_server.send(200, "text/json", temp);
  }

  // Manage not found URL ( Rest)
void handleNotFoundRest() {
  String message = "File Not Found\n";
  message += "URI: ";
  message += rest_server.uri();
  message += "\nMethod: ";
  message += (rest_server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += rest_server.args();
  message += "\n";
  for (uint8_t i = 0; i < rest_server.args(); i++) {
    message += " " + rest_server.argName(i) + ": " + rest_server.arg(i) + "\n";
    }
  Serial.println("File Not Found: ");
  Serial.println(message);
  rest_server.send(404, "text/plain", message);
  }

void restServerRoutingRest() {
  rest_server.on("/", HTTP_GET, []() {
    // Send webpage
    String webpage;
    webpage = build_light_html();
    rest_server.send(200, ("text/html"), (webpage));
    });  
  }

  void handle_iot_rest_remote_client() {
    rest_server.handleClient();
  }

  void set_callbacks() {
  //Associate handler functions to web requests
#ifdef NODE_HTTP_SERVER
  restServerRoutingRest();
  rest_server.on(F("/node-data"), HTTP_GET, getNodeData);
  rest_server.on(F("/node-info"), HTTP_GET, getNodeInfo); 
  rest_server.on(F("/node-debug"), HTTP_GET, getNodeDebug); 
  rest_server.on(F("/node-settings"), HTTP_GET, getNodeSettings); 
  rest_server.on(F("/node-settings"), HTTP_PUT, putNodeSettings);
  rest_server.onNotFound(handleNotFoundRest);        // When a Rest client requests an unknown URI (i.e. something other than "/"), call function "handleNotFoundRest"
  rest_server.begin();  
#elif defined NODE_HTTP_CLIENT
  restServerRoutingRest();
  rest_server.on(F("/node-data"), HTTP_GET, getNodeData);
  rest_server.on(F("/node-info"), HTTP_GET, getNodeInfo); 
  rest_server.on(F("/node-debug"), HTTP_GET, getNodeDebug); 
  rest_server.on(F("/node-settings"), HTTP_GET, getNodeSettings); 
  rest_server.on(F("/node-settings"), HTTP_PUT, putNodeSettings);
  rest_server.onNotFound(handleNotFoundRest);        // When a Rest client requests an unknown URI (i.e. something other than "/"), call function "handleNotFoundRest"
  
#endif
}
*/