#ifndef MNP_RTC_H_
#define MNP_RTC_H_
#include<Arduino.h>
#include<stdint.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

void mnpRtc_init(void);
bool mnpRtc_setDateTime(uint8_t mDay,uint8_t mMonth,uint16_t mYear,uint8_t mHour,uint8_t mMinute,uint8_t mSecond);
tmElements_t mnpRtc_getDateTime(void);
bool mnpRtc_icOk(void);

#endif /*MNP_RTC_H_*/
