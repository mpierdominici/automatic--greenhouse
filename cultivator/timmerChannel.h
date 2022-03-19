#ifndef MNP_TIMMERCHANNEL_H_
#define MNP_TIMMERCHANNEL_H_
#include<Arduino.h>
#include<stdint.h>
#include "mnpEeprom.h"


#define TIMMER_CHANNEL_MEMPOS_ON 0
#define TIMMER_CHANNEL_MEMPOS_OFF 1
#define TIMMER_CHANNEL_MEMPOS_AUTO 2

class timmerChannel{
  public:
  timmerChannel(uint8_t pin,uint8_t memAdress,bool inverted);
  setHoraActual(uint8_t hora){horaActual=hora;};//actualiza la hora actual
  setHoraOn(uint8_t hora){horaOn=hora;mnpEeprom_write8(horaOn,this->memAdress-TIMMER_CHANNEL_MEMPOS_ON);};//hora a partir de la cual se enciende el canal
  setHoraOff(uint8_t hora){horaOff=hora;mnpEeprom_write8(horaOff,this->memAdress-TIMMER_CHANNEL_MEMPOS_OFF);}; //hora a partir que se apaga el canal
  setMode(bool isAuto){this->isAuto=isAuto;mnpEeprom_write8(this->isAuto,this->memAdress-TIMMER_CHANNEL_MEMPOS_AUTO);};//cambio de modo manual a automatico 
  bool getMode(void){return isAuto;};
  uint8_t getHoraOn(void){return horaOn;};
  uint8_t getHoraOff(void){return horaOff;};
  updateChannel(uint8_t _horaActual);
  turnOn(void){state=true;digitalWrite(pin,!isInverted?state:!state);};
  turnOff(void){state=false;digitalWrite(pin,!isInverted?state:!state);};
  
  private:
  
  uint8_t horaActual;
  uint8_t horaOn;
  uint8_t horaOff;
  bool isAuto;//True para indicar que el sistema esta en automatico
  bool state;//Trues para indicar que los pines se encuentran en estado alto, false caso contrario
  bool isInverted; //true indica logica negada
  uint8_t pin; //pin que se esta controlando
  uint8_t memAdress;//posicion de memoria donde almaceno si esta en modo automatico o manual la bomba, tiene que dejar 3 bytes hacia atras
};



#endif
