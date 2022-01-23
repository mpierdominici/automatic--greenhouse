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


#define ENCODER_PIN_A 12
#define ENCODER_PIN_B 11
#define ENCODER_PIN_BUTTON 10

#define CH1_ANALOG_PIN A0

#define CH1_BOMBA_PIN 7

#define TIME_OUT_RTC 20 //cada 2 segundo chequiar el rtc


mnp_encoder menuControl(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_PIN_BUTTON);//encoder
myTimer toRtc(TIME_OUT_RTC);//rtc

soilHumidity senCh1(CH1_ANALOG_PIN,CH1_G_H,CH1_O_H);
waterPump bombaCh1(CH1_BOMBA_PIN);
soilHumControl controlCh1(&senCh1,&bombaCh1,0);
tmElements_t te;

void setup() {

  GUI_init();
  //Serial.begin(9600);
//  mnpRtc_setDateTime(16,1,2022,16,10,10);
te=mnpRtc_getDateTime();
controlCh1.udateDate(makeTime(te));
controlCh1.setTimeOut(30);
}
uint8_t contadorIzq = 0;
uint8_t contadorDer = 0;
uint8_t contadorBut = 0;

void loop() {

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
  
  
  

  GUI_presentacion();
  gui_principal_reDrawAll();
  while (1) {

    if(toRtc.timeOver()){
      te=mnpRtc_getDateTime();
      gui_principal_printTime(te);
      controlCh1.udateDate(makeTime(te));
      controlCh1.run();
      gui_principal_updateCh1(senCh1.getHumidity());
      gui_princial_updatePump1(bombaCh1.isOn()); 
      //gui_principal_updateCh1(analogRead(A0));
      //Serial.println("Time out");
    }
    
    switch (menuControl.mnp_encoderRun()) {
      case MNP_ENCODER_TIZQ :
        contadorIzq++;
        break;
      case MNP_ENCODER_TDER:
        contadorDer++;
        break;
      case MNP_ENCODER_TBUTTON:
        contadorBut++;
        break;

    }
  }

}
