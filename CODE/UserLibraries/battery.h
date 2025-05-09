#ifndef BATTERY_H
#define BATTERY_H

/*********************
 * battery.h
 * 
 * ����: COTOMO
 * ��Ŀ��ַ: https://github.com/Eclipse-01/SmartVehicleSchool
 * MIT��Դ֤��
 * JNU-FLY at Jiangnan University
 * 2024 November
 * 
 * ���ļ������˵�ص�ѹ���ͷ��������ƵĽӿڡ�
 */

#include "headfile.h"

extern uint16 battery_voltage; //��ص�ѹ
extern uint16 battery_percentage; //��ص����ٷֱ�

void battery_init(void);
void update_battery_status(void);
void Beep_init(void);
void Beep_set(uint8 status);
uint16 get_battery_voltage(void);
uint16 get_battery_percentage(void);
#endif // BATTERY_H
