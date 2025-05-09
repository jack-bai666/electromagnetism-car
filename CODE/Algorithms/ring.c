/*********************
 * ring.c
 * 
 * ����: COTOMO
 * ��Ŀ��ַ: https://github.com/Eclipse-01/SmartVehicleSchool
 * MIT��Դ֤��
 * JNU-FLY at Jiangnan University
 * 2024 November
 * 
 * ���ļ�ʵ���˳���������˳������Ĵ����߼��������������Ͷ���Ƕȿ��ơ�
 ********************************************************************************************************************/

#include "headfile.h"
#include "ring.h"

#define DISTANCE 4500 // �����뻷�źŵ��뻷�ľ���
#define RETURN_TRACK 13000 // �뻷��ָ�ѭ��ǰ�ľ���
#define SERVO_ENTER_RING_ANGLE -31 // ������

static uint8 RING_FLAG = 0; // �����ⲿ����

/**
 * @brief �������ĺ���
 *
 * �ú������ڴ��������뻷�����߼��������������Ͷ���Ƕ����á�
 */
void ring_handler(void){
    uint32 distance = 0;
    uint32 normal_distance = 0; // ��¼�뻷ǰ�ľ���
    uint32 real_distance = 0; // ��¼ʵ�ʾ���

    /*
    * ΪʲôҪ������distance�أ���Ϊ��ʱ���ڵ���clear_encoder_distance()����ʱ��
    * ���ֵ����һ���ᱻ�嵽0��������Ҫһ��normal_distance����¼�뻷ǰ�Ķ���������real_distance
    * ���ǵ������������߼����ʽ�ĸ��Ӷ�
    */

    char str[64];
    clear_encoder_distance();
    normal_distance = get_encoder_distance();
    sprintf(str, "Normal distance: %d\n", normal_distance);
    wireless_uart_send_buff(str, strlen(str));

    while ( real_distance < DISTANCE ) // �ж��Ƿ񵽴��뻷�㣬���ѭ�������˶����ǵ�ʱ��
    {
        sprintf(str, "Distance: %d\n", distance - normal_distance);
        wireless_uart_send_buff(str, strlen(str));
        distance = get_encoder_distance();
        real_distance = distance - normal_distance;
        PID_control_ring();
        delay_ms(10);
    }
    RING_FLAG = RING_FLAG ^ 1; // ������־λȡ��
    if (RING_FLAG == 0)
    {
        Beep_set(0);
        return;
    }
    wireless_uart_send_buff("We have reached the ring point, setting servo angle\n", 52);

    servo_set_position(SERVO_ENTER_RING_ANGLE); // ������
    while ( distance - normal_distance < RETURN_TRACK ) // �ж��Ƿ񵽴��뻷��ָ�ѭ��ǰ�ľ��룬�����ƶ����ǵ�ʱ��
    {
        distance = get_encoder_distance();
        real_distance = distance - normal_distance;
        delay_ms(10);
    }
		Beep_set(0);
    wireless_uart_send_buff("Ring enterance operation finished\n", 36);
    // �˳��������ƣ��ָ�ѭ��
}
