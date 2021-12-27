
#ifndef WATER_PUMP_H_
#define WATER_PUMP_H_
#include<Arduino.h>
#include<stdint.h>
#include<stdbool.h>


class waterPump{
  public:
  waterPump(uint8_t waterPumpPin); //constructor por defecto
  writeSpeed(uint8_t pwmDutyy);//setea el ciclo de trabajo de la senal pwm que controla la velocidad del motor 0 a 255
  turnOn(void);//enciende la bomba
  turnOff(void);//apaga la bomba
  bool isOn(void);// si devuelve true la bomba esta prendida caso contrario devuelve false
  private:

  uint8_t pin;
  uint8_t pwmDuty;
  bool pumpState;
 

};


#endif /*WATER_PUMP_H_*/
