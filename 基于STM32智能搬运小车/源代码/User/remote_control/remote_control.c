#include "remote_control.h"
void Remote_Control(void)
{
	
	
	
	KeyNum = ps2_key_serch();			//只处理按键部分  	获取按键按下的数量

	PS2_Mode = ps2_mode_get();			//选择模式---红灯或绿灯
	
	
		
		if (PS2_Mode != Last_PS2_Mode)	//如果小车在运动过程中切换模式时，让其停下复位
		{
			Car_Forward(0);
			Arc_Pwm = Arc_Servo_Param_Get(SERVO_MIDDLE);
			Arc_ServoPwm_Set(Arc_Pwm);
		}
		
		if (PS2_Mode == PSB_REDLIGHT_MODE)
		{
			unsigned char ps2_lx,ps2_ly,ps2_rx,ps2_ry;
			
			//阿克曼舵机控制
			ps2_lx = ps2_get_anolog_data(PSS_LX);
			Arc_Pwm = (unsigned int)(ps2_lx*0.235f + Arc_LeftLimit);
			Arc_ServoPwm_Set(Arc_Pwm);
			
			//电机控制
			ps2_ly = ps2_get_anolog_data(PSS_LY);
			if ( ps2_ly <= 0x6f)
			{
				Car_Forward(500);
			}
			else if (ps2_ly >= 0x8f)
			{
				Car_Backward(500);
			}
			else
			{
				Car_Forward(0);				
			}
			
			//机械臂控制
			ps2_rx = ps2_get_anolog_data(PSS_RX);
			if (ps2_rx == 0x00)
			{
				RobotArm_DropHand(UNIT_PWM);
			}
			else if (ps2_rx == 0xFF)
			{
				RobotArm_RaiseHand(UNIT_PWM);
			}
			
			ps2_ry = ps2_get_anolog_data(PSS_RY);
			if (ps2_ry == 0x00)
			{
				RobotArm_StrechHand(UNIT_PWM);
			}
			else if (ps2_ry == 0xFF)
			{
				RobotArm_ShinkHand(UNIT_PWM);
			}		
			
			if (ps2_get_key_state(PSB_R1))
			{
				RobotArm_ShakeHand(UNIT_PWM);
			}
			else if (ps2_get_key_state(PSB_R2))
			{
				RobotArm_LetHand(UNIT_PWM);
			}
		}	
		else if (PS2_Mode == PSB_GREENLIGHT_MODE)
		{
			if (KeyNum)
			{
				if (ps2_get_key_state(PSB_PAD_UP) || ps2_get_key_state(PSB_L1))
				{
					Car_Forward(500);
				}
				else if (ps2_get_key_state(PSB_PAD_DOWN) || ps2_get_key_state(PSB_L2))
				{
					Car_Backward(500);
				}
				else
				{
					Car_Forward(0);
				}

				if (ps2_get_key_state(PSB_PAD_LEFT))
				{
					Arc_Pwm -= UNIT_PWM;
					//限幅
					if (Arc_Pwm < Arc_LeftLimit)
					{
						Arc_Pwm = Arc_LeftLimit;
					}
					else if (Arc_Pwm > Arc_RightLimit)
					{
						Arc_Pwm = Arc_RightLimit;
					}
					Arc_ServoPwm_Set(Arc_Pwm);
				}
				else if (ps2_get_key_state(PSB_PAD_RIGHT))
				{
					Arc_Pwm += UNIT_PWM;
					//限幅
					if (Arc_Pwm < Arc_LeftLimit)
					{
						Arc_Pwm = Arc_LeftLimit;
					}
					else if (Arc_Pwm > Arc_RightLimit)
					{
						Arc_Pwm = Arc_RightLimit;
					}
					Arc_ServoPwm_Set(Arc_Pwm);
				}	

				//机械臂控制
				if (ps2_get_key_state(PSB_PINK))
				{
					RobotArm_DropHand(UNIT_PWM);
				}
				else if (ps2_get_key_state(PSB_RED))
				{
					RobotArm_RaiseHand(UNIT_PWM);
				}
				else if (ps2_get_key_state(PSB_GREEN))
				{
					RobotArm_StrechHand(UNIT_PWM);
				}
				else if (ps2_get_key_state(PSB_BLUE))
				{
					RobotArm_ShinkHand(UNIT_PWM);
				}
				else if (ps2_get_key_state(PSB_R1))
				{
					RobotArm_ShakeHand(UNIT_PWM);
				}
				else if (ps2_get_key_state(PSB_R2))
				{
					RobotArm_LetHand(UNIT_PWM);
				}
			}
			else
			{
				Car_Forward(0);
			}
		}
		else
		{
			Car_Forward(0);
			Arc_Pwm = Arc_Servo_Param_Get(SERVO_MIDDLE);
			Arc_ServoPwm_Set(Arc_Pwm);
		}
		
		Last_PS2_Mode = PS2_Mode;
		Delay_ms(20);
		
}

