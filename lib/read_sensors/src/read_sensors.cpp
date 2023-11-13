/***************************read_sensors.cpp************************************

    Description:    Read sensor and return Value
                    Supported sensors: Optic RPM sensors 

*******************************************************************************/
/*------------------------------------------------------------------------------
    Version 0.7     Yasperzee   12'22   Add SHT3x Sensor support
    Version 0.7     Yasperzee   12'22   Add BMP280 & BME280 Sensors
    Version 0.6     Yasperzee   12'22   Add HC-SRO4 Ultrasonic Distance Sensor  
    Version 0.5     Yasperzee   12'22   Cleaning and refactoring
    Version 0.4     Yasperzee   11'22   IR TEMPERATURE sensor support
    Version 0.3     Yasperzee   11'22   Weather stuff removed
    Version 0.2     Yasperzee   11'22   RPM meter support added
    Version 0.1     Yasperzee   06'19   DHT sensors to separate module

#TODO:
------------------------------------------------------------------------------*/

#include "read_sensors.h"
#include "eeprom.h"
#include <EEPROM.h>
#include "ESP8266WiFi.h"
#include <Arduino.h>
#ifdef SENSOR_IR_THERMOMETER
#include <Adafruit_MLX90614.h>
#endif
#if defined(SENSOR_DHT11) || defined(SENSOR_DHT22)
#include <read_DHT_sensors.h>
#endif
#ifdef SENSOR_BMP280
#include <Adafruit_BMP280.h>
#endif
#ifdef SENSOR_BME280
#include <Adafruit_BME280.h>
#endif
#if defined SENSOR_SHT3X
#include "Adafruit_SHT31.h"
#endif

extern localEeprom  eeprom_c;
Values values;

#ifdef SENSOR_TACOMETER// RPM stuff
  extern uint8 wings_eeprom_address;
  float rev;
  float rpm;
  int   oldtime;
  int   newtime;
  float revTime;
#endif

#ifdef SENSOR_IR_THERMOMETER
  // IR Thermometer stuff
  #define EMISSIVITY 1.00  
  extern uint8 emissivity_eeprom_address;
  void set_emissivity();
#endif

#ifdef SENSOR_ULTRASONIC_DISTANCE
  const int trigPin = 12; //D6
  const int echoPin = 14; // D5
  //define sound velocity in cm/uS
  #define SOUND_VELOCITY 0.034
  #define CM_TO_INCH 0.393701
  long duration;
  float distanceCm;
#endif

#if defined SENSOR_TACOMETER
    Values ReadSensors::get_rpm() {
    values.wings = WINGS;
    detachInterrupt(RPM_PIN);
    newtime=millis()-oldtime; //finds the time 
    
    // Read ppr (PulsesPerRevolution) from EEPROM
    int wings= eeprom_c.read_eeprom(wings_eeprom_address);
    if(wings <= 0) wings = 1; // Just in case if EEPROM is NOK. . .
    float RPMnew = rev/wings; 
    values.rpm =(RPMnew/newtime)*60000; //calculates rpm
    oldtime=millis(); //saves the current time
    int newtime_secs =newtime/1000;

    Serial.println("\n___TACHOMETER___");
    Serial.print("rev: ");
    Serial.println(rev);
    Serial.print(values.rpm);
    Serial.println(" RPM");
    Serial.print("Time in secs: ");
    Serial.println(newtime_secs);
    Serial.print("wings: ");
    Serial.println(wings);
    rev=0;
    attachInterrupt(digitalPinToInterrupt(RPM_PIN), isr, FALLING);

    return values;
    }

  //IRAM_ATTR void ReadSensors:: isr() {
  IRAM_ATTR void isr() {
    rev++;
    //digitalWrite (DEBUG_PIN, LOW);  
    //delay(2);  // Some Delay
    //digitalWrite (DEBUG_PIN, HIGH); 
    }
#endif

#if defined SENSOR_IR_THERMOMETER
Values ReadSensors::get_ir_temperature() {

    Adafruit_MLX90614 mlx = Adafruit_MLX90614();

    // init ir_temp sensor
    if (!mlx.begin()) {
        Serial.println("Error connecting to MLX sensor. Check wiring.");
        //while (1);
        }
    else {
        set_emissivity();
        }            

    Serial.print("MLX90614 ObjectTemperature in C: ");
    values.ir_object_temp = mlx.readObjectTempC();
    Serial.println(values.ir_object_temp);

    Serial.print("MLX90614 Ambient Temperature in C: ");
    values.ir_ambient_temp = mlx.readObjectTempC();
    Serial.println(values.ir_ambient_temp);
    Serial.println();

    return values;
    }

void ReadSensors::set_emissivity() {
  //set new emissivity if not same as current one
  Serial.println("\nAdafruit MLX90614 Emissivity Setter.");
  // read current emissivity
  double curr_emiss;
   
  curr_emiss = mlx.readEmissivity();
  if (isnan(curr_emiss)) { // try again
    delay (100);
    curr_emiss = mlx.readEmissivity();
    }
  Serial.print("Current emissivity = "); 
  Serial.println(curr_emiss);
 
  //previous_emissivity 
  Serial.print("values.emissivity = "); 
  Serial.println(values.emissivity);

  if(fabs(curr_emiss - values.emissivity)) {
    mlx.writeEmissivity(EMISSIVITY); // this does the 0x0000 erase write
    //Write emissivity for IR Thermometr to EEPROM
    eeprom_c.write_eeprom(emissivity_eeprom_address, EMISSIVITY);
    values.emissivity= mlx.readEmissivity();
   // delay(100);
    Serial.print("New emissivity: "); 
    Serial.println(mlx.readEmissivity());
    Serial.println("DONE. Restart the module.");
    }
  }
#endif // SENSOR_IR_THERMOMETER

#if defined SENSOR_ULTRASONIC_DISTANCE
Values ReadSensors::ReadUltrasonicSensor() {
  Serial.println("\nUltrasonic Sensor HC-SR04");
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  values.distanceCm = distanceCm;
  // Prints the distance on the Serial Monitor
  // Serial.print("duration: ");
  // Serial.println(duration);
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.println();

/*
  if(values.distanceCm >= minDist && values.distanceCm <= maxDist) {
    digitalWrite(relayPin, LOW);
    delay(lightsOnDelay);
    digitalWrite(relayPin, HIGH);
    }
*/
  return values;
  }
#endif

#ifdef SENSOR_BMP280
Values ReadSensors::read_bmp280()
    {
    #define SEALEVELPRESSURE_HPA (1013.25) //for Altitude

    Adafruit_BMP280 bmp280;

    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    if (!bmp280.begin(BMP280_ADDR, BMP280_CHIPID))
        {// continue anyway but show valeus as "-999.99" (ERROR_VALUE)
        Serial.println("BMP280 init FAIL!!");
        values.temperature = ERROR_VALUE;
        values.pressure = ERROR_VALUE;
        values.altitude = ERROR_VALUE;
        values.fail_count ++;
        }
    else
        {
        values.temperature  = (int)(bmp280.readTemperature() * 100 + .5); 
        values.temperature = values.temperature /100; // 2 decimals

        values.pressure = (int)(bmp280.readPressure()/100 * 100 + .5);
        values.pressure = values.pressure / 100; // 2 decimals

        values.altitude = (int)(bmp280.readAltitude(SEALEVELPRESSURE_HPA)* 100 + .5);
        values.altitude =  values.altitude / 100; // 2 decimals

        #ifdef TRACE_INFO
        Serial.print("BMP280: Temperature: ");
        Serial.println(values.temperature);
        Serial.print("BMP280: Pressure   : ");
        Serial.println(values.pressure);
        Serial.print("BMP280: Altitude   : ");
        Serial.println(values.altitude);
        #endif
        #ifdef NODE_FEATURE_AMBIENT_LIGHT
            pinMode(ALS_PIN, INPUT);
            values.als = analogRead(ALS_PIN); //Read light levelM
            //float reading = analogRead(ALS_PIN); //Read light level
            //float square_ratio = reading / 1023.0; //Get percent of maximum value (1023)
            //square_ratio = pow(square_ratio, 2.0);
            //values.als = reading;
          #ifdef TRACE_INFO
          Serial.print("temt6000: ");
          Serial.print(values.als);
          Serial.print("\n");
          #endif
        #endif
        }
    return values;
    } //read_bmp280
#endif

#ifdef SENSOR_BME280
Values ReadSensors::read_bme280()
    {
    #define SEALEVELPRESSURE_HPA (1013.25) //for Altitude  
    //BME280_ADDRESS                (0x77)
    //BME280_ADDRESS_ALTERNATE      (0x76)

    //double tmp_value;

    Adafruit_BME280 bme280;
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    //Wire.setClock(100000);

    if(!bme280.begin(BME280_ADDRESS_ALTERNATE))
        {// continue anyway but show valeus as "-999.99" (ERROR_VALUE)
        Serial.println("BME280 init FAIL!!");
        values.temperature  = ERROR_VALUE;
        values.humidity     = ERROR_VALUE;
        values.pressure     = ERROR_VALUE;
        values.altitude     = ERROR_VALUE;

        values.fail_count ++;
        }
    else
        {
        values.temperature  = (int)(bme280.readTemperature() * 100 + .5); 
        values.temperature = values.temperature /100; // 2 decimals

        values.humidity = (int)( bme280.readHumidity() * 100 + .5);
        values.humidity = values.humidity / 100; // 2 decimals
        
        values.pressure = (int)(bme280.readPressure()/100 * 100 + .5);
        values.pressure = values.pressure / 100; // 2 decimals

        values.altitude = (int)(bme280.readAltitude(SEALEVELPRESSURE_HPA)* 100 + .5);
        values.altitude =  values.altitude / 100; // 2 decimals

        #ifdef TRACE_INFO
        Serial.print("BME280: Temperature: ");
        Serial.println(values.temperature);
        Serial.print("BME280: Humidity: ");
        Serial.println(values.humidity);
        Serial.print("BME280: Pressure: ");
        Serial.println(values.pressure);
        Serial.print("BME280: Altitude: ");
        Serial.println(values.altitude);
        #endif

        #ifdef NODE_FEATURE_AMBIENT_LIGHT
            pinMode(ALS_PIN, INPUT);
            float reading = analogRead(ALS_PIN); //Read light level
            //float square_ratio = reading / 1023.0; //Get percent of maximum value (1023)
            //square_ratio = pow(square_ratio, 2.0);
            values.als = reading;
          #ifdef TRACE_INFO
            Serial.print("ALS: ");
            Serial.println(values.als);
          #endif
        #endif
        }
    return values;
} //read_bme280
#endif

#if defined SENSOR_SHT3X
Adafruit_SHT31 sht31 = Adafruit_SHT31();

Values ReadSensors::read_sht3x()
{
 if (! sht31.begin(0x44)) {   //Set to 0x45 for alternate i2c addr
    values.temperature  = ERROR_VALUE;
    values.humidity     = ERROR_VALUE;
    Serial.println("Couldn't find SHT31");
  }
else {
  values.temperature  = (int)(sht31.readTemperature() * 100 + .5); 
  values.temperature = values.temperature /100; // 2 decimals

  values.humidity = (int)( sht31.readHumidity() * 100 + .5);
  values.humidity = values.humidity / 100; // 2 decimals

    if (! isnan(values.temperature)) {  // check if 'is not a number'
      Serial.print("Temp *C = "); Serial.print(values.temperature); Serial.print("\t\t");
      } 
    else { 
      values.temperature  = ERROR_VALUE;
      Serial.println("Failed to read temperature");
      }
  
  if (! isnan(values.humidity)) {  // check if 'is not a number'
    Serial.print("Hum. % = "); Serial.println(values.humidity);
    } 
  else { 
    values.humidity     = ERROR_VALUE;
    Serial.println("Failed to read humidity");
    }
  }
    return values;
 }
#endif