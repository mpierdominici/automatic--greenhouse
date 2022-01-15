#ifndef MY_TIMER_H_
#define MY_TIMER_H_
#include<Arduino.h>
#include<stdint.h>


class myTimer
{
  public:
    myTimer(unsigned int seconds = 0); // lo canvie a ms
    bool timeOver(void);
    void setNewTime(unsigned long seconds_);
    void showInfo();

    unsigned long seconds;
    unsigned long startTime;
    void resetTimer(void);

};

#endif//MNP_ENCODER_H_
