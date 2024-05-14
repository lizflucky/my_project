#include "tracking.h"





uint16_t ServoPwm = 150;
uint8_t SensorState[4];
uint8_t Area,Last_Area;
void track(void)
{

	
	Car_Forward(100);
		
	
		SensorState[0]=Sensor0_Get_State();
		SensorState[1]=Sensor1_Get_State();
		SensorState[2]=Sensor2_Get_State();
		SensorState[3]=Sensor3_Get_State();
		
		if (SensorState[0] && !SensorState[1] && !SensorState[2] && SensorState[3])   //Area 3����ǰ����ֱ��
		{
			Area = 3;
			Arc_ServoPwm_Set(150);
		}
		else if (SensorState[0] && SensorState[1] && !SensorState[2] && SensorState[3]) //Area4, ��С���
		{
			Area = 4;
			Arc_ServoPwm_Set(160);
		}
		else if (SensorState[0] && SensorState[1] && SensorState[2] && !SensorState[3]) //Area6 ,�Ҵ���
		{
			Area = 6;
			Arc_ServoPwm_Set(180);
		}
		else if (SensorState[0] && !SensorState[1] && SensorState[2] && SensorState[3]) //Area2 ,��С��
		{
			Area = 2;
			Arc_ServoPwm_Set(140);
		}
		else if (!SensorState[0] && SensorState[1] && SensorState[2] && SensorState[3]) //Area0 ,�����
		{
			Area = 0;
			Arc_ServoPwm_Set(120);
		}				
		else if (SensorState[0] && SensorState[1] && SensorState[2] && SensorState[3])
		{
			if (Last_Area == 4)   //���е���
			{
				Area = 5;
				Arc_ServoPwm_Set(170);
			}
			else if (Last_Area == 2)  //���е���
			{
				Area = 1;
				Arc_ServoPwm_Set(130);
			}
		}

		Last_Area = Area;
		Delay_ms(30);
		
}


