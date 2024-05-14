#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Usart.h"
#include "Wdg.h"
#include "HC_SR04.h"
#include "OLED.h"
#include "Motor.h"
#include "key.h"
#include "LED.h"
#include "obstacle_avoidance.h"
#include "remote_control.h"
#include "follow.h"
#include "tracking.h"
uint8_t KeyNum;
uint8_t PS2_Mode,Last_PS2_Mode;
uint8_t Arc_Pwm, Arc_LeftLimit,Arc_RightLimit;

void Sonar_Measure(void);

int main(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	HC_SR04Init();
	OLED_Init();
	Motor_Init();
	Key_Init();
	//LED_Init();
	PS2_Init();
	Servo_Init();
	
	OLED_ShowString(1,1,"Length:");
	OLED_ShowChar(1,11,'.');
	OLED_ShowString(1,14,"cm");	
	
	Arc_Pwm = Arc_Servo_Param_Get(SERVO_MIDDLE);
	Arc_LeftLimit = Arc_Servo_Param_Get(SERVO_LEFT_LIMIT);
	Arc_RightLimit = Arc_Servo_Param_Get(SERVO_RIGHT_LIMIT);
	
	
	
	while(1)
	{	
		Sonar_Measure();						//��๦��
		
		
		switch(Key_Num)
		{
			case 1:follow_function();			//���湦��
			break;
			case 2:Obstacle_Avoidance();		//���Ϲ���
			break;
			case 3:Remote_Control();			//ң�ع���
			break;
			case 4:track();						//ѭ������
			break;
			default : break;
				
		}

	}
	
}

// ���峬������ຯ��
void Sonar_Measure(void)
{
    // �ڴ˴�ʵ�ֳ���������߼�
		uint16_t Length_mm = Sonar();
		uint16_t Length_cm = Length_mm / 10;
		uint16_t length_cmf = Length_mm % 10;
		OLED_ShowNum(1,8,Length_cm,3);
		OLED_ShowNum(1,12,length_cmf,1);
		Delay_ms(100);
}


