#include "waterPump.h"


waterPump::waterPump(uint8_t waterPumpPin){
  pin=waterPumpPin;
  pinMode(pin,OUTPUT);
  analogWrite(pin,0);
  pwmDuty=125;
  pumpState=false;
  
}

waterPump::writeSpeed(uint8_t pwmDutyy){
  pwmDuty=pwmDutyy;
}

waterPump::turnOn(void){
  pumpState=true;
  analogWrite(pin,pwmDuty);
}

waterPump::turnOff(void){
  pumpState=false;
  analogWrite(pin,0);
}

bool waterPump::isOn(void){
  return pumpState;
}
