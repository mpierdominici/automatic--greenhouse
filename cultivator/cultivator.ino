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

#define LUZ_PIN 47 //49
#define VENTILACION_PIN 49  //47
#define RECIRCULADO_PIN 41  //45

#define CH1_ANALOG_PIN A0
#define CH1_BOMBA_PIN 7

#define CH2_ANALOG_PIN A1
#define CH2_BOMBA_PIN 6

#define CH3_ANALOG_PIN A2
#define CH3_BOMBA_PIN 5

#define CH4_ANALOG_PIN A3
#define CH4_BOMBA_PIN 4

#define WATER_LEVEL_PIN 13

#define LIGHT_TEMPERATURE_SENSOR_PIN A4 //pin analogico 4
/*FIN Listado de pines*/

#define TIME_OUT_RTC 20 //cada 2 segundo chequiar el rtc
#define TIME_OUT_LUZ_PANTALLA 600 //un minuto sin actividad apaga la pantalla


#define LM35_TEMPRATURE_2_SAMPLES 2.048 // 1 grado celcius son 2.048 muestras
#define MAX_LIGHT_TEMPERATURE 70 //temperatura a la que se apaga el sistema por proteccion contra sobre temperatura
#define MAX_LIGHT_TEMPERATURE_SAMPLES (MAX_LIGHT_TEMPERATURE*LM35_TEMPRATURE_2_SAMPLES)

/*****************Prototipo de funciones en main ***********************/
void checkWaterLevel(void);
void updateAlllPerifericals(void);
void calibracionSensoresHumedad(void);
void checkLigthTemperature(void);
/*****************FIN Prototipo de funciones en main ***********************/
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
myTimer timeOutScreen(TIME_OUT_LUZ_PANTALLA);//si estoy en time out apago la pantalla

/*****************Canales de riego*************************/
soilHumidity senCh1(CH1_ANALOG_PIN, CH1_G_H, CH1_O_H);
waterPump bombaCh1(CH1_BOMBA_PIN);
soilHumControl controlCh1(&senCh1, &bombaCh1, CH1_C_H);

soilHumidity senCh2(CH2_ANALOG_PIN, CH2_G_H, CH2_O_H);
waterPump bombaCh2(CH2_BOMBA_PIN);
soilHumControl controlCh2(&senCh2, &bombaCh2, CH2_C_H);

soilHumidity senCh3(CH3_ANALOG_PIN, CH3_G_H, CH3_O_H);
waterPump bombaCh3(CH3_BOMBA_PIN);
soilHumControl controlCh3(&senCh3, &bombaCh3, CH3_C_H);

soilHumidity senCh4(CH4_ANALOG_PIN, CH4_G_H, CH4_O_H);
waterPump bombaCh4(CH4_BOMBA_PIN);
soilHumControl controlCh4(&senCh4, &bombaCh4, CH4_C_H);

/*****************Fin canales de riego********************/


timmerChannel luzControl(LUZ_PIN,LUZ_MEM,true);
timmerChannel ventilacionControl(VENTILACION_PIN,VENTILAZION_MEM,true);
timmerChannel recirculadoControl(RECIRCULADO_PIN,RECIRCULADO_MEM,true);

tmElements_t te;

/***************Variables maquina de estados************************/
uint8_t dispositivoSeleccionado=CANALES_RIEGO;//variable que indica que periferico esta seleccionado
bool isSelectectingDevice=false; //si esta en true altero la fuente de eventos al precionar el encoder
uint8_t canalSeleccionado = 0;// variable que almacena que canal de riego esta seleccionado
soilHumControl * controladores[4] = {&controlCh1, &controlCh2, &controlCh3, &controlCh4};
uint8_t selectedHmin = 0;//variable donde almaceno la humeadad minima
uint8_t selectedHmax = 0;//variable donde almaceno la humedad maxima
uint32_t selectedDeltat = 0;//variable donde almaceno el tiempo que dejo seca la tierra

uint8_t horaMenu=0;//variable que se utiliza para almacenar temporalmente la hora;

bool slectedPerIsAuto=false;// true si el periferico seleccionado esta en modo automatico

bool refreshIdleScreen=false;//true refresco la informacion de la pantalla principal

/***************FIN Variables maquina de estados************************/

/*************Funciones de estado y transiciones******************/
void enterIdle(void) {
  gui_principal_reDrawAll();
  }
void duringIdle(void) {
  if(refreshIdleScreen){
    refreshIdleScreen=false;
    gui_principal_printTime(te);
    gui_principal_updateCh1(senCh1.getHumidity());
    gui_princial_updatePump1(bombaCh1.isOn());

    gui_principal_updateCh2(senCh2.getHumidity());
    gui_princial_updatePump2(bombaCh2.isOn());

    gui_principal_updateCh3(senCh3.getHumidity());
    gui_princial_updatePump3(bombaCh3.isOn());

    gui_principal_updateCh4(senCh4.getHumidity());
    gui_princial_updatePump4(bombaCh4.isOn());

    gui_luz_state(luzControl.getMode(), luzControl.getOutputSatatus());
    gui_vent_state(ventilacionControl.getMode(), ventilacionControl.getOutputSatatus());
    gui_res_state(recirculadoControl.getMode(), recirculadoControl.getOutputSatatus());
  }


}
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
  controladores[canalSeleccionado - 1]->setMode(slectedPerIsAuto?TH:MANUAL);//alamceno el resultado del paso anterior
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
  gui_menu_luz_horaOn(horaMenu);
}

void enterLuzHoraOff(void){
  gui_menu_luz_horaOff(horaMenu);
}

void enterRecirculadoHoraOn(void){
  gui_menu_recirculado_horaOn(horaMenu);
}

void enterRecirculadoHoraOff(void){
  gui_menu_recirculado_horaOff(horaMenu);
}

void enterVentilacionHoraOn(void){
  gui_menu_ventilacion_horaOn(horaMenu);
}

void enterVentilacionHoraOff(void){
  gui_menu_ventilacion_horaOff(horaMenu);
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

void t_selPeriferico2luzHoraOn(void){
  horaMenu=luzControl.getHoraOn();
  slectedPerIsAuto=luzControl.getMode();
}

void t_luzHoraOn2horaOff(void){
  luzControl.setHoraOn(horaMenu);//guardo la hora en la eeprom
  horaMenu=luzControl.getHoraOff();
  
}

void t_luzHoraOnInc(void){
  horaMenu++;
  horaMenu=horaMenu>23?23:horaMenu;
}

void t_luzHoraOnDec(void){
   horaMenu--;
  horaMenu=horaMenu>23?0:horaMenu;//overflow uint8
}

void t_luzHoraOffInc(void){
  horaMenu++;
  horaMenu=horaMenu>23?23:horaMenu;
}

void t_luzHoraOffDec(void){
  horaMenu--;
  horaMenu=horaMenu>23?0:horaMenu;//overflow uint8
}

void t_luzHoraOff2idle(void){
    luzControl.setHoraOff(horaMenu);//guardo la hora en la eeprom
}


void t_selPeriferico2ventilacionHoraOn(void){
  horaMenu=ventilacionControl.getHoraOn();
  slectedPerIsAuto=ventilacionControl.getMode();
}
void t_ventilacionHoraOnInc(void){
  horaMenu++;
  horaMenu=horaMenu>23?23:horaMenu;
}
void t_ventilacionHoraOnDec(void){
  horaMenu--;
  horaMenu=horaMenu>23?0:horaMenu;//overflow uint8
}
void t_ventilacionHoraOn2ventilacionHoraOff(void){
  ventilacionControl.setHoraOn(horaMenu);
  horaMenu=ventilacionControl.getHoraOff();
}
void t_ventilacionHoraOffInc(void){
  horaMenu++;
  horaMenu=horaMenu>23?23:horaMenu;
}
void t_ventilacionHoraOffDec(void){
  horaMenu--;
  horaMenu=horaMenu>23?0:horaMenu;//overflow uint8
}
void t_ventilacionHoraOff2idle(void){
  ventilacionControl.setHoraOff(horaMenu);
}

    void t_selPeriferico2recirculadoHoraOn(void){
        horaMenu=recirculadoControl.getHoraOn();
        slectedPerIsAuto=recirculadoControl.getMode();
    }
    void t_recirculadoHoraOnInc(void){
      horaMenu++;
      horaMenu=horaMenu>23?23:horaMenu;
    }
    void t_recirculadoHoraOnDec(void){
      horaMenu--;
      horaMenu=horaMenu>23?0:horaMenu;//overflow uint8
    }
    void t_recirculadoHoraOn2recirculadoHoraOff(void){
      recirculadoControl.setHoraOn(horaMenu);
      horaMenu=recirculadoControl.getHoraOff();

    }
    void t_recirculadoHoraOffInc(void){
      horaMenu++;
      horaMenu=horaMenu>23?23:horaMenu;
    }
    void t_recirculadoHoraOffDec(void){
      horaMenu--;
      horaMenu=horaMenu>23?0:horaMenu;//overflow uint8
    }
    void t_recirculadoHoraOff2idle(void){
      recirculadoControl.setHoraOff(horaMenu);

    }



  void t_hmax2hmin(void){
    controladores[canalSeleccionado - 1]->setMaxHum(selectedHmax);
  }
  void t_hmin2deltat(void){
    controladores[canalSeleccionado - 1]->setMinHum(selectedHmin);
  }
  void t_deltat2idle(void){
    controladores[canalSeleccionado - 1]->setTimeOut(selectedDeltat);
  }

  void enter_riego_enable(void){
    gui_predenrApagarPeriferico(slectedPerIsAuto);
  }

  void s_selec2riegoEnable(void){
    slectedPerIsAuto=controladores[canalSeleccionado - 1]->getMode()?false:true;
  }

  void t_riegoEnableDecc(void){
    slectedPerIsAuto=!slectedPerIsAuto;
  }

  void enter_luz_enable(void){
    gui_predenrApagarPeriferico(slectedPerIsAuto);
  }

  void t_luzEnable2luzHoraOn(void){
    luzControl.setMode(slectedPerIsAuto);
  }

  void t_luzEnableChangeState(void){
    slectedPerIsAuto=!slectedPerIsAuto;
  }

  void enter_recirculado_enable(void){
    gui_predenrApagarPeriferico(slectedPerIsAuto);
  }

  void enter_ventilacion_enable(void){
      gui_predenrApagarPeriferico(slectedPerIsAuto);
  }

  void t_ventilacionEnable2ventilacionHoraOn(void){
    ventilacionControl.setMode(slectedPerIsAuto);
  }

  void t_ventilacionEnableCahngeState(void){
    slectedPerIsAuto=!slectedPerIsAuto;
  }

  void t_recirculadoEnable2recirculadoHoraOn(void){
    recirculadoControl.setMode(slectedPerIsAuto);
  }

  void t_recricualdoEnableChangeState(void){
    slectedPerIsAuto=!slectedPerIsAuto;
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

  State s_riego_enable(&enter_riego_enable,NULL,NULL);
  State s_luz_enable(&enter_luz_enable,NULL,NULL);
  State s_recirculado_enable(&enter_recirculado_enable,NULL,NULL);
  State s_ventilacion_enable(&enter_ventilacion_enable,NULL,NULL);


  Fsm fsm(&s_idle);


  /*********FIN Ceacion de la maquina de estado y sus estados*********/

  /*********Asignacion de transiciones a eventos*********************/
  void assignTransitionFsm(void) {
    //fsm.add_transition(&s_idle, &s_selec, ENTER, &t_idle2selec);

    fsm.add_transition(&s_idle, &s_selectPeriferico, ENTER, &t_idle2selecPeriferico);
    fsm.add_transition(&s_selectPeriferico, &s_selectPeriferico, INC, &t_selecPerifericoInc);
    fsm.add_transition(&s_selectPeriferico, &s_selectPeriferico, DECC, &t_selecPerifericoDec);
    fsm.add_transition(&s_selectPeriferico, &s_idle, EV_SALIR,NULL);



    //fsm.add_transition(&s_selectPeriferico,&s_luzHoraOn,IR_LUZ,&t_selPeriferico2luzHoraOn);
    fsm.add_transition(&s_selectPeriferico,&s_luz_enable,IR_LUZ,&t_selPeriferico2luzHoraOn);
    fsm.add_transition(&s_luz_enable,&s_luzHoraOn,ENTER,&t_luzEnable2luzHoraOn);
    fsm.add_transition(&s_luz_enable,&s_luz_enable,INC,&t_luzEnableChangeState);
    fsm.add_transition(&s_luz_enable,&s_luz_enable,DECC,&t_luzEnableChangeState);

    fsm.add_transition(&s_luzHoraOn,&s_luzHoraOn,INC,&t_luzHoraOnInc);
    fsm.add_transition(&s_luzHoraOn,&s_luzHoraOn,DECC,&t_luzHoraOnDec);
    fsm.add_transition(&s_luzHoraOn,&s_luzHoraOff,ENTER,&t_luzHoraOn2horaOff);
    fsm.add_transition(&s_luzHoraOff,&s_luzHoraOff,INC,&t_luzHoraOffInc);
    fsm.add_transition(&s_luzHoraOff,&s_luzHoraOff,DECC,&t_luzHoraOffDec);
    fsm.add_transition(&s_luzHoraOff,&s_idle,ENTER,&t_luzHoraOff2idle);


    //fsm.add_transition(&s_selectPeriferico,&s_ventilacionHoraOn,IR_VENTILACION,&t_selPeriferico2ventilacionHoraOn);
    fsm.add_transition(&s_selectPeriferico,&s_ventilacion_enable,IR_VENTILACION,&t_selPeriferico2ventilacionHoraOn);
    fsm.add_transition(&s_ventilacion_enable,&s_ventilacionHoraOn,ENTER,&t_ventilacionEnable2ventilacionHoraOn);
    fsm.add_transition(&s_ventilacion_enable,&s_ventilacion_enable,INC,&t_ventilacionEnableCahngeState);
    fsm.add_transition(&s_ventilacion_enable,&s_ventilacion_enable,DECC,&t_ventilacionEnableCahngeState);


    fsm.add_transition(&s_ventilacionHoraOn,&s_ventilacionHoraOn,INC,&t_ventilacionHoraOnInc);
    fsm.add_transition(&s_ventilacionHoraOn,&s_ventilacionHoraOn,DECC,&t_ventilacionHoraOnDec);
    fsm.add_transition(&s_ventilacionHoraOn,&s_ventilacionHoraOff,ENTER,&t_ventilacionHoraOn2ventilacionHoraOff);
    fsm.add_transition(&s_ventilacionHoraOff,&s_ventilacionHoraOff,INC,&t_ventilacionHoraOffInc);
    fsm.add_transition(&s_ventilacionHoraOff,&s_ventilacionHoraOff,DECC,&t_ventilacionHoraOffDec);
    fsm.add_transition(&s_ventilacionHoraOff,&s_idle,ENTER,&t_ventilacionHoraOff2idle);



    //fsm.add_transition(&s_selectPeriferico,&s_recirculadoHoraOn,IR_RECIRCULADO,&t_selPeriferico2recirculadoHoraOn);
    fsm.add_transition(&s_selectPeriferico,&s_recirculado_enable,IR_RECIRCULADO,&t_selPeriferico2recirculadoHoraOn);
    fsm.add_transition(&s_recirculado_enable,&s_recirculadoHoraOn,ENTER,&t_recirculadoEnable2recirculadoHoraOn);
    fsm.add_transition(&s_recirculado_enable,&s_recirculado_enable,INC,&t_recricualdoEnableChangeState);
    fsm.add_transition(&s_recirculado_enable,&s_recirculado_enable,DECC,&t_recricualdoEnableChangeState);



    fsm.add_transition(&s_recirculadoHoraOn,&s_recirculadoHoraOn,INC,&t_recirculadoHoraOnInc);
    fsm.add_transition(&s_recirculadoHoraOn,&s_recirculadoHoraOn,DECC,&t_recirculadoHoraOnDec);
    fsm.add_transition(&s_recirculadoHoraOn,&s_recirculadoHoraOff,ENTER,&t_recirculadoHoraOn2recirculadoHoraOff);
    fsm.add_transition(&s_recirculadoHoraOff,&s_recirculadoHoraOff,INC,&t_recirculadoHoraOffInc);
    fsm.add_transition(&s_recirculadoHoraOff,&s_recirculadoHoraOff,DECC,&t_recirculadoHoraOffDec);
    fsm.add_transition(&s_recirculadoHoraOff,&s_idle,ENTER,&t_recirculadoHoraOff2idle);


    fsm.add_transition(&s_selectPeriferico, &s_selec, IR_RIEGO, &t_idle2selec);
    //fsm.add_transition(&s_selec, &s_hmax, ENTER, t_selec2hmax);
    fsm.add_transition(&s_selec, &s_selec, INC, &t_selecInc);
    fsm.add_transition(&s_selec, &s_selec, DECC, &t_selecDec);
    fsm.add_transition(&s_selec, &s_idle, ESC, NULL);

    fsm.add_transition(&s_selec,&s_riego_enable,ENTER,&s_selec2riegoEnable);
    fsm.add_transition(&s_riego_enable, &s_hmax, ENTER, &t_selec2hmax);
    fsm.add_transition(&s_riego_enable, &s_riego_enable, DECC, &t_riegoEnableDecc);
    fsm.add_transition(&s_riego_enable, &s_riego_enable, INC, &t_riegoEnableDecc);

    fsm.add_transition(&s_hmax, &s_hmin, ENTER, &t_hmax2hmin);
    fsm.add_transition(&s_hmax, &s_hmax, INC, &t_hmaxInc);
    fsm.add_transition(&s_hmax, &s_hmax, DECC, &t_hmaxDec);
    fsm.add_transition(&s_hmax, &s_idle, ESC, NULL);

    fsm.add_transition(&s_hmin, &s_deltat, ENTER, &t_hmin2deltat);
    fsm.add_transition(&s_hmin, &s_hmin, INC,& t_hminInc);
    fsm.add_transition(&s_hmin, &s_hmin, DECC,& t_hminDec);
    fsm.add_transition(&s_hmin, &s_idle, ESC, NULL);

    fsm.add_transition(&s_deltat, &s_idle, ENTER, &t_deltat2idle);
    fsm.add_transition(&s_deltat, &s_deltat, INC, &t_delatInc);
    fsm.add_transition(&s_deltat, &s_deltat, DECC, &t_delatDec);
    fsm.add_transition(&s_deltat, &s_idle, ESC, NULL);
  
 
  
  }
  /*********FIN Asignacion de transiciones a eventos*********************/


  void setup() {
    assignTransitionFsm();
    GUI_init();
    Serial.begin(9600);
    pinMode(WATER_LEVEL_PIN,INPUT);
    te = mnpRtc_getDateTime();
    //
    //
    
    controlCh1.udateDate(makeTime(te));
    //
    //controlCh1.setMode(TH);
  }
  uint8_t contadorIzq = 0;
  uint8_t contadorDer = 0;
  uint8_t contadorBut = 0;

  void loop() {


    GUI_backlight(true);
    delay(500);
    if(menuControl.mnp_encoderRun()==MNP_ENCODER_TBUTTON){
      GUI_backlight(false);
      delay(350);
      GUI_backlight(true);
      delay(350);
      calibracionSensoresHumedad();
    }
    
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

            controlCh2.setMaxHum(81);
            controlCh2.setMinHum(5);
            controlCh2.setTimeOut(25);

            controlCh3.setMaxHum(81);
            controlCh3.setMinHum(5);  
            controlCh3.setTimeOut(25);

            controlCh4.setMaxHum(81);
            controlCh4.setMinHum(5);
            controlCh4.setTimeOut(25);


            luzControl.setHoraOn(10);
            luzControl.setHoraOff(20);
            luzControl.setMode(true);

            ventilacionControl.setHoraOn(20);
            ventilacionControl.setHoraOff(30);
            ventilacionControl.setMode(true);

            recirculadoControl.setHoraOn(30);
            recirculadoControl.setHoraOff(40);
            recirculadoControl.setMode(true);
              
            mnpRtc_setDateTime(16, 1, 2022, 16, 10, 10);
            Serial.println("Configuracion Finalizada");
            Serial.print("Demora de la configuracion: ");
            Serial.print(millis()-deltaTT);
            Serial.println(" mS");
            Serial.println(luzControl.getMode());
        }
      }


      updateAlllPerifericals();
      checkWaterLevel();
      

      switch (menuControl.mnp_encoderRun()) {
        case MNP_ENCODER_TIZQ :
          timeOutScreen.resetTimer();
          fsm.trigger(DECC);
          fsm.run_machine();

          break;
        case MNP_ENCODER_TDER:
          timeOutScreen.resetTimer();
          fsm.trigger(INC);
          fsm.run_machine();

          break;
        case MNP_ENCODER_TBUTTON:
          timeOutScreen.resetTimer();
          //fsm.trigger(ENTER);
          //fsm.run_machine();
          delay(250);
          if(isSelectectingDevice){//si estoy seleccionando dispositivo altero la fuente de eventos enter para bifurcar la fsm
            fsm.trigger(IR_RIEGO+dispositivoSeleccionado);//ver sipositivos t y eventots t
            fsm.run_machine();
          }else{
            fsm.trigger(ENTER);
            fsm.run_machine();
          }
          break;

      }
      fsm.run_machine();
      checkLigthTemperature();
    }

  }








  //Serial.println(controlCh1.humMax);
  //Serial.println(controlCh1.humMin);
  //Serial.println(controlCh1.timeInterval);


void checkWaterLevel(void){//miro si me quedo sin agua bloqueo el codigo y espero a que llenen de agua y confirmen para continuar
  if(!digitalRead(WATER_LEVEL_PIN)){//si detecto que el nivel del tanque es bajo ingreso a la rutina de proteccion de bombas
  //informo en pantalla
  gui_emergencia_flataAgua1();
  //desactivar bombas
  for(uint8_t i=0;i<4;i++){
    controladores[i]->updateWaterTankState(true);
  }
  while(!digitalRead(WATER_LEVEL_PIN)){delay(100);updateAlllPerifericals();}//hasta que no llene el tanque bloque el sistema

  gui_emergencia_flataAgua2();

  while(!(menuControl.mnp_encoderRun()==MNP_ENCODER_TBUTTON)){delay(100);updateAlllPerifericals();}
  //reactivoRiego
  for(uint8_t i=0;i<4;i++){
    controladores[i]->updateWaterTankState(false);
  }
  gui_principal_reDrawAll();

}
}


void updateAlllPerifericals(void){
  if (toRtc.timeOver()) {
        refreshIdleScreen=true;//indico que la funcion durin de idle rediuje la pantalla principal
        te = mnpRtc_getDateTime();
        //gui_principal_printTime(te);
        controlCh1.udateDate(makeTime(te));
        controlCh1.run();
        controlCh2.udateDate(makeTime(te));
        controlCh2.run();
        controlCh3.udateDate(makeTime(te));
        controlCh3.run();
        controlCh4.udateDate(makeTime(te));
        controlCh4.run();

        luzControl.updateChannel(te.Hour);
        ventilacionControl.updateChannel(te.Hour);
        recirculadoControl.updateChannel(te.Hour);
       
        //gui_principal_updateCh1(analogRead(A0));
       // Serial.println(te.Second);
       if(timeOutScreen.timeOverSinReset()){
         GUI_backlight(false);
       }else{
         GUI_backlight(true);
       }
      }
}

bool doubleCheckLigthTemperature(void){
  uint32_t accumulator=0;
  uint8_t i=0;
  for(i=0;i<100;i++){
    accumulator+=analogRead(LIGHT_TEMPERATURE_SENSOR_PIN);
    delay(5);
  }

  accumulator=accumulator/i;
  return (accumulator>=MAX_LIGHT_TEMPERATURE_SAMPLES);

}

void checkLigthTemperature(void){//chequeo que la temperatura de las luces sea inferior 
  if((analogRead(LIGHT_TEMPERATURE_SENSOR_PIN)>=MAX_LIGHT_TEMPERATURE_SAMPLES)&&doubleCheckLigthTemperature()){
    //inidicar en pantalla que se esta en una situacion de emergencia
    gui_sobretemperaturaLuz();
    //apagar todo los perifericos
      for(uint8_t i=0;i<4;i++){
        controladores[i]->setMode(MANUAL);
      }
      recirculadoControl.setMode(false);
      ventilacionControl.setMode(false);
      luzControl.setMode(false);

    // bloquear el equipo hasta que se reinicie
     while(1){
        GUI_backlight(false);
        delay(300);
        GUI_backlight(true);
        delay(300);
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


  void calibracionSensoresHumedad(void){

    gui_calibracion_soilHum_stage1(1);
    while(menuControl.mnp_encoderRun()!=MNP_ENCODER_TBUTTON);
    senCh1.calibrateSetLowMesurement(2);
    gui_calibracion_soilHum_stage2();
    delay(250);
    while(menuControl.mnp_encoderRun()!=MNP_ENCODER_TBUTTON);
    senCh1.calibrateSetHighMesurement(98);
    gui_calibracion_soilHum_stage3();
    delay(250);
    while(menuControl.mnp_encoderRun()!=MNP_ENCODER_TBUTTON);
    senCh1.calibrate();



    gui_calibracion_soilHum_stage1(2);
    while(menuControl.mnp_encoderRun()!=MNP_ENCODER_TBUTTON);
    senCh2.calibrateSetLowMesurement(2);
    gui_calibracion_soilHum_stage2();
    delay(250);
    while(menuControl.mnp_encoderRun()!=MNP_ENCODER_TBUTTON);
    senCh2.calibrateSetHighMesurement(98);
    gui_calibracion_soilHum_stage3();
    delay(250);
    while(menuControl.mnp_encoderRun()!=MNP_ENCODER_TBUTTON);
    senCh2.calibrate();


    gui_calibracion_soilHum_stage1(3);
    while(menuControl.mnp_encoderRun()!=MNP_ENCODER_TBUTTON);
    senCh3.calibrateSetLowMesurement(2);
    gui_calibracion_soilHum_stage2();
    delay(250);
    while(menuControl.mnp_encoderRun()!=MNP_ENCODER_TBUTTON);
    senCh3.calibrateSetHighMesurement(98);
    gui_calibracion_soilHum_stage3();
    delay(250);
    while(menuControl.mnp_encoderRun()!=MNP_ENCODER_TBUTTON);
    senCh3.calibrate();


    gui_calibracion_soilHum_stage1(4);
    while(menuControl.mnp_encoderRun()!=MNP_ENCODER_TBUTTON);
    senCh4.calibrateSetLowMesurement(2);
    gui_calibracion_soilHum_stage2();
    delay(250);
    while(menuControl.mnp_encoderRun()!=MNP_ENCODER_TBUTTON);
    senCh4.calibrateSetHighMesurement(98);
    gui_calibracion_soilHum_stage3();
    delay(250);
    while(menuControl.mnp_encoderRun()!=MNP_ENCODER_TBUTTON);
    senCh4.calibrate();

  }