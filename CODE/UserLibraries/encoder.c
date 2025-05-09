/*********************
 * encoder.c
 * 
 * ����: COTOMO
 * ��Ŀ��ַ: https://github.com/Eclipse-01/SmartVehicleSchool
 * MIT��Դ֤��
 * JNU-FLY at Jiangnan University
 * 2024 November
 * 
 * ���ļ�ʵ���˱������ĳ�ʼ������ȡ��������㹦�ܡ�
 *
 * ���Э����� LICENSE �ļ���
 ********************************************************************************************************************/

#include "headfile.h"

static uint32 TravelDistanceL = 0, TravelDistanceR = 0, TravelDistance = 0;

/**
 * @brief ��ʼ��������
 * 
 * ��ʼ��������1�ͱ�����2�ļ������źͷ�������
 */
void encoder_init(void)
{
    // ��ʼ��������1������CTIM0_P34��LSB1������������P35��DIR1��
    ctimer_count_init(CTIM0_P34);
    // ��ʼ��������2������CTIM3_P04��LSB2������������P53��DIR2��
    ctimer_count_init(CTIM3_P04);
}

/**
 * @brief ��ȡ������1��ֵ
 * 
 * �������������ʱ����ݷ������Ŷ�ȡ������1�ļ���ֵ
 * 
 * @return int16 ������1�ļ���ֵ
 */
int16 encoder_read1(void)
{
    int16 dat;
    if(P35 == 1) // DIR1 ע����ڰ�װԭ�����������������Ҳ�����������෴
    {
        dat = ctimer_count_read(CTIM0_P34);
    }
    else
    {
        dat = -ctimer_count_read(CTIM0_P34);
    }
    TravelDistanceL += dat;
    ctimer_count_clean(CTIM0_P34);
    delay_ms(encoder_time);
    if(P35 == 1) // DIR1 ע����ڰ�װԭ�����������������Ҳ�����������෴
    {
        dat = ctimer_count_read(CTIM0_P34);
    }
    else
    {
        dat = -ctimer_count_read(CTIM0_P34);
    }
    TravelDistanceL += dat;
    return dat;
}

/**
 * @brief ��ȡ������2��ֵ
 * 
 * �������������ʱ����ݷ������Ŷ�ȡ������2�ļ���ֵ
 * 
 * @return int16 ������2�ļ���ֵ
 */
int16 encoder_read2(void)
{
    int16 dat;
    if(P53 == 0) // DIR2
    {
        dat = ctimer_count_read(CTIM3_P04);
    }
    else
    {
        dat = -ctimer_count_read(CTIM3_P04);
    }
    TravelDistanceR += dat;
    ctimer_count_clean(CTIM3_P04);
    delay_ms(encoder_time);
    if(P53 == 0) // DIR2
    {
        dat = ctimer_count_read(CTIM3_P04);
    }
    else
    {
        dat = -ctimer_count_read(CTIM3_P04);
    }
    TravelDistanceR += dat;
    return dat;
}

/**
 * @brief ��ȡƽ����ʻ����
 * 
 * ���������Ҳ��������ƽ����ʻ����
 * 
 * @return int16 ƽ����ʻ����
 */
int16 get_encoder_distance(void)
{
    encoder_read1();
    encoder_read2(); // �������ұ���������ʻ����
    TravelDistance = (TravelDistanceL + TravelDistanceR) / 2;
    return TravelDistance;
}

/**
 * @brief �����������ʻ����
 * 
 * ����ࡢ�Ҳ������ʻ��������
 */
void clear_encoder_distance(void)
{
    TravelDistanceL = 0;
    TravelDistanceR = 0;
    TravelDistance = 0;
}