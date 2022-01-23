#include"soilHumControl.h"

//typedef enum{
//  TH;//control tiempo humedad
//  MANUAL;//manual, detien el funcionamiento de la bomba
//
//}soilHumControl_types_t;
//
//
//class soilHumControl{
//  public:
//  soilHumControl(soilHumidity * sensHum,waterPump * pump,uint8_t mem2save);
//  void udateDate(tmElements_t currentTime);
//  soilHumControl_types_t getMode(void){return mode;};
//  void setMode(soilHumControl_types_t nMode);
//  void setTimeOut(uint32_t timeBetewenLowHum);
//  void setMaxHum(uint8_t hum);//0-100%
//  void setMinHum(uint8_t hum);//0-100%
//  void updateWaterTankState(bool empty){waterTankIsEmpty=empty;};
//  uint8_t getHumActual(void){return humActual};
//  void run(void); //actualizo estado de bomba, lectura de sesnores
//  private:
//  bool waterTankIsEmpty;//estado del tanque, tru si esta bacio
//  soilHumidity * humGraund;//lectura de humedad
//  waterPump * water;//control de la bomba
//  uint32_t actualDate;//date actual
//  uint32_t nextEvent;//momento en que deve iniciarse el riego
//  uint32_t timeInterval;//intervalo a esperar luego de que se seque el suelo
//  soilHumControl_types_t mode;//modo acual del control
//  uint8_t humMax;//nivel maximo de humedad
//  uint8_t humMin;//nivel minimo de humedad
//  uint8_t humActual;//ultima medicion de humedad
//
//
//};

soilHumControl::soilHumControl(soilHumidity * sensHum, waterPump * pump, uint8_t mem2save) {
  humGraund = sensHum;
  water = pump;
  humMax = 80;
  humMin = 20;
  humActual = 0;
  mode = MANUAL;
  timeInterval = 0;
  actualDate = 0;
  nextEvent = 0;
}

void soilHumControl::run(void) {
  static bool startCount = false; //true para indiciar que esta en la etapa de espera
  static bool regando = false; //true para indicar que me encuentro en la etapa de rigeo
  if (mode==TH) {
    humActual = humGraund->getHumidity();
    if ((humActual < humMin) && (startCount == false)) { //chequeo que este en hum minima
      startCount = true;
      nextEvent = timeInterval + actualDate;
    }

    if (startCount) { //si estoy en la etapa de espera, chequeo el timer
      if ((nextEvent <= actualDate) && (regando == false)) {
        regando = true;
        water->writeSpeed(255);
        water->turnOn();
      }
      if (regando) {
        if (humActual >= humMax) {
          water->turnOff();
          regando = false;
          startCount = false;
        }
      }
    }
  }else{
    startCount=false;
    regando=false;   
  }
}

void soilHumControl::setMode(soilHumControl_types_t nMode){
  if(mode!=nMode){
    if (mode==TH){
      water->turnOff();
    }
    mode=nMode;
  }
  
}
