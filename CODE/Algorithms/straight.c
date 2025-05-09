/*********************
 * straight.c
 * 
 * ����: COTOMO
 * ��Ŀ��ַ: https://github.com/Eclipse-01/SmartVehicleSchool
 * MIT��Դ֤��
 * JNU-FLY at Jiangnan University
 * 2024 November
 * 
 * ���ļ�ʵ����С��ֱ�е�PID�����㷨���쳣�����߼���
 ********************************************************************************************************************/

#include "headfile.h"
#include "straight.h"

#define XINGS 1 // ����ʮ�ֵ�����
#define VELOCITY 63
#define MAX_VELOCITY 59

#define USE_DYNAMIC_SPEED 

#ifdef USE_DYNAMIC_SPEED
#define MOTOR_SPEED dynamic_speed()
#else
#define MOTOR_SPEED VELOCITY
#endif // USE_DYNAMIC_SPEED
int recv = 32;
char str[64];
int dynamic_speed();
/*PID����������*/
float Kp = 0.615;
float Ki = 0.110;
float Kd = 3.1450;
float reactFactor = 0.6;

// ��ӻ��ֺ�΢�ֵ����ֵ����
float MAX_INTEGRAL = 100.0;    // ������Ҫ����
float MAX_DERIVATIVE = 100.0; // ������Ҫ����

/**
 * @brief ֱ����ں���
 * @return uint8 ����ֵ
 */
uint8 straight_entrance(void)
{
    ips200_clear();
    ips200_show_string(0, 0, "Straight mode");
    wireless_uart_send_buff("Straight mode\n", 13);
    while (1)
    {
        Remote_Stop();
        PID_control_straint();
        delay_ms(26);
    }
    return 0;
}

/**
 * @brief PID���ƺ���
 */

int position;
static int last_error = 0;
static int integral = 0;
int angle;

void PID_control_straint(void)
{

    position = Line_calculate_position();
    integral += position;
    angle = (position * Kp + integral * Ki + (position - last_error) * Kd) * reactFactor;
    last_error = position;
    servo_set_position(angle);
    drv8701_control(MOTOR_BOTH, MOTOR_SPEED);

    if (integral > MAX_INTEGRAL)
        integral = MAX_INTEGRAL;
    if (integral < -MAX_INTEGRAL)
        integral = -MAX_INTEGRAL;
    if (last_error > MAX_DERIVATIVE)
        last_error = MAX_DERIVATIVE;
    if (last_error < -MAX_DERIVATIVE)	
        last_error = -MAX_DERIVATIVE;
}

void PID_control_ring(void)
{

    position = Line_calculate_position_ring();
    integral += position;
    angle = (position * Kp + integral * Ki + (position - last_error) * Kd) * reactFactor;
    last_error = position;
    servo_set_position(angle);
    drv8701_control(MOTOR_BOTH,30);

    if (integral > MAX_INTEGRAL)
        integral = MAX_INTEGRAL;
    if (integral < -MAX_INTEGRAL)
        integral = -MAX_INTEGRAL;
    if (last_error > MAX_DERIVATIVE)
        last_error = MAX_DERIVATIVE;
    if (last_error < -MAX_DERIVATIVE)
        last_error = -MAX_DERIVATIVE;
}

static float last_speed = 0;
/**
 * @brief ��̬�ٶȿ��ƺ���
 * @return int �����ٶ�
 */
int dynamic_speed()
{
    float this_speed;
    this_speed = VELOCITY - abs(servo_position) * 0.3 - abs(position) * 0.3;
    if (this_speed - last_speed > 0.7)
        this_speed = last_speed + 0.7;
    if (this_speed < 34)
		//{while(this_speed < 35){this_speed+=0.5;delay_us(200);}}
        this_speed = 34;
    if (this_speed > MAX_VELOCITY)
		//{while(this_speed > MAX_VELOCITY){this_speed-=1;delay_us(200);}}
        this_speed = MAX_VELOCITY;
    last_speed = this_speed;
    return this_speed;
}

/**
 * @brief �쳣������
 * @param exception_code �쳣����
 */
void exception_handler(int exception_code)
{
    ips200_clear();
    ips200_show_string(0, 0, "Exception occured");
    switch (exception_code)
    {
    case 999:
        ips200_show_string(16, 0, "System error");
        break;
    case 998:
        ips200_show_string(16, 0, "Divide by zero");
        break;
    case 997:
        ips200_show_string(16, 0, "Line lost");
        break;
    case 996:
        ips200_show_string(16, 0, "Cross line");
        return;
    case 995:
        ips200_show_string(16, 0, "Left roundabout");
        break;
    case 994:
        ips200_show_string(16, 0, "Right roundabout");
        break;
    default:
        ips200_show_string(16, 0, "Unknown error");
    }
}

/**
 * @brief �����ߵ�λ��
 *
 * �����˲����ADCֵ�����ߵ�λ�ã����ܼ�⻷��
 *
 * @return int �ߵ�λ��
 * @warning С��ʹ��
 */

extern uint8 RING_FLAG = 0; // �����ⲿ����
int Line_calculate_position()
{
    float adc1, adc2, adc3, adc4, denominator, position;
    Line_read_raw();

    // ��һ��������Χ��0-100
    adc1 = ((float)adc_value.ADC1 * 100) / 4095;
    adc2 = ((float)adc_value.ADC2 * 100) / 4095;
    adc3 = ((float)adc_value.ADC3 * 100) / 4095;
    adc4 = ((float)adc_value.ADC4 * 100) / 4095;

    if ((adc1 + adc4 > 160)) // ��⵽����
    {
        Beep_set(1);
        last_speed = 30;
        ring_handler();
    }
    denominator = adc1 + adc4;
    if (denominator == 0)
        denominator = 1;
    position = (adc4 - adc1) / denominator * 100;
    sprintf(str, "Position: %f, adc1: %f, adc4: %f\n", position, adc1, adc4);
    wireless_uart_send_buff(str, strlen(str));
    return position;
}

/**
 * @brief �����ߵ�λ��
 *
 * �����˲����ADCֵ�����ߵ�λ�ã��ڻ�����ʹ��
 *
 * @return int �ߵ�λ��
 * @warning С��ʹ��
 */

extern uint8 RING_FLAG; // �����ⲿ����
int Line_calculate_position_ring()
{
    float adc1, adc2, adc3, adc4, denominator, position;
    Line_read_raw();

    // ��һ��������Χ��0-100
    adc1 = ((float)adc_value.ADC1 * 100) / 4095;
    adc2 = ((float)adc_value.ADC2 * 100) / 4095;
    adc3 = ((float)adc_value.ADC3 * 100) / 4095;
    adc4 = ((float)adc_value.ADC4 * 100) / 4095;
    denominator = adc1 + adc4;
    if (denominator == 0)
        denominator = 1;
    position = (adc4 - adc1) / denominator * 100;
    sprintf(str, "Position: %f\n, adc1: %f, adc4: %f\n", position, adc1, adc4);
    wireless_uart_send_buff(str, strlen(str));
    return position;
}

