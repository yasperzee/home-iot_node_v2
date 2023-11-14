#ifndef SETUP_H
#define SETUP_H

/*************************** setup.h *******************************************

  Description:  Node setup defined 

*******************************************************************************/
/*------------------------------------------------------------------------------
  Version 0.7     Yasperzee   12'22          Add SHT3x Sensor support
  Version 0.6     Yasperzee   12'22          Add BMP280 & BME280 Sensors
  Version 0.5     Yasperzee   12'22          Add HC-SRO4 Ultrasonic Distance Sensor     
  Version 0.4     Yasperzee   12'22          Cleaning and refactoring
  Version 0.3     Yasperzee   12'22          IR Thermometer support added
  Version 0.2     Yasperzee   12'22          Sensor model and type added
  Version 0.1     Yasperzee   12'22          Imported to platformio

  ----------------------------------------------------------------------------*/
  #include <Arduino.h>

//************ Configurations ************
#define TRACE_INFO

// Select node role
  #define NODE_HTTP_SERVER
  //#define NODE_HTTP_CLIENT
  //#define NODE_MQTT_CLIENT
  //#define NODE_STANDALONE

// Select Sensor
  //#define SENSOR_BME280
  //#define SENSOR_BMP280
  //#define SENSOR_DHT22
  #define SENSOR_SHT3X
  //#define SENSOR_TACOMETER
  //#define SENSOR_IR_THERMOMETER
  //#define SENSOR_ULTRASONIC_DISTANCE
  
  #define APP_SW_VERSION "zee_esp_http_0.2.0_dev"
  #define HW_VERSION "zee_esp_weather_0.1.0_demo"
  #define NODEMCU_STR "ESP-12E" // "ESP-01s"
  #define NODE_FUNCTION "Weather" // e.g. Ultrasonic-distance, Tacometer, IR-Thermometer,  Weather, Environment, Handheld, ...
  #define SENSOR_MODEL_STR "SHT-31" // e.g. Olimex SNS-IR-3-8

  #define BAUDRATE 115200
  #define WIFI_RETRY_TIME 1000
  
  #define HTTP_PORT 80
  #define HTTP_REST_PORT 8080
    // TCP/IP port 1883 is reserved with IANA for use with MQTT.
    // TCP/IP port 8883 is also registered, for using MQTT over SSL.
    //#define MQTT_PORT_SSL    8883
    #define MQTT_PORT    1883

    // IR Thermometer settings
  #define EMISSIVITY 1.00 

    // Increment number for each node
  //#define NODE_ID_STR "Node_00001"  // this is obsolete, use ESP.getChipId();

#endif //SETUP_H