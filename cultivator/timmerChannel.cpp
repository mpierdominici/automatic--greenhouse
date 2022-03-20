#include "timmerChannel.h"


/*
class timmerChannel{
  public:
  timmerChannel(uint8_t pin,uint8_t memAdress);
  setHoraActual(uint8_t hora){horaActual=hora;};//actualiza la hora actual
  setHoraOn(uint8_t hora){horaOn=hora;mnpEeprom_write8(horaOn,this->memAdress-TIMMER_CHANNEL_MEMPOS_ON);};//hora a partir de la cual se enciende el canal
  setHoraOff(uint8_t hora){horaOff=hora;mnpEeprom_write8(horaOff,this->memAdress-TIMMER_CHANNEL_MEMPOS_OFF);}; //hora a partir que se apaga el canal
  setMode(bool isAuto){this->isAuto=isAuto;mnpEeprom_write8(this->isAuto,this->memAdress-TIMMER_CHANNEL_MEMPOS_AUTO);};//cambio de modo manual a automatico 
  bool getMode(void){return isAuto;);
  uint8_t getHoraOn(void){return horaOn;};
  uint8_t getHoraOff(void){return horaOff;};
  updateChannel(uint8_t _horaActual);
  turnOn(void){state=true;digitalWrite(pin,state);};
  turnOff(void){state=false;digitalWrite(pin,state);};
  
  private:
  
  uint8_t horaActual;
  uint8_t horaOn;
  uint8_t horaOff;
  bool isAuto;//True para indicar que el sistema esta en automatico
  bool state;//Trues para indicar que los pines se encuentran en estado alto, false caso contrario
  uint8_t pin; //pin que se esta controlando
  uint8_t memAdress;//posicion de memoria donde almaceno si esta en modo automatico o manual la bomba, tiene que dejar 3 bytes hacia atras
}

*/

timmerChannel::timmerChannel(uint8_t pin,uint8_t memAdress,bool inverted) {
  this->pin = pin;
  this->memAdress = memAdress;
  state=false;
  isInverted=inverted;
  pinMode(this->pin, OUTPUT); //configuro pin como salida
  turnOff();
  horaActual=0; 
  horaOn = mnpEeprom_read8(this->memAdress - TIMMER_CHANNEL_MEMPOS_ON);
  horaOff = mnpEeprom_read8(this->memAdress - TIMMER_CHANNEL_MEMPOS_OFF);
  isAuto = mnpEeprom_read8(this->memAdress - TIMMER_CHANNEL_MEMPOS_AUTO);
  
}

timmerChannel::updateChannel(uint8_t _horaActual){
  horaActual=_horaActual;
  if(isAuto){
    if(horaActual>=horaOn && horaActual<=horaOff){
      turnOn();
    }else{
      turnOff();
    }
  }
}
