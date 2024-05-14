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
		
		if (SensorState[0] && !SensorState[1] && !SensorState[2] && SensorState[3])   //Area 3，当前处于直线
		{
			Area = 3;
			Arc_ServoPwm_Set(150);
		}
		else if (SensorState[0] && SensorState[1] && !SensorState[2] && SensorState[3]) //Area4, 右小弯道
		{
			Area = 4;
			Arc_ServoPwm_Set(160);
		}
		else if (SensorState[0] && SensorState[1] && SensorState[2] && !SensorState[3]) //Area6 ,右大弯
		{
			Area = 6;
			Arc_ServoPwm_Set(180);
		}
		else if (SensorState[0] && !SensorState[1] && SensorState[2] && SensorState[3]) //Area2 ,左小弯
		{
			Area = 2;
			Arc_ServoPwm_Set(140);
		}
		else if (!SensorState[0] && SensorState[1] && SensorState[2] && SensorState[3]) //Area0 ,左大弯
		{
			Area = 0;
			Arc_ServoPwm_Set(120);
		}				
		else if (SensorState[0] && SensorState[1] && SensorState[2] && SensorState[3])
		{
			if (Last_Area == 4)   //右中等弯
			{
				Area = 5;
				Arc_ServoPwm_Set(170);
			}
			else if (Last_Area == 2)  //左中等弯
			{
				Area = 1;
				Arc_ServoPwm_Set(130);
			}
		}

		Last_Area = Area;
		Delay_ms(30);
		
}


