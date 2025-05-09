/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2023-07-27

 ********************************************************************************************************************/
#include "headfile.h"

// �����ں�Ƶ�ʵ��趨�����Բ鿴board.h�ļ�
// ��board_init��,�Ѿ���P54��������Ϊ��λ
// �����Ҫʹ��P54����,������board.c�ļ��е�board_init()������ɾ��SET_P54_RESRT����


void main(void)
{
    clock_init(SYSTEM_CLOCK_52M); // ��ʼ��ϵͳƵ��,��ɾ���˾���롣
    board_init();                 // ��ʼ���Ĵ���,��ɾ���˾���롣

    // �˴���д�û����� ���������ʼ�������
    ips200_init_spi();
    servo_init();
    drv8701_init();
    encoder_init(); 
    Line_init();
    Beep_init();
    wireless_uart_init();
    // �˴���д�û����� ���������ʼ�������
    code_entrance();
}

void code_entrance()
{   
    char str[64];
    int recv = 32;

    straight_entrance(); //����С������Ҫ���룬ֱ����ʻ��Ҫ��С����ô�ܵĵ�����
    
    sprintf(str, "Vehicle init finished\n");
    wireless_uart_send_buff(str, strlen(str));
    while (1)
    {
        delay_ms(100);
        Get_ADC_Range();
    }
}


//�泶��������ֻҪ�����ģ�����UTF-8����GBK���������룬������ڿ����Ǹ�����
//���ӷ��ˣ���Ӣ���