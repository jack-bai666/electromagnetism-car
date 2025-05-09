/*********************
 * servo.h
 * 
 * ����: COTOMO
 * ��Ŀ��ַ: https://github.com/Eclipse-01/SmartVehicleSchool
 * MIT��Դ֤��
 * JNU-FLY at Jiangnan University
 * 2024 November
 * 
 * ���ļ������˶���ĳ�ʼ����λ�����ù��ܡ�
 *
 * ���Э����� LICENSE �ļ���
 */
#ifndef SERVO_H
#define SERVO_H

#include "zf_pwm.h"

extern int8 servo_position; // ���λ��

void servo_init(void);
void servo_set_position(int16 position);

#endif // SERVO_H
