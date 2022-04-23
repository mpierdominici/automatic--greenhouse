//***********************TIMER**********************************

#include "myTimer.h"

myTimer::myTimer(unsigned int seconds)
{
  setNewTime(seconds);
}

//timeOver
//devuelve true si ya paso el tiempo seteado,
//caso contrario devuelve false
//
bool myTimer::timeOver(void)
{
  if ((millis()) > startTime)
  {
    resetTimer();
    return true;
  }
  else
  {
    return false;
  }
}

void myTimer::resetTimer(void)
{
  unsigned long temp = seconds * 100 + millis(); //centena de milisegundos

  startTime = temp;
  //Serial.print("se llamo a rest timer con: ");
  //Serial.println(startTime);
}

void  myTimer::setNewTime(unsigned long seconds_)
{
  unsigned long temp = seconds_;
  //Serial.println(temp);
  seconds = temp;

  //Serial.print("s seteo un timer cada: ");
  //Serial.print(seconds_);
  //Serial.print(" se registro un tirmpo de: ");
  //Serial.println(seconds/1000);
  resetTimer();

}

void myTimer::showInfo()
{
  //Serial.println(startTime);
  unsigned long dif = startTime - millis();
  //Serial.print("Remaining time (seconds):");
  //Serial.println(dif/1000);
  //Serial.println(startTime);
  //Serial.println(millis());
  //Serial.println(seconds/1000);
}

bool myTimer::timeOverSinReset(void)
{
  if ((millis()) > startTime)
  {
    
    return true;
  }
  else
  {
    return false;
  }
}