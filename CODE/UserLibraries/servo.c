/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣��???�ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32F12K
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"
#include "servo.h"

/*
 * �����ں�Ƶ�ʵ��趨�����Բ鿴board.h�ļ�
 * ��board_init��,�Ѿ���P54��������Ϊ��λ
 * �����Ҫʹ��P54����,������board.c�ļ��е�board_init()������ɾ��SET_P54_RESRT����
 */

int8 servo_position = 0;  // ���λ��

// ����Kp��Kd����
float servo_Kp = 1.0f;
float servo_Kd = 0.1f;

// ����ǰһ�ε����
static int16 previous_error = 0;

/**
 *@brief  �����ʼ��
 *@note   Ҫ���ڳ�ʼ����λ�ã������޸�Center��ֵ
 */
void servo_init(void)
{
    uint32 duty = Servo_Center; // ��ʼ��Ϊ�м�λ�ã��궨����ռ�ձ�
    pwm_init(PWMB_CH3_P33, 50, duty);            // PWMƵ����50Hz����20ms��
}

/**
 * @brief ��������õ�ָ���Ƕȡ�
 *
 * �˺�������һ���Զ�Ϊ��λ�ĽǶȣ����������������Ӧ��λ�á�
 * �Ƕ�Ӧ�ڶ��֧�ֵ���Ч��Χ�ڣ���MaxAngle�ĺ궨�塣
 *
 * @param angle Ҫ���õĶ���Ƕȣ���ռ�ձ�Ϊ��λ��
 */
void servo_set_position(int16 angle)
{
    uint32 pulseWide = Servo_Center;
    if (angle > Servo_MaxAngle)
    {
        angle = Servo_MaxAngle;
    }
    if (angle < -Servo_MaxAngle)
    {
        angle = -Servo_MaxAngle;
    }
    pulseWide = (Servo_Center + angle);
    pwm_duty(PWMB_CH3_P33, pulseWide);
    servo_position = angle;
}



