#include <Wire.h>

#include <TimeLib.h>
#include <DS1307RTC.h>
#include <LiquidCrystal_I2C.h>
#include "mnpEeprom.h"
#include "mnp_encoder.h"
#include "gui.h"
#include "waterPump.h"
#define ENCODER_PIN_A 7
#define ENCODER_PIN_B 6
#define ENCODER_PIN_BUTTON 5



mnp_encoder menuControl(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_PIN_BUTTON);



void setup() {

  GUI_init();
  //Serial.begin(9600);

}
uint8_t contadorIzq = 0;
uint8_t contadorDer = 0;
uint8_t contadorBut = 0;
void loop() {
  // mnpEeprom_write16(10025, 4);
  //Serial.println(mnpEeprom_read16(4));
  //mnpEeprom_writeFloat(10.25,8);
  //Serial.println(mnpEeprom_readFloat(8));
  //Serial.println(1.3);
    GUI_presentacion();
    //delay(100);
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

    };

  }
