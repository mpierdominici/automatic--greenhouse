#include "soilHumidity.h"
#include "mnpEeprom.h"
#define AMAUNT_SAMPLES 10
soilHumidity::soilHumidity(uint8_t analogPin, uint8_t memAdressGain, uint8_t memAdressOffset) {
  pin = analogPin;
  memoryAdressGain = memAdressGain;
  memoryAdressOffset = memAdressOffset;
  gain = mnpEeprom_readFloat(memoryAdressGain);
  offset = mnpEeprom_readFloat(memoryAdressOffset);
  //leer gain
  //leer ofset
}

soilHumidity::calibrate(void) {
  gain = (calHighTeo - calLowTeo) / (calHighMes - calLowMes);
  offset = calLowTeo - gain * calLowMes;
  mnpEeprom_writeFloat(gain, memoryAdressGain);
  mnpEeprom_writeFloat(offset, memoryAdressOffset);

}

soilHumidity::calibrateSetHighMesurement(uint8_t teorical) {
  uint32_t acumulator = 0;
  calHighTeo = teorical;
  for (uint8_t i = 0; i < AMAUNT_SAMPLES; i++) {
    acumulator = acumulator + analogRead(pin);
    
  }
  calHighMes = acumulator / AMAUNT_SAMPLES;
}
soilHumidity::calibrateSetLowMesurement(uint8_t teorical) {
  uint32_t acumulator = 0;
  calLowTeo = teorical;
  for (uint8_t i = 0; i < AMAUNT_SAMPLES; i++) {
    acumulator = acumulator + analogRead(pin);
    
  }
  calLowMes = acumulator / AMAUNT_SAMPLES;
}

uint8_t soilHumidity::getHumidity(void) {
  uint32_t acumulator = 0;
  float tempData=0;
  for (uint8_t i = 0; i < AMAUNT_SAMPLES; i++) {
    acumulator = acumulator + analogRead(pin);
    
  }
  tempData=((acumulator / AMAUNT_SAMPLES) * gain + offset);
  if(tempData>SOIL_HUM_MAX_O){
    tempData=SOIL_HUM_MAX_O;
  }
  if(tempData<SOIL_HUM_MIN_O){
    tempData=SOIL_HUM_MIN_O;
  }
  return  (uint8_t)tempData;
}
