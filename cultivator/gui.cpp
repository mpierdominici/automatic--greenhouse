#include "gui.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>



#define GUI_I2C_ADRESS 0x27
#define GUI_SCREEN_H 4
#define GUI_SCREEN_W 20
#define GUI_STRING_DEVICE_NAME  "      Growmatic"
#define GUI_STRING_CLEAN_LINE  "                    "
#define GUI_STRING_FRIMWARE_VERSION "Frimware version 1.0"
#define GUI_STRING_SERIAL_NUMBER "      CULT-100"

LiquidCrystal_I2C lcd(GUI_I2C_ADRESS, GUI_SCREEN_W, GUI_SCREEN_H); //creo la estructura de la pantalla


byte hoja[8] =  {
  B00000,
  B00100,
  B00100,
  B01010,
  B10101,
  B10101,
  B01010,
  B00100
};

void GUI_init(void) {
  lcd.init();
  //mnpRtc_setDateTime(9,1,2022,12,03,10);
  lcd.backlight();
  lcd.createChar (7, hoja);
}

void GUI_presentacion(void) {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(GUI_STRING_DEVICE_NAME);
  delay(2000);
  lcd.setCursor(0,3);
  lcd.print(GUI_STRING_FRIMWARE_VERSION);
  delay(2000);
  lcd.setCursor(0,3);
  lcd.print(GUI_STRING_CLEAN_LINE);
  lcd.setCursor(0,3);
  lcd.print(GUI_STRING_SERIAL_NUMBER);
  delay(2000);    
}



void gui_principal_reDrawAll(void){
  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.write (byte (7));
  lcd.print("000% OFF");
  
  lcd.setCursor(0, 3);
  lcd.write (byte (7));
  lcd.print("000% OFF");
  
  lcd.setCursor(10, 2);
  lcd.write (byte (7));
  lcd.print("000% OFF");
  
  lcd.setCursor(10, 3);
  lcd.write (byte (7));
  lcd.print("000% OFF");
  
}

void gui_principal_printTime(tmElements_t dateTime){
  //tempData=mnpRtc_getDateTime();
     lcd.setCursor(0,0);
     lcd.print(GUI_STRING_CLEAN_LINE);
     lcd.setCursor(0,0);
     lcd.print(dateTime.Hour);
     lcd.print(":");
     lcd.print(dateTime.Minute);
     lcd.print(":");
     lcd.print(dateTime.Second);
}

void gui_principal_updateCh1(uint16_t medicion){
    lcd.setCursor(1, 2);
    lcd.print("   ");
    lcd.setCursor(1, 2);
    lcd.print(medicion);  
}

void gui_princial_updatePump1(bool state){
    lcd.setCursor(6, 2);
    lcd.print("   ");
    lcd.setCursor(6, 2);
    lcd.print(state?"ON":"OFF"); 
}



void gui_calibracion_soilHum_stage1(uint8_t ch){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibrando canal: ");
  lcd.print(ch);
  lcd.setCursor(0,2);
  lcd.print("Inserte suelo seco");
  lcd.setCursor(0,3);
  lcd.print("pres Enter Continuar");
}

void gui_calibracion_soilHum_stage2(void){
  lcd.clear();
  lcd.setCursor(0,2);
  lcd.print("Inserte suelo hum");
  lcd.setCursor(0,3);
  lcd.print("pres Enter Continuar");
  
}

void gui_calibracion_soilHum_stage3(void){

    lcd.clear();
  lcd.setCursor(0,2);
  lcd.print("cal Finalizada");

  
}
