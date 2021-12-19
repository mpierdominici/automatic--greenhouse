#include "mnp_encoder.h"

mnp_encoder::mnp_encoder(uint8_t aPin, uint8_t bPin, uint8_t buttonPin) {
  this->pinA = aPin;
  this->pinB = bPin;
  this->pinButton = buttonPin;
 
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(pinButton, INPUT_PULLUP);
  edgeA.currentData=digitalRead(pinA);
  edgeButton.currentData=digitalRead(pinButton);
}



mnp_encoderEvent_t mnp_encoder::mnp_encoderRun(void) {

  mnp_encoderEvent_t data2return = MNP_ENCODER_NO_EVENT;
  edgeA.updateMme(digitalRead(pinA));
  edgeButton.updateMme(digitalRead(pinButton));

  if(edgeA.fallingEdge()){
    if(digitalRead(pinB)){
      data2return=MNP_ENCODER_TDER;
    }else{
      data2return=MNP_ENCODER_TIZQ;
    }
  }else if(edgeButton.fallingEdge()){
    data2return=MNP_ENCODER_TBUTTON;
  }
  return data2return;

}
