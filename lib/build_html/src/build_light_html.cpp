/***************************build_light_html.h.cpp******************************

    Description:    Build basic welcome page 

*******************************************************************************/
/* -----------------------------------------------------------------------------
    Version 0.2     Yasperzee   12'22    Cleaning and refactoring
    Version 0.1     Yasperzee   11'22   Build basic welcome page

#TODO:
------------------------------------------------------------------------------*/

#include "build_light_html.h"

String build_light_html(void) {
    //Values values;
    String webpage;

    // Web Page Heading
    webpage  = "<!DOCTYPE html><html>";
    webpage += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    webpage += "<link rel=\"icon\" href=\"data:,\">";
    webpage += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";   
    webpage += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
    webpage += "</style></head>";

    if(true) {
        webpage += "Welcome to the REST IOT Server on ESP8266";
        }
    else {
        webpage += "<body>";
        webpage += "<p>Node information </p>";
        // CSS to style the on/off buttons
        // Feel free to change the background-color and font-size attributes to fit your preferences
        //webpage += ".button1 { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
        //webpage += ".button2 { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
        webpage += "<p><a href=\"/nodeData\"><button class=\"button1\">GET data</button></a></p>";
        webpage += "<p><a href=\"/nodeInfo\"><button class=\"button2\">GET info</button></a></p>";
        webpage += "<info>";
        webpage +=  "Info: ";
        webpage += "</info>";
        }
    webpage += "</body></html>";
    // The HTTP response ends with another blank line
    webpage += "";

    return (webpage);
    } // build_light_html