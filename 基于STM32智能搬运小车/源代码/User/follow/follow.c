

#include "follow.h"

void follow_function(void)
{
		
	    int Distance_mm = Sonar();
		int Distance_cm = Distance_mm / 10;
		int Distance_cm_p = Distance_mm % 10;
		
					
		if (Distance_cm ==0 && Distance_cm_p==0)
		{
			Car_Stop();
		}
		else
		{
			if (Distance_cm > 20)
			{
				Car_Forward(200);
			}
			else if (Distance_cm < 10)
			{
				Car_Backward(200);
			}
			else
			{
				Car_Stop();
			}
		}
		Delay_ms(10);
		

}
