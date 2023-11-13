/*************************build_json_docs.cpp***********************************

    Description:     Build responses

*******************************************************************************/
/* -----------------------------------------------------------------------------
    Version 0.8     Yasperzee   12'22   Add SHT3x Sensor support
    Version 0.7     Yasperzee   12'22   Add BMP280 & BME280 Sensors
    Version 0.4     Yasperzee   12'22   Add HC-SRO4 Ultrasonic Distance Sensor 
    Version 0.3     Yasperzee   12'22   Cleaning and refactoring
    Version 0.2     Yasperzee   11'22   IR Thermometer sensor support
    Version 0.1     Yasperzee   11'22   RPM measurement 

#TODO:
------------------------------------------------------------------------------*/
//#include <Arduino.h>
#include "ArduinoJson.h"
#include "build_json_docs.h"
#include "eeprom.h"
#include "read_sensors.h"
#include "ESP8266WiFi.h"
#include "setup.h"

extern int reboots_eeprom_address; // EEPROM address to save reboots
extern uint8 wings_eeprom_address;

extern float rev;
extern float revTime;
extern int new_emissivity;
extern Values values;

const int relayPin= 13; // D7
const int minDist = 60; //Cm
const int maxDist = 120; //Cm
const int lightsOnDelay = 5000; // ms

extern localEeprom  eeprom_c;

ReadSensors read_sensors;

String buildJsonDocs::build_json_getdata_html(void) {
    String webpage;
    StaticJsonDocument<500> root;
#ifdef SENSOR_TACOMETER
    values = read_sensors.get_rpm();
    root["RPM"] = values.rpm;
#elif defined SENSOR_IR_THERMOMETER
    read_sensors.get_ir_temperature();
    root["IR_TEMP_AMBIENT: "] = values.ir_ambient_temp;
    root["IR_TEMP_OBJECT: "] = values.ir_object_temp;
#elif defined SENSOR_ULTRASONIC_DISTANCE
    read_sensors.ReadUltrasonicSensor();
    root["DISTANCE: "] = values.distanceCm;
#elif defined SENSOR_DHT22
    read_sensors.read_dhtXXX();
    root["DHT_T: "] = values.temperature;
    root["DHT_H: "] = values.humidity;
 #elif defined SENSOR_BMP280
    read_sensors.read_bmp280();
    root["BMP280_T: "] = values.temperature;
    root["BMP280_P: "] = values.pressure;
    root["BMP280_A: "] = values.altitude;
#elif defined SENSOR_BME280
    read_sensors.read_bme280();
   // Serial.printf("BME280: Temperature: %f \n", values.temperature);
    root["BME280_T: "] = values.temperature;
    root["BME280_H: "] = values.humidity;
    root["BME280_P: "] = values.pressure;
    root["BME280_A: "] = values.altitude;
#elif defined SENSOR_SHT3X
    read_sensors.read_sht3x();
    root["SHT3X_T: "] = values.temperature;
    root["SHT3X_H: "] = values.humidity;
#endif
    //Store JSON in String variable  
    serializeJson(root, webpage);

    return webpage;
    }

String buildJsonDocs::build_json_getinfo_html(void) {
    String webpage;
    StaticJsonDocument<500> root;
   // DynamicJsonDocument<500> root;
    root["RSSI"] = WiFi.RSSI();
    root["NODE_IP"] = WiFi.localIP();
    //root["MAC"] = WiFi.macAddress();
    //root["VCC"] = ESP.getVcc();
    root["CHIPID"] = ESP.getChipId();
    //root["NODETYPE"] = NODEMCU_STR;
    //Store JSON in String variable  
    serializeJson(root, webpage);
    Serial.print("Info  : ");
    Serial.println(webpage);

    return webpage;
    }

String buildJsonDocs::build_json_getDebug_html(void) {
    String webpage;
    StaticJsonDocument<500> root;
    //DynamicJsonDocument<500> root;
    //root["CORE_VERSION"] = ESP.getCoreVersion();
    //root["SDK_VERSION"] = ESP.getSdkVersion();
    root["APP_SW"] = APP_SW_VERSION;
    root["HardWare"] = HW_VERSION;
    root["NODE_FUNCTION"] = NODE_FUNCTION;
    root["SENSOR_MODEL_STR"] = SENSOR_MODEL_STR;
    int reboots = eeprom_c.read_eeprom(reboots_eeprom_address);
    //Serial.print("Reboots: ");
    //Serial.println(reboots);
#ifdef SENSOR_TACOMETER
    root["Revolutions: "] = rev;
    root["Rev.time: "] = revTime;
#elif defined SENSOR_IR_THERMOMETER
    // Add something if any...
#endif
    root["Reboots: "] = reboots;
    //Store JSON in String variable  
    serializeJson(root, webpage);

    return webpage;
    }

String buildJsonDocs::build_json_getSettings_html(void) {
    String webpage;
    StaticJsonDocument<500> root;
   // DynamicJsonDocument<500> root;
    //root["Settings"] = "GET n/a yet";
    root["Node function "] = NODE_FUNCTION;
    root["Sensor model "] = SENSOR_MODEL_STR;
#ifdef SENSOR_TACOMETER
   values.wings= eeprom_c.read_eeprom(wings_eeprom_address);
    root["RPM wings "] = values.wings;
#elif defined SENSOR_IR_THERMOMETER
    root["Emissivity "] = values.emissivity;
#endif
    //Store JSON in String variable  
    serializeJson(root, webpage);

    return webpage;
    }

String buildJsonDocs::build_json_putSettings_html(void) {
    String webpage;
    
    StaticJsonDocument<500> root;
   // DynamicJsonDocument<500> root;
    root["Settings"] = " Update settins n/a yet";
    eeprom_c.write_eeprom(reboots_eeprom_address, 0);

    // things to updated wia PUT, saved to EEPROM, username & password required
        //  ssid, password  obsolete, handled by WiFiManager
        //  SENSOR_STR
        //  wings a.k.a ppr -> PulsesPerRevolution for Tachometer
        //  emissivity for IR Thermometer
        //  Sensor polarity NC, NO

        //  Hidden ones: reset Reboot counter
    
    //Store JSON in String variable  
    serializeJson(root, webpage);

    return webpage;
    }

#ifdef SENSOR_DHT22
void read_dht_sensor() {
    Serial.print("read_dht_sensor  ");
while(true) {
    read_sensors.read_dhtXXX();
  
    Serial.print("DHT_T: ");
    Serial.println(values.temperature);
    Serial.print("DHT_H: ");
    Serial.println(values.humidity);
        }
    }
#endif

#ifdef SENSOR_BMP280
void read_bmp280() {
    Serial.print("read_BMP280_sensor  ");
    read_sensors.read_bmp280();

    Serial.print("BMP280_T: ");
    Serial.println(values.temperature);
    Serial.print("BMP280_P: ");
    Serial.println(values.pressure);

    }
#endif

#if defined SENSOR_ULTRASONIC_DISTANCE
// Standalone, so stays here forever, no Web/Wifi activated, on ESP-01 Rx(gpio1) is used for relay as  Ultrasonic sensor reserve gpio0 and gpio2 for i2c
void switch_light() {
    pinMode(relayPin, OUTPUT);
    Serial.print("switch_light?  ");
while(true) {
    read_sensors.ReadUltrasonicSensor();
    if(values.distanceCm >= minDist && values.distanceCm <= maxDist) {
        Serial.println("YES!");
        digitalWrite(relayPin, LOW);
        delay(lightsOnDelay);
        digitalWrite(relayPin, HIGH);
        }
    }
}
#endif
