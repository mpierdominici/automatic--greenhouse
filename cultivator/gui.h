#ifndef GUI_H_
#define GUI_H_

#include<Arduino.h>
#include<stdint.h>
#include "mnpRtc.h"

void GUI_init(void);

void GUI_presentacion(void);

void gui_principal_reDrawAll(void);
void gui_principal_printTime(tmElements_t dateTime);


void gui_calibracion_soilHum_stage1(uint8_t ch);
void gui_calibracion_soilHum_stage2(void);
void gui_calibracion_soilHum_stage3(void);

void gui_princial_updatePump1(bool state);
void gui_principal_updateCh1(uint16_t medicion);

void gui_princial_updatePump2(bool state);
void gui_principal_updateCh2(uint16_t medicion);

void gui_princial_updatePump3(bool state);
void gui_principal_updateCh3(uint16_t medicion);

void gui_princial_updatePump4(bool state);
void gui_principal_updateCh4(uint16_t medicion);

void gui_menu_slec(uint8_t canal);
void gui_menu_hmax(uint8_t humedad);
void gui_menu_hmin(uint8_t humedad);
void gui_menu_deltat(uint32_t tiempo);


void gui_menu_selecPeriferico(uint8_t per);
void gui_menu_luz_horaOn(uint8_t hora);
void gui_menu_luz_horaOff(uint8_t hora);

void gui_menu_ventilacion_horaOn(uint8_t hora);
void gui_menu_ventilacion_horaOff(uint8_t hora);
void gui_menu_recirculado_horaOff(uint8_t hora);
void gui_menu_recirculado_horaOn(uint8_t hora);

void GUI_backlight(bool on);

void gui_emergencia_flataAgua1(void);
void gui_emergencia_flataAgua2(void);

void gui_predenrApagarPeriferico(bool estado);

void gui_sobretemperaturaLuz(void);

#endif /*GUI*/
