#ifndef STRAIGHT_H
#define STRAIGHT_H

/*********************
 * straight.h
 * 
 * ����: COTOMO
 * ��Ŀ��ַ: https://github.com/Eclipse-01/SmartVehicleSchool
 * MIT��Դ֤��
 * JNU-FLY at Jiangnan University
 * 2024 November
 * 
 * ���ļ�������ֱ���㷨����ڡ�PID���Ƽ��쳣����ӿڡ�
 */

#include "headfile.h"

// ֱ����ں���
uint8 straight_entrance(void);

// PID���ƺ���
void PID_control_straint(void);
void PID_control_ring(void);

// �쳣������
void exception_handler(int exception_code);

// �����ߵ�λ��
int Line_calculate_position(void);
int Line_calculate_position_ring(void);

extern uint8 RING_FLAG; // �����ⲿ����

#endif // STRAIGHT_H
