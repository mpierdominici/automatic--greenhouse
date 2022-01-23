#ifndef SOIL_HUMCONTROL_H_
#define SOIL_HUMCONTROL_H_
#include<Arduino.h>
#include<stdint.h>
#include "waterPump.h"
#include "soilHumidity.h"
#include <TimeLib.h>

typedef enum{
  TH,//control tiempo humedad
  MANUAL//manual, detien el funcionamiento de la bomba
}soilHumControl_types_t;


class soilHumControl{
  public:
  soilHumControl(soilHumidity * sensHum,waterPump * pump,uint8_t mem2save);
  void udateDate(uint32_t currentTime){actualDate=currentTime;};
  soilHumControl_types_t getMode(void){return mode;};
  void setMode(soilHumControl_types_t nMode);
  void setTimeOut(uint32_t timeBetewenLowHum){timeInterval=timeBetewenLowHum;};
  void setMaxHum(uint8_t hum){humMax=hum;};//0-100%
  void setMinHum(uint8_t hum){humMin=hum;};//0-100%
  void updateWaterTankState(bool empty){waterTankIsEmpty=empty;};
  uint8_t getHumActual(void){return humActual;};
  void run(void); //actualizo estado de bomba, lectura de sesnores
  private:
  bool waterTankIsEmpty;//estado del tanque, tru si esta bacio
  soilHumidity * humGraund;//lectura de humedad
  waterPump * water;//control de la bomba
  uint32_t actualDate;//date actual
  uint32_t nextEvent;//momento en que deve iniciarse el riego
  uint32_t timeInterval;//intervalo a esperar luego de que se seque el suelo
  soilHumControl_types_t mode;//modo acual del control
  uint8_t humMax;//nivel maximo de humedad
  uint8_t humMin;//nivel minimo de humedad
  uint8_t humActual;//ultima medicion de humedad
  

};


#endif /*SOIL_HUMCONTROL_H_*/
