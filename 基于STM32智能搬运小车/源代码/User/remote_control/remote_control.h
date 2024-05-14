#ifndef __REMOTE_CONTROL__H_
#define __REMOTE_CONTROL__H_
#include "stm32f10x.h"  
#include "Servo.h"
#include "PS2.h"
#include "Motor.h"
#include "Delay.h"


#define   UNIT_PWM	3

extern uint8_t KeyNum;
extern uint8_t PS2_Mode,Last_PS2_Mode;
extern uint8_t Arc_Pwm, Arc_LeftLimit,Arc_RightLimit;

void Remote_Control(void);



#endif

