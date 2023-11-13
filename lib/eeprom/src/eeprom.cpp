/***************************eeprom.cpp******************************************

    Description:    

*******************************************************************************/
/*------------------------------------------------------------------------------
    Version 0.1     Yasperzee   12'22   Cleaning and refactoring

#TODO:
------------------------------------------------------------------------------*/
#include "eeprom.h"
#include <EEPROM.h>

localEeprom  eeprom_c;

int localEeprom ::read_eeprom(int address) {
  int value = EEPROM.read(address);
  return value;
  }

void localEeprom ::write_eeprom( int addr, int value) {
  EEPROM.write(addr, value);
  delay(10);
  Serial.print("");
  if (EEPROM.commit()) {
    Serial.println("EEPROM successfully committed");
    } 
  else {
    Serial.println("ERROR! EEPROM commit failed");
    }
  delay(10);
  }  

// boolean clear_eeprom(int start_addr, int lenght)
void localEeprom ::clear_eeprom() {
  Serial.print("");
  Serial.print("EEPROM.length: ");
  Serial.println(EEPROM.length());
  Serial.print("");

  for (size_t addr = 0 ; addr < EEPROM.length() ; addr++) {
    int value = EEPROM.read(addr);
    Serial.print(value);
    delay(10);
    }
  Serial.print("");

  for (size_t i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
    //delay(10);
    }
  EEPROM.commit();

  Serial.print("");

  for (size_t addr = 0 ; addr < EEPROM.length() ; addr++) {
    int value = EEPROM.read(addr);
    Serial.print(value);    
    delay(10);
    } 
  }
