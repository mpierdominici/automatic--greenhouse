#include <EEPROM.h>

void mnpEeprom_write16(uint16_t data, uint8_t posH) {

  uint8_t tempData;

  tempData = 0x00ff & data;
  EEPROM[posH - 1] = tempData;
  tempData = (0xff00 & data) >> 8;
  EEPROM[posH] = tempData;

}
uint16_t mnpEeprom_read16(uint8_t posH) {
  uint16_t tempData = 0;

  tempData = EEPROM[posH - 1];
  tempData = tempData + (EEPROM[posH] << 8);
  return tempData;
}




void mnpEeprom_write8(uint8_t data, uint8_t posH) {
  EEPROM[posH] = data;
}
uint8_t mnpEeprom_read8(uint8_t posH) {
  return EEPROM[posH];
}

void mnpEeprom_writeFloat(float data, uint8_t posH) {

  posH = posH - sizeof(data);
  uint8_t* p = (uint8_t*)(void*)&data;
  for (int i = 0; i < sizeof(data); i++)
    EEPROM.write(posH++, *p++);

}
float mnpEeprom_readFloat(uint8_t posH) {
  
  float value = 0.0;
  posH = posH - sizeof(value);
  uint8_t* p = (uint8_t*)(void*)&value;
  for (int i = 0; i < sizeof(value); i++)
    *p++ = EEPROM.read(posH++);
  return value;
}

void mnpEeprom_write32(uint32_t data, uint8_t posH){
  posH = posH - sizeof(data);
  uint8_t* p = (uint8_t*)(void*)&data;
  for (int i = 0; i < sizeof(data); i++)
    EEPROM.write(posH++, *p++);
}

uint32_t mnpEeprom_read32( uint8_t posH){
    uint32_t value = 0;
  posH = posH - sizeof(value);
  uint8_t* p = (uint8_t*)(void*)&value;
  for (int i = 0; i < sizeof(value); i++)
    *p++ = EEPROM.read(posH++);
  return value;
}
