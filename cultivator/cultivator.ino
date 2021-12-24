#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "mnp_encoder.h"
#include "gui.h"
#define ENCODER_PIN_A 7
#define ENCODER_PIN_B 6
#define ENCODER_PIN_BUTTON 5 


mnp_encoder menuControl(ENCODER_PIN_A,ENCODER_PIN_B,ENCODER_PIN_BUTTON);



void setup() {
  GUI_init();
 
}
uint8_t contadorIzq=0;
uint8_t contadorDer=0;
uint8_t contadorBut=0;
void loop() {

    GUI_presentacion(); 
  //delay(100);
  switch(menuControl.mnp_encoderRun()){
    case MNP_ENCODER_TIZQ :
    contadorIzq++;
    break;
    case MNP_ENCODER_TDER:
    contadorDer++;
    break;
    case MNP_ENCODER_TBUTTON:
    contadorBut++;
    break;
    
  };
  
}
