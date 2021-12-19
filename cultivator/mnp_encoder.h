
#ifndef MNP_ENCODER_H_
#define MNP_ENCODER_H_
#include<Arduino.h>
#include<stdint.h>
#include "memEdgeDetector.h"

typedef enum{
  MNP_ENCODER_NO_EVENT,//no se genero un evento de encoder
  MNP_ENCODER_TIZQ,//hubo un pulso hacia la izquierda
  MNP_ENCODER_TDER,// hubo un pulso hacia la derecha
  MNP_ENCODER_TBUTTON// se apreto el boton
}mnp_encoderEvent_t;

class mnp_encoder{
  public:
  mnp_encoder(uint8_t aPin,uint8_t bPin,uint8_t buttonPin); //constructor por defecto
  mnp_encoderEvent_t mnp_encoderRun(void);//funcion polling para chequiar los pines
  private:
  uint8_t pinA;
  uint8_t pinB;
  uint8_t pinButton;
  memEdgeDetector edgeA;
  memEdgeDetector edgeButton;
};


#endif /*MNP_ENCODER_H_*/
