#ifndef MNP_EEPROM_H_
#define MNP_EEPROM_H_
#include<Arduino.h>
#include<stdint.h>

void mnpEeprom_write32(uint32_t data, uint8_t posH);
uint32_t mnpEeprom_read32(uint8_t posH);

void mnpEeprom_write16(uint16_t data, uint8_t posH);
uint16_t mnpEeprom_read16(uint8_t posH);

void mnpEeprom_write8(uint8_t data, uint8_t posH);
uint8_t mnpEeprom_read8(uint8_t posH);

void mnpEeprom_writeFloat(float data, uint8_t posH);
float mnpEeprom_readFloat(uint8_t posH);



#endif /*MNP_EEPROM_H_*/
