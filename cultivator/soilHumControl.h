#ifndef SOIL_HUMCONTROL_H_
#define SOIL_HUMCONTROL_H_
#include<Arduino.h>
#include<stdint.h>
#include "waterPump.h"
#include "soilHumidity.h"
#include <TimeLib.h>

typedef enum{
  HUMEDAD;
  TIEMPO;
  MANUAL;
  
}soilHumControl_types_t;


class soilHumControl{
  public:
  soilHumControl();
  udateDate();
  getMode();
  setMode();
  setTimeOut();
  setMaxHum();
  setMinHum();
  updateWaterTankState();

  private:
  bool waterTankIsEmpty;
  soilHumidity * humGraund;
  waterPump * water;
  tmElements_t actualDate;
  tmElements_t nextEvent;
  soilHumControl_types_t mode;
  

};


#endif /*SOIL_HUMCONTROL_H_*/
