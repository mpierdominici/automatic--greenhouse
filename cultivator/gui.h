#ifndef GUI_H_
#define GUI_H_

#include<Arduino.h>
#include<stdint.h>
#include "mnpRtc.h"

void GUI_init(void);

void GUI_presentacion(void);

void gui_principal_reDrawAll(void);
void gui_principal_printTime(tmElements_t dateTime);
void gui_principal_updateCh1(uint16_t medicion);

void gui_calibracion_soilHum_stage1(uint8_t ch);
void gui_calibracion_soilHum_stage2(void);
void gui_calibracion_soilHum_stage3(void);

void gui_princial_updatePump1(bool state);

#endif /*GUI*/
