#ifndef SOIL_HUMIDITY_H_
#define SOIL_HUMIDITY_H_
#include<Arduino.h>
#include<stdint.h>
#include<stdbool.h>


class soilHumidity{
  public:
  
  soilHumidity(uint8_t analogPin, uint8_t memAdressGain,uint8_t memAdressOffset); //constructor por defecto
  uint8_t getHumidity(void);//devuelve la medicion de la humedad del suelo en un rango de 0 a 100
  calibrateSetHighMesurement(uint8_t teorical);//seteo punto de maxima humedad
  calibrateSetLowMesurement(uint8_t teorical);//seteo punto de minima humedad
  calibrate(void);//calcula la ganancia y el ofset y lo guarda en rom
  
  private:
  
  uint8_t memoryAdressGain;
  uint8_t memoryAdressOffset;
  float gain;
  float offset;
  uint8_t pin;
};


#endif /*SOIL_HUMIDITY_H_*/
