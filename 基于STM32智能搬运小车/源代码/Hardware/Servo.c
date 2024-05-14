#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "Servo.h"

struct SERVO_PARAM arc_servo = {150, 120, 150, 180};
struct SERVO_PARAM robotarm_left = {200, 120, 200, 250}; 			//机械臂左舵机   控制机械臂向上向下
struct SERVO_PARAM robotarm_right = {180, 120, 180, 250};			//机械臂右舵机	控制机械臂向外向内
struct SERVO_PARAM robotarm_up = {150, 100, 150, 250};				//机械臂前舵机	控制机械臂夹取物体

//舵机的初始化
void Servo_Init()
{
	Servo_Gpio_Init();
	Servo_Time_Init();
	Robot_Arm_Gpio_Init();
	
	Arc_ServoPwm_Set(arc_servo.middle);			//复位   让舵机处于中间值
}

uint8_t Arc_Servo_Param_Get(uint8_t servo_param)
{
	uint8_t param_temp;
	if (servo_param == SERVO_MIDDLE)
	{
		param_temp = arc_servo.middle;
	}
	else if (servo_param == SERVO_LEFT_LIMIT)
	{
		param_temp = arc_servo.left_limit;
	}
	else if (servo_param == SERVO_RIGHT_LIMIT)
	{
		param_temp = arc_servo.right_limit;
	}
	else
	{
		param_temp = arc_servo.middle;
	}
	return param_temp;
}

void Arc_ServoPwm_Set(uint16_t value)
{
	//限幅
	if (value < arc_servo.left_limit)
	{
		value = arc_servo.left_limit;
	}
	else if (value > arc_servo.right_limit)
	{
		value = arc_servo.right_limit;
	}
	
	arc_servo.pwm = value;
}

//机械臂向上抬起
void RobotArm_RaiseHand(uint8_t unit_pwm)
{
	unit_pwm = robotarm_left.pwm + unit_pwm;
	Robot_LeftPwm_Set(unit_pwm);
}


//机械臂向下放下
void RobotArm_DropHand(uint8_t unit_pwm)
{
	unit_pwm = robotarm_left.pwm - unit_pwm;
	Robot_LeftPwm_Set(unit_pwm);
}	


//机械臂向外伸展
void RobotArm_StrechHand(uint8_t unit_pwm)
{
	unit_pwm = robotarm_right.pwm + unit_pwm;
	Robot_RightPwm_Set(unit_pwm);
}


//机械臂向内收缩
void RobotArm_ShinkHand(uint8_t unit_pwm)
{
	unit_pwm = robotarm_right.pwm - unit_pwm;
	Robot_RightPwm_Set(unit_pwm);
}


//机械臂夹取
void RobotArm_ShakeHand(uint8_t unit_pwm)
{
	unit_pwm = robotarm_up.pwm - unit_pwm;
	Robot_UpPwm_Set(unit_pwm);
}

//机械臂松开
void RobotArm_LetHand(uint8_t unit_pwm)
{
	unit_pwm = robotarm_up.pwm + unit_pwm;
	Robot_UpPwm_Set(unit_pwm);
}
/*************************************/
void Servo_Gpio_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

void Robot_Arm_Gpio_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;  //左、前、右
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
}

void Servo_Time_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_InternalClockConfig(TIM1);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 10-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM1, ENABLE);
}

//设置左舵机的PWM-------控制向上向下
void Robot_LeftPwm_Set(uint16_t value)
{
	//限幅
	if(value > robotarm_left.right_limit)
	{
		value = robotarm_left.right_limit;
	}
	else if (value < robotarm_left.left_limit)
	{
		value = robotarm_left.left_limit;
	}
	
	robotarm_left.pwm = value;
}


//设置右舵机的PWM-------控制向内向外
void Robot_RightPwm_Set(uint16_t value)
{
	//限幅
	if(value > robotarm_right.right_limit)
	{
		value = robotarm_right.right_limit;
	}
	else if (value < robotarm_right.left_limit)
	{
		value = robotarm_right.left_limit;
	}
	
	robotarm_right.pwm = value;
}

//设置前舵机的PWM-------控制夹取
void Robot_UpPwm_Set(uint16_t value)
{
	//限幅
	if(value > robotarm_up.right_limit)
	{
		value = robotarm_up.right_limit;
	}
	else if (value < robotarm_up.left_limit)
	{
		value = robotarm_up.left_limit;
	}
	
	robotarm_up.pwm = value;
}

uint16_t counter;
void TIM1_UP_IRQHandler()
{
	if (TIM_GetITStatus(TIM1,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
		
		counter++;
		if (counter > 2000)
		{
			counter = 0;
		}

		//PWM模拟		
		if (counter < arc_servo.pwm)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
		}
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		}
		
		//左舵机
		if (counter < robotarm_left.pwm)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_13);
		}
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_13);
		}
		
		//前舵机
		if (counter < robotarm_up.pwm)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_14);
		}
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_14);
		}	
		
		//右舵机
		if (counter < robotarm_right.pwm)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_15);
		}
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_15);
		}		
	}
}
