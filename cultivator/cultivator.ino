#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "mnp_encoder.h"
#define ENCODER_PIN_A 7
#define ENCODER_PIN_B 6
#define ENCODER_PIN_BUTTON 5 

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27, 20, 4); //
mnp_encoder menuControl(ENCODER_PIN_A,ENCODER_PIN_B,ENCODER_PIN_BUTTON);

byte cuerpo[8] = {
  B01110,
  B01110,
  B00100,
  B11111,
  B00100,
  B01010,
  B10001,
  B00000,
};

void setup() {
  // Inicializar el LCD
  lcd.init();

  //Encender la luz de fondo.
  lcd.backlight();
  lcd.createChar (7, cuerpo);
  delay(100);
  // Escribimos el Mensaje en el LCD.
  lcd.setCursor(0, 0);
  lcd.print("Hola Mundo");
  lcd.setCursor(0, 2);
  lcd.print("Hola Mundo");
  lcd.write (byte (7));
}
uint8_t contadorIzq=0;
uint8_t contadorDer=0;
uint8_t contadorBut=0;
void loop() {
  // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  lcd.setCursor(0, 1);
  // Escribimos el número de segundos trascurridos
  lcd.print(contadorIzq);
  lcd.print(" ");
  lcd.print(contadorDer);
  lcd.print(" ");
  lcd.print(contadorBut);
 
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
