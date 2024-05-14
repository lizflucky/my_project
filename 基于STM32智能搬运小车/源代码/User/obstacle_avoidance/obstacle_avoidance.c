#include "obstacle_avoidance.h"


void Obstacle_Avoidance(void)
{
		int Distance_mm = Sonar();
		int Distance_cm = Distance_mm / 10;
		int Distance_cm_p = Distance_mm % 10;
	
	if(Distance_cm==0 && Distance_cm_p==0)
 {
		Car_Forward(200);
 }
	else
 {
	if(Distance_cm < 20)
	{
		Car_Backward(200);
		Car_TurnRight(300);
		Delay_ms(10);
	}
	else if(Distance_cm>20)
	{
		Car_Forward(200);
	}
	
 }

	Delay_ms(10);
	
//	if (Distance_cm ==0 && Distance_cm_p==0)
//		{
//			Car_Stop();
//		}
//		else
//		{
//			if (Distance_cm > 20)
//			{
//				Car_Forward(200);
//			}
//			else if (Distance_cm < 10)
//			{
//				Car_Backward(200);
//			}
//			else
//			{
//				Car_Stop();
//			}
//		}
	
	
	
}
