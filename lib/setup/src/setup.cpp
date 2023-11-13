/***************************setup.cpp ******************************************

  Description:  

*******************************************************************************/
/*------------------------------------------------------------------------------
  Version 0.2     Yasperzee   12'22         Add HC-SRO4 Ultrasonic Distance Sensor     
  Version 0.1     Yasperzee   12'22         Cleaning and refactoring
 
  #TODO:
------------------------------------------------------------------------------*/
#include "setup.h"
#include "debug.h"
#include "eeprom.h"
#include <EEPROM.h>
#include <WiFiManager.h> 
#include "read_sensors.h"

extern void set_callbacks();
extern void set_emissivity();

//EEPROM stuff
extern localEeprom  eeprom_c;
int reboots_eeprom_address = 0; // address to save reboots
int reboots_eeprom_length = sizeof(reboots_eeprom_address); // size of data to save eeprom

uint8 wings_eeprom_address = reboots_eeprom_address+reboots_eeprom_length; // address to save wings
int wings_eeprom_length = sizeof(wings_eeprom_address); // size of data to save eeprom

uint64 emissivity_eeprom_address = wings_eeprom_address+wings_eeprom_length  ; // address to save emissivity
int emissivity_eeprom_length = sizeof(emissivity_eeprom_address); // size of data to save eeprom

void do_setup() {

  Serial.begin(BAUDRATE);
  EEPROM.begin(EEPROM_SIZE);
  //clear_eeprom(); 

#ifdef SENSOR_TACOMETER // Infrared sensor for Tacometer
  pinMode(RPM_PIN, INPUT_PULLUP); 
  // Write PPR (PulsesPerRevolution) for Tachometer to EEPROM
  //eeprom_c.write_eeprom(wings_eeprom_address, WINGS);
#elif defined SENSOR_IR_THERMOMETER 
  //Do something if any...
#elif defined SENSOR_ULTRASONIC_DISTANCE
  //Do something if any...
#endif
  
  
/*
Serial.print("reboots_eeprom_address: ");
Serial.println(reboots_eeprom_address);
Serial.print("reboots_eeprom_length: ");
Serial.println(reboots_eeprom_length);

Serial.print("wings_eeprom_address: ");
Serial.println(wings_eeprom_address);
Serial.print("wings_eeprom_length: ");
Serial.println(wings_eeprom_length);

Serial.print("emissivity_eeprom_address: ");
Serial.println(emissivity_eeprom_address);
Serial.print("emissivity_eeprom_length: ");
Serial.println(emissivity_eeprom_length);
*/
  // read reboots count from EEPROM, increment and write back
  int reboots = eeprom_c.read_eeprom(reboots_eeprom_address);
  reboots++;
  eeprom_c.write_eeprom(reboots_eeprom_address, reboots);

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.
  WiFiManager wifiManager;

  //reset saved WifiManager settings, for debugging  
  //wifiManager.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result
  bool result;
  // result = wifiManager.autoConnect(); // auto generated AP name from chipid
  result = wifiManager.autoConnect("AutoConnectAP"); // anonymous ap
  //result = wifiManager.autoConnect("AutoConnectAP","password"); // password protected ap

  if(!result) {
    Serial.println("Failed to connect WiFi");
    // ESP.restart();
    } 
  else { 
    Serial.println("WiFi connected, IP address: ");
    Serial.println(WiFi.localIP());
    }

  set_callbacks();

#ifdef SENSOR_TACOMETER
  attachInterrupt(digitalPinToInterrupt(RPM_PIN), isr, FALLING);; 
#elif defined SENSOR_IR_THERMOMETER
  //Do something if any...
#elif defined SENSOR_ULTRASONIC_DISTANCE
  //Do something if any..
#endif

  printInfo();
}
