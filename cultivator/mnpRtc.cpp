#include "mnpRtc.h"
#define MNP_RTC_DIFF_YEAR 1970
tmElements_t temp;
void mnpRtc_init(void){
  
}
bool mnpRtc_setDateTime(uint8_t mDay,uint8_t mMonth,uint16_t mYear,uint8_t mHour,uint8_t mMinute,uint8_t mSecond){
  
  temp.Hour=mHour;
  temp.Minute=mMinute;
  temp.Second=mSecond;
  temp.Day=mDay;
  temp.Month=mMonth;
  temp.Year=mYear-MNP_RTC_DIFF_YEAR;
  return RTC.write(temp);
}
tmElements_t mnpRtc_getDateTime(void){
  RTC.read(temp);
  return temp;
}

bool mnpRtc_icOk(void){
  return RTC.chipPresent();
}


 //Serial.print("Ok, Time = ");
   // Serial.print(tm.Hour);
    //Serial.write(':');
    //Serial.print(tm.Minute);
    //Serial.write(':');
    //Serial.print(tm.Second);
    //Serial.print(", Date (D/M/Y) = ");
    //Serial.print(tm.Day);
    //Serial.write('/');
    //Serial.print(tm.Month);
    //Serial.write('/');
    //Serial.print(tmYearToCalendar(tm.Year));
    //Serial.println();
