#include"soilHumControl.h"
#include"mnpEeprom.h"


soilHumControl::soilHumControl(soilHumidity * sensHum, waterPump * pump, uint8_t mem2save) {
  humGraund = sensHum;
  water = pump;
  memPosHigh=mem2save;
  //humMax = 80;
  //humMin = 20;
  humActual = 0;
  mode = MANUAL;
  //timeInterval = 0;
  actualDate = 0;
  nextEvent = 0;
  waterTankIsEmpty=false;

  timeInterval=mnpEeprom_read32(mem2save-SOILHUMCONTROL_DELTA_T_POS);
  humMax=mnpEeprom_read8(mem2save-SOILHUMCONTROL_HUM_MAX_POS);
  humMin=mnpEeprom_read8(mem2save-SOILHUMCONTROL_HUM_MIN_POS);

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
  }else{//reinicio flags
    startCount=false;
    regando=false;   
  }
  if(waterTankIsEmpty && regando){
    water->turnOff();    
  }else if((waterTankIsEmpty==false)&& regando && (water->isOn() ==false)){
    water->turnOn();
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
