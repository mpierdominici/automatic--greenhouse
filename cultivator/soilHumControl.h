#ifndef SOIL_HUMCONTROL_H_
#define SOIL_HUMCONTROL_H_
#include<Arduino.h>
#include<stdint.h>
#include "waterPump.h"
#include "soilHumidity.h"
#include <TimeLib.h>

typedef enum{
  TH;//control tiempo humedad
  MANUAL;//manual, detien el funcionamiento de la bomba
  
}soilHumControl_types_t;


class soilHumControl{
  public:
  soilHumControl();
  void udateDate(tmElements_t currentTime);
  soilHumControl_types_t getMode(void){return mode;};
  void setMode(soilHumControl_types_t nMode);
  void setTimeOut(uint32_t timeBetewenLowHum);
  void setMaxHum(uint8_t hum);//0-100%
  void setMinHum(uint8_t hum);//0-100%
  void updateWaterTankState(bool empty){waterTankIsEmpty=empty;};
  uint8_t getHumActual(void){return humActual};
  void run(void); //actualizo estado de bomba, lectura de sesnores
  private:
  bool waterTankIsEmpty;
  soilHumidity * humGraund;//lectura de humedad
  waterPump * water;//control de la bomba
  uint32_t actualDate;//date actual
  uint32_t nextEvent;//momento en que deve iniciarse el riego
  uint32_t timeInterval;//intervalo a esperar luego de que se seque el suelo
  soilHumControl_types_t mode;//modo acual del control
  uint8_t humMax;
  uint8_t humMin;
  uint8_t humActual;
  

};


#endif /*SOIL_HUMCONTROL_H_*/
