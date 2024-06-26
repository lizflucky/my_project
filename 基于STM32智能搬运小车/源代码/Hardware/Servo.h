#ifndef _SERVO_H_
#define _SERVO_H_

typedef enum
{
	SERVO_MIDDLE = 0,
	SERVO_LEFT_LIMIT,
	SERVO_RIGHT_LIMIT,
}servo_limit;

struct SERVO_PARAM
{
	uint8_t pwm;			//当前的pwm
	uint8_t left_limit;		//左的极限值
	uint8_t middle;			//中间值
	uint8_t right_limit;	//右的极限值
};

void Servo_Init(void);		//初始化舵机
uint8_t Arc_Servo_Param_Get(uint8_t servo_param);
void Arc_ServoPwm_Set(uint16_t value);
void Servo_Gpio_Init(void);
void Servo_Time_Init(void);

void Robot_Arm_Gpio_Init(void);
void Robot_LeftPwm_Set(uint16_t value);
void Robot_RightPwm_Set(uint16_t value);
void Robot_UpPwm_Set(uint16_t value);
void RobotArm_RaiseHand(uint8_t unit_pwm);
void RobotArm_DropHand(uint8_t unit_pwm);
void RobotArm_StrechHand(uint8_t unit_pwm);
void RobotArm_ShinkHand(uint8_t unit_pwm);
void RobotArm_ShakeHand(uint8_t unit_pwm);
void RobotArm_LetHand(uint8_t unit_pwm);
#endif

