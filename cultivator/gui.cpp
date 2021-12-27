#include "gui.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "mnpRtc.h"

#define GUI_I2C_ADRESS 0x27
#define GUI_SCREEN_H 4
#define GUI_SCREEN_W 20
#define GUI_STRING_DEVICE_NAME  "      Growmatic"
#define GUI_STRING_CLEAN_LINE  "                    "
#define GUI_STRING_FRIMWARE_VERSION "Frimware version 1.0"
#define GUI_STRING_SERIAL_NUMBER "      CULT-100"

LiquidCrystal_I2C lcd(GUI_I2C_ADRESS, GUI_SCREEN_W, GUI_SCREEN_H); //creo la estructura de la pantalla
tmElements_t tempData;
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

  //LiquidCrystal_I2C lcd(GUI_I2C_ADRESS, GUI_SCREEN_W, GUI_SCREEN_H); //creo la estructura de la pantalla
  // Inicializar el LCD
  lcd.init();
  //mnpRtc_setDateTime(27,12,2021,19,20,10);
  //Encender la luz de fondo.
  lcd.backlight();
  
  lcd.createChar (7, hoja);
  
  //delay(100);
  // Escribimos el Mensaje en el LCD.
  //lcd.setCursor(0, 0);
  //lcd.print("Hola Mundo");
  //lcd.setCursor(0, 2);
  //lcd.print("Hola Mundo");
  // lcd.write (byte (7));

}

void GUI_presentacion(void) {
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
  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.write (byte (7));
  lcd.print("050% ON");
  
  lcd.setCursor(0, 3);
  lcd.write (byte (7));
  lcd.print("050% OFF");
  
  lcd.setCursor(10, 2);
  lcd.write (byte (7));
  lcd.print("050% ON");
  
  lcd.setCursor(10, 3);
  lcd.write (byte (7));
  lcd.print("050% OFF");

  
  while(1){
    
     tempData=mnpRtc_getDateTime();
     lcd.setCursor(0,0);
     lcd.print(GUI_STRING_CLEAN_LINE);
     lcd.setCursor(0,0);
     lcd.print(tempData.Hour);
     lcd.print(":");
     lcd.print(tempData.Minute);
     lcd.print(":");
     lcd.print(tempData.Second);
     delay(1100);
  }

}

 
