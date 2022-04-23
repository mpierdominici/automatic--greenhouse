#ifndef SOIL_HUMCONTROL_H_
#define SOIL_HUMCONTROL_H_
#include<Arduino.h>
#include<stdint.h>
#include "waterPump.h"
#include "soilHumidity.h"
#include"mnpEeprom.h"
#include <TimeLib.h>

typedef enum{
  TH,//control tiempo humedad
  MANUAL//manual, detien el funcionamiento de la bomba
}soilHumControl_types_t;

#define SOILHUMCONTROL_MODE_MAX_POS 7
#define SOILHUMCONTROL_HUM_MAX_POS 6
#define SOILHUMCONTROL_HUM_MIN_POS 5
#define SOILHUMCONTROL_DELTA_T_POS 0

class soilHumControl{
  public:
  soilHumControl(soilHumidity * sensHum,waterPump * pump,uint8_t mem2save);
  void udateDate(uint32_t currentTime){actualDate=currentTime;};
  soilHumControl_types_t getMode(void){return mode;};
  void setMode(soilHumControl_types_t nMode);
  void setTimeOut(uint32_t timeBetewenLowHum){timeInterval=timeBetewenLowHum;mnpEeprom_write32(timeInterval,memPosHigh-SOILHUMCONTROL_DELTA_T_POS);};
  void setMaxHum(uint8_t hum){humMax=hum;mnpEeprom_write8(humMax,memPosHigh-SOILHUMCONTROL_HUM_MAX_POS);};//0-100%
  void setMinHum(uint8_t hum){humMin=hum;mnpEeprom_write8(humMin,memPosHigh-SOILHUMCONTROL_HUM_MIN_POS);};//0-100%
  void updateWaterTankState(bool empty){waterTankIsEmpty=empty;this->run();};
  uint8_t getHumActual(void){return humActual;};
  void run(void); //actualizo estado de bomba, lectura de sesnores

  uint8_t getHmax(void){return humMax;};
  uint8_t getHmin(void){return humMin;};
  uint32_t getDeltat(void){return timeInterval;};
  
  bool isAuto(void){return mode==TH;};//true si el controlador esta en estado automatico

  private:
  bool startCount; //true para indiciar que esta en la etapa de espera
  bool regando; //true para indicar que me encuentro en la etapa de rigeo

  uint8_t humMax;//nivel maximo de humedad
  uint8_t humMin;//nivel minimo de humedad
  uint32_t timeInterval;//intervalo a esperar luego de que se seque el suelo
  
  bool waterTankIsEmpty;//estado del tanque, tru si esta bacio
  soilHumidity * humGraund;//lectura de humedad
  waterPump * water;//control de la bomba
  uint32_t actualDate;//date actual
  uint32_t nextEvent;//momento en que deve iniciarse el riego
  
  soilHumControl_types_t mode;//modo acual del control
  
  uint8_t humActual;//ultima medicion de humedad
  uint8_t memPosHigh;//posicion de memorias hasta donde se guarda los parametros del control

  

};


#endif /*SOIL_HUMCONTROL_H_*/
