#ifndef EEPROM_H
#define EEPROM_H

/*************************** eeprom.h ******************************************

  Description:  
  
*******************************************************************************/
/*------------------------------------------------------------------------------
  Version 0.1     Yasperzee   12'22     Cleaning and refactoring
 
 #TODO:
------------------------------------------------------------------------------*/
#include "setup.h"
//#include <EEPROM.h>

#define EEPROM_SIZE 512

class localEeprom {
  public:
  void write_eeprom( int addr, int value);
  int read_eeprom(int address);
  void clear_eeprom();
private:
};

#endif // EEPROM_H

