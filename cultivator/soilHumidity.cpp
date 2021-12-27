#include "soilHumidity.h"
#include "mnpEeprom.h"
soilHumidity::soilHumidity(uint8_t analogPin, uint8_t memAdressGain,uint8_t memAdressOffset){
  pin=analogPin;
  memoryAdressGain=memAdressGain;
  memoryAdressOffset=memAdressOffset;
  gain=mnpEeprom_readFloat(memoryAdressGain);
  offset=mnpEeprom_readFloat(memoryAdressOffset);
  //leer gain
  //leer ofset
}
