#include <Wire.h>

#include <TimeLib.h>
#include <DS1307RTC.h>
#include <LiquidCrystal_I2C.h>
#include "mnpEeprom.h"
#include "mnp_encoder.h"
#include "gui.h"
#include "waterPump.h"
#include "soilHumidity.h"
#include "eepromMemMap.h"
#include "soilHumControl.h"
#include "myTimer.h"
#include "Fsm.h"
#include "timmerChannel.h"

#define AMAUNT_CHANNEL 4

/*Listado de pines*/

#define ENCODER_PIN_A 12
#define ENCODER_PIN_B 11
#define ENCODER_PIN_BUTTON 10

#define LUZ_PIN 49

#define CH1_ANALOG_PIN A0

#define CH1_BOMBA_PIN 7

/*FIN Listado de pines*/

#define TIME_OUT_RTC 20 //cada 2 segundo chequiar el rtc




typedef enum {
  IDLE,
  SLEC,
  HMAX,
  HMIN,
  DELTA
} estados_t;

typedef enum {
  ENTER,
  ESC,
  INC,
  DECC,
  IR_RIEGO,
  IR_LUZ,
  IR_VENTILACION,
  IR_RECIRCULADO,
  EV_SALIR
} eventos_t;

typedef enum{
  CANALES_RIEGO,
  ILUMINCACION,
  VENTILACION,
  RECIRCULADO,
  SALIR,
  N_DSIPOSITIVOS
}dispositivos_t;



mnp_encoder menuControl(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_PIN_BUTTON);//encoder
myTimer toRtc(TIME_OUT_RTC);//rtc


soilHumidity senCh1(CH1_ANALOG_PIN, CH1_G_H, CH1_O_H);
waterPump bombaCh1(CH1_BOMBA_PIN);
soilHumControl controlCh1(&senCh1, &bombaCh1, CH1_C_H);

timmerChannel luzControl(LUZ_PIN,LUZ_MEM,true);

tmElements_t te;

/***************Variables maquina de estados************************/
uint8_t dispositivoSeleccionado=CANALES_RIEGO;
bool isSelectectingDevice=false; //si esta en true altero la fuente de eventos al precionar el encoder
uint8_t canalSeleccionado = 0;
soilHumControl * controladores[4] = {&controlCh1, &controlCh1, &controlCh1, &controlCh1};
uint8_t selectedHmin = 0;
uint8_t selectedHmax = 0;
uint32_t selectedDeltat = 0;
/***************FIN Variables maquina de estados************************/

/*************Funciones de estado y transiciones******************/
void enterIdle(void) {
  gui_principal_reDrawAll();
  }
void duringIdle(void) {}
void exitIdle(void) {}

void enterSelec(void) {
  gui_menu_slec(canalSeleccionado);
}

void t_idle2selec(void) {
  canalSeleccionado = 1;
}

void t_selecInc(void) {
  canalSeleccionado++;
  if (canalSeleccionado > AMAUNT_CHANNEL) {
    canalSeleccionado = AMAUNT_CHANNEL;
  }
}
void t_selecDec(void) {
  canalSeleccionado--;
  if (canalSeleccionado < 1) {
    canalSeleccionado = 1;
  }
}

void duringSelec(void) {}
void exitSelec(void) {}

void enterHmax(void) {
  gui_menu_hmax(selectedHmax);
}
void duringHmax(void) {}
void exitHmax(void) {}

void enterHmin(void) {
  gui_menu_hmin(selectedHmin);
}
void duringHmin(void) {}
void exitHmin(void) {}

void enterDeltat(void) {
   gui_menu_deltat(selectedDeltat);
   
  }
void duringDeltat(void) {}
void exitDeltat(void) {}



void t_selec2hmax(void) {
  selectedHmin = controladores[canalSeleccionado - 1]->getHmin();
  selectedHmax = controladores[canalSeleccionado - 1]->getHmax();
  selectedDeltat = controladores[canalSeleccionado - 1]->getDeltat();
}

void t_hmaxInc(void) {
  selectedHmax++;
  if (selectedHmax > 100) {
    selectedHmax = 100;
  }
}

void t_hmaxDec(void) {
  selectedHmax--;
  if (selectedHmax > 100) { //overflow de la variable, salta a 255
    selectedHmax = 0;
  }
}


void t_hminInc(void) {
  selectedHmin++;
  if (selectedHmin > selectedHmax) {
    selectedHmin = selectedHmax;
  }
}
  void t_hminDec(void) {
    selectedHmin--;
    if (selectedHmin > selectedHmax) { //overflow de la variable, salta a 255
      selectedHmin = 0;
    }

  }

void t_delatInc(void){
  selectedDeltat++;
}

void t_delatDec(void){
  selectedDeltat--;
}

void enterSelectPeriferico(void){
  isSelectectingDevice=true;
  gui_menu_selecPeriferico(dispositivoSeleccionado);
}

void exitSelectPeriferico(void){
  isSelectectingDevice=false;
}

void enterLuzHoraOn(void){

}

void enterLuzHoraOff(void){

}

void enterRecirculadoHoraOn(void){

}

void enterRecirculadoHoraOff(void){

}

void enterVentilacionHoraOn(void){

}

void enterVentilacionHoraOff(void){


}

void t_idle2selecPeriferico(void){
  dispositivoSeleccionado=CANALES_RIEGO;//reinicio el contador
}

void t_selecPerifericoInc(void){
  dispositivoSeleccionado++;
  dispositivoSeleccionado=(dispositivoSeleccionado>=N_DSIPOSITIVOS)?N_DSIPOSITIVOS-1 : dispositivoSeleccionado;
  gui_menu_selecPeriferico(dispositivoSeleccionado);
}

void t_selecPerifericoDec(void){
  dispositivoSeleccionado--;
  dispositivoSeleccionado=dispositivoSeleccionado>N_DSIPOSITIVOS?0:dispositivoSeleccionado;
  gui_menu_selecPeriferico(dispositivoSeleccionado);
}


  /*************FIN Funciones de estado y transiciones******************/

  /*********Ceacion de la maquina de estado y sus estados*********/
  State s_idle(&enterIdle, &duringIdle, &exitIdle);
  State s_selec(&enterSelec, &duringSelec, &exitSelec);
  State s_hmax(&enterHmax, &duringHmax, &exitHmax);
  State s_hmin(&enterHmin, &duringHmin, &exitHmin);
  State s_deltat(&enterDeltat, &duringDeltat, &exitDeltat);
  

  State s_selectPeriferico(&enterSelectPeriferico,NULL,&exitSelectPeriferico);
  
  State s_luzHoraOn(&enterLuzHoraOn,NULL,NULL);
  State s_luzHoraOff(&enterLuzHoraOff,NULL,NULL);
  
  State s_recirculadoHoraOn(&enterRecirculadoHoraOn,NULL,NULL);
  State s_recirculadoHoraOff(&enterRecirculadoHoraOff,NULL,NULL);

  State s_ventilacionHoraOn(&enterVentilacionHoraOn,NULL,NULL);
  State s_ventilacionHoraOff(&enterVentilacionHoraOff,NULL,NULL);

  Fsm fsm(&s_idle);


  /*********FIN Ceacion de la maquina de estado y sus estados*********/

  /*********Asignacion de transiciones a eventos*********************/
  void assignTransitionFsm(void) {
    //fsm.add_transition(&s_idle, &s_selec, ENTER, &t_idle2selec);

    fsm.add_transition(&s_idle, &s_selectPeriferico, ENTER, &t_idle2selecPeriferico);
    fsm.add_transition(&s_selectPeriferico, &s_selectPeriferico, INC, &t_selecPerifericoInc);
    fsm.add_transition(&s_selectPeriferico, &s_selectPeriferico, DECC, &t_selecPerifericoDec);
    fsm.add_transition(&s_selectPeriferico, &s_idle, EV_SALIR,NULL);

    fsm.add_transition(&s_selectPeriferico, &s_selec, IR_RIEGO, &t_idle2selec);
    fsm.add_transition(&s_selec, &s_hmax, ENTER, t_selec2hmax);
    fsm.add_transition(&s_selec, &s_selec, INC, t_selecInc);
    fsm.add_transition(&s_selec, &s_selec, DECC, t_selecDec);
    fsm.add_transition(&s_selec, &s_idle, ESC, NULL);

    fsm.add_transition(&s_hmax, &s_hmin, ENTER, NULL);
    fsm.add_transition(&s_hmax, &s_hmax, INC, t_hmaxInc);
    fsm.add_transition(&s_hmax, &s_hmax, DECC, t_hmaxDec);
    fsm.add_transition(&s_hmax, &s_idle, ESC, NULL);

    fsm.add_transition(&s_hmin, &s_deltat, ENTER, NULL);
    fsm.add_transition(&s_hmin, &s_hmin, INC, t_hminInc);
    fsm.add_transition(&s_hmin, &s_hmin, DECC, t_hminDec);
    fsm.add_transition(&s_hmin, &s_idle, ESC, NULL);

    fsm.add_transition(&s_deltat, &s_idle, ENTER, NULL);
    fsm.add_transition(&s_deltat, &s_deltat, INC, t_delatInc);
    fsm.add_transition(&s_deltat, &s_deltat, DECC, t_delatDec);
    fsm.add_transition(&s_deltat, &s_idle, ESC, NULL);
  }
  /*********FIN Asignacion de transiciones a eventos*********************/


  void setup() {
    assignTransitionFsm();
    GUI_init();
    Serial.begin(9600);
    
    te = mnpRtc_getDateTime();
    //
    //
    
    controlCh1.udateDate(makeTime(te));
    //
    controlCh1.setMode(TH);
  }
  uint8_t contadorIzq = 0;
  uint8_t contadorDer = 0;
  uint8_t contadorBut = 0;

  void loop() {



    GUI_presentacion();
    gui_principal_reDrawAll();
    while (1) {

      if(Serial.available()){
        if(Serial.read()=='s'){//si me llego una S por terminal cargo los parametros a eeprom
            uint32_t deltaTT=millis();
            Serial.println("Iniciando configuracion de EEPROM");
            controlCh1.setMaxHum(81);
            controlCh1.setMinHum(5);
            controlCh1.setTimeOut(25);
            luzControl.setHoraOn(10);
            luzControl.setHoraOff(30);
            luzControl.setMode(true);

            mnpRtc_setDateTime(16, 1, 2022, 16, 10, 10);
            Serial.println("Configuracion Finalizada");
            Serial.print("Demora de la configuracion: ");
            Serial.print(millis()-deltaTT);
            Serial.println(" mS");
            Serial.println(luzControl.getMode());
        }
      }


      if (toRtc.timeOver()) {
        te = mnpRtc_getDateTime();
        //gui_principal_printTime(te);
        controlCh1.udateDate(makeTime(te));
        controlCh1.run();
        luzControl.updateChannel(te.Second);
        //gui_principal_updateCh1(senCh1.getHumidity());
        //gui_princial_updatePump1(bombaCh1.isOn());
        //gui_principal_updateCh1(analogRead(A0));
       // Serial.println(te.Second);
      }

      switch (menuControl.mnp_encoderRun()) {
        case MNP_ENCODER_TIZQ :
          fsm.trigger(DECC);
          fsm.run_machine();

          break;
        case MNP_ENCODER_TDER:
          fsm.trigger(INC);
          fsm.run_machine();

          break;
        case MNP_ENCODER_TBUTTON:
          //fsm.trigger(ENTER);
          //fsm.run_machine();
          if(isSelectectingDevice){//si estoy seleccionando dispositivo altero la fuente de eventos enter para bifurcar la fsm
            fsm.trigger(IR_RIEGO+dispositivoSeleccionado);//ver sipositivos t y eventots t
            fsm.run_machine();
          }else{
            fsm.trigger(ENTER);
            fsm.run_machine();
          }
          break;

      }
    }

  }






  //  gui_calibracion_soilHum_stage1(1);
  //  while(menuControl.mnp_encoderRun()!=MNP_ENCODER_TBUTTON);
  //  senCh1.calibrateSetLowMesurement(2);
  //  gui_calibracion_soilHum_stage2();
  //  delay(250);
  //  while(menuControl.mnp_encoderRun()!=MNP_ENCODER_TBUTTON);
  //  senCh1.calibrateSetHighMesurement(98);
  //  gui_calibracion_soilHum_stage3();
  //  delay(250);
  //  while(menuControl.mnp_encoderRun()!=MNP_ENCODER_TBUTTON);
  //  senCh1.calibrate();

  //Serial.println(controlCh1.humMax);
  //Serial.println(controlCh1.humMin);
  //Serial.println(controlCh1.timeInterval);
