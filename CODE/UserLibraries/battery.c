/*********************
 * battery.c
 * 
 * ����: COTOMO
 * ��Ŀ��ַ: https://github.com/Eclipse-01/SmartVehicleSchool
 * MIT��Դ֤��
 * JNU-FLY at Jiangnan University
 * 2024 November
 * 
 * ���ļ�ʵ���˵�ص�ѹ��⡢���������Ƽ�����״̬���µĹ��ܡ�
 ********************************************************************************************************************/

#include "headfile.h"

#define BAT_VOL_PIN ADC_P05
#define BEEP P32

uint16 ad_result = 0;           // ���ŵ�ѹ
uint32 temp;

uint16 battery_voltage = 0;     // ��ص�ѹ
uint16 battery_percentage = 0;  // ��ص����ٷֱ�

/**
 * @brief ��ʼ��������
 *
 * �ú����� P3.2 ��������Ϊ�������ģʽ�����ڿ��Ʒ�������
 */
void Beep_init(void){
    gpio_mode(P3_2,GPO_PP);	// ��P6.7����Ϊ�������
    P32 = 0;	// ��ʼ��Ϊ�͵�ƽ
}

/**
 * @brief ���÷�����״̬
 * 
 * �ú����������÷�����״̬��
 * 
 * @param status ������״̬��0Ϊ�رգ�1Ϊ��
 */
void Beep_set(uint8 status){
    P32 = status;
}

/**
 * @brief ��ʼ����ص�ѹ���ģ��
 * 
 * �ú�����ʼ�����ڵ�ص�ѹ����ADCģ�顣
 */
void battery_init(void){
    adc_init(BAT_VOL_PIN, ADC_SYSclk_DIV_32);
}

/**
 * @brief ��ȡ��ص�ѹ
 * 
 * �ú���ͨ��ADC��ȡ��ص�ѹ�������ݷ�ѹ�������ʵ�ʵ�ص�ѹ��
 * �����ص�ѹ����6.5V������ʾ��LOW BATTERY����ֹͣ�����������battery.h���޸�LOW_BATTERY_VOLTAGE��ֵΪ-1���ô˹���
 * 
 * @return uint16 ���ص�ص�ѹ����λΪ����
 */
uint16 get_battery_voltage(void){
    ad_result = adc_once(BAT_VOL_PIN, ADC_12BIT);
    temp = (((uint32)ad_result * 3300) / 4095);  //�������ǰadc���ŵĵ�ѹ ���㹫ʽΪ ad_result*VCC/ADC�ֱ���    VCC��λΪmv
    battery_voltage =  temp * 11; //�������ŵ�ѹ  �ͷ�ѹ�������ֵ�����ص�ѹ ���㹫˾Ϊ   ���ŵ�ѹ*(R2+R3)/R3   R3Ϊ�ӵض˵���
    while (battery_voltage < LOW_BATTERY_VOLTAGE){
        ips200_show_string(120,200,"LOW BATTERY");
        drv8701_stop(MOTOR_BOTH); //ֹͣ�����ֹ��ع���
        while(ENABLE_LOW_BATTERY_PROTECTION)
        delay_ms(10000); // ���������ֹ��������
    }
    
    return battery_voltage;
}

uint16 get_battery_percentage(void){
    int max_voltage = 4200 * BATTERY_GROUP_NUM;
    int min_voltage = LOW_BATTERY_VOLTAGE * BATTERY_GROUP_NUM;
    int current_voltage = get_battery_voltage();
    int percentage = (current_voltage - min_voltage) * 100 / (max_voltage - min_voltage);
    if (percentage > 100){
        percentage = 100;
    }
    if (percentage < 0){
        percentage = 0;
    }
    return percentage;
}

/**
 * @brief ���µ��״̬
 * 
 * �ú������ݵ�ǰ��ص�ѹ�͵����ٷֱȸ��µ��״̬�ͳ��״̬��
 */
void update_battery_status(void) {
    battery_percentage = get_battery_percentage();
}


