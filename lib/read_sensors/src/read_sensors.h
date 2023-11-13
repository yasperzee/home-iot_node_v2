#ifndef READ_SENSORS_H
#define READ_SENSORS_H

/*************************** read_sensors.h ************************************
 
  Description:  
  
*******************************************************************************/
/*------------------------------------------------------------------------------
  Version 0.7     Yasperzee   12'22     Add SHT3x Sensor support
  Version 0.6     Yasperzee   12'22     Add HC-SRO4 Ultrasonic Distance Sensor  
  Version 0.5     Yasperzee   12'22     Cleaning and refactoring
  Version 0.4     Yasperzee   11'22     IR TEMPERATURE sensor support
  Version 0.3     Yasperzee   11'22     Weather stuff removed
  Version 0.2     Yasperzee   11'22     RPM support
  Version 0.1     Yasperzee   11'22     Imported to platformio

#TODO:
------------------------------------------------------------------------------*/
//#include <Arduino.h>
#include "setup.h"

// constants
const int ERROR_VALUE = -999.99;

#if defined SENSOR_TACOMETER // Tachometer settings
  #define WINGS 1  // PulsesPerRevolution
  #define RPM_PIN 2 // ESP-01
  //#define RPM_PIN 5 // D1 on NodeMcu
#endif

#if defined SENSOR_BMP280
  //#define BMP280_ADDR BMP280_ADDRESS //(0x77) SDO = HIGH
#define BMP280_ADDR BMP280_ADDRESS_ALT  //(0x76) SDO = LOW
#endif

// #define ALTITUDE 119.0 // Altitude of Tampere-Pirkkala airport, Finland. In meters
//#define ALTITUDE 129.0  // Altitude of Kalkunvuori, Tampere Finland. In meters

  //#define I2C_SCL_PIN 2 //ESP01 
  //#define I2C_SDA_PIN 0 //ESP01 
  #define I2C_SCL_PIN D1 //Lolin 
  #define I2C_SDA_PIN D2 // Lolin
  //#define DEBUG_PIN 0 // e.g. for external LED

// values for sensor readings
struct Values {
    double temperature    = ERROR_VALUE;
    double humidity       = ERROR_VALUE;
    double pressure       = ERROR_VALUE;
    double altitude       = ERROR_VALUE;
    double als            = ERROR_VALUE;
    double vcc_batt       = ERROR_VALUE;
    double rpm            = ERROR_VALUE;
    int wings             = 1;
    double emissivity     = EMISSIVITY;
    double ir_object_temp = ERROR_VALUE;
    double ir_ambient_temp= ERROR_VALUE;
    double distanceCm     = ERROR_VALUE;
    int fail_count        = 0;
    };

class ReadSensors {
  public:
  #if defined SENSOR_TACOMETER
    Values get_rpm(void);
    
    // IRAM_ATTR void isr();
  #endif  
  #if defined SENSOR_IR_THERMOMETER
    Values get_ir_temperature();
    void set_emissivity();
  #endif
  #if defined SENSOR_ULTRASONIC_DISTANCE
    Values ReadUltrasonicSensor(); 
  #endif
  #if defined(SENSOR_DHT11) || defined(SENSOR_DHT22)
    Values read_dhtXXX(void);
  #endif
  #if defined SENSOR_BMP280
    Values read_bmp280();
  #endif
  #if defined SENSOR_BME280
    Values read_bme280();
  #endif
    #if defined SENSOR_SHT3X
    Values read_sht3x();
  #endif
  
   private:
};

// Functions
void IRAM_ATTR isr(void); // ToDo: move to Class

#endif // READ_SENSORS_H


