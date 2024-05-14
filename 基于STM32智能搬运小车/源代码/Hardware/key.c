#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#include "LED.h"
void Key_Init(void)
{
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);
	
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line0 | EXTI_Line1 | EXTI_Line4 | EXTI_Line5;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	
	
	EXTI_Init(&EXTI_InitStruct);
	
	
	NVIC_InitStruct.NVIC_IRQChannel =EXTI0_IRQn ;
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE ;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel =EXTI1_IRQn ;
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE ;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel =EXTI4_IRQn ;
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE ;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel =EXTI9_5_IRQn ;
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE ;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	
	NVIC_Init(&NVIC_InitStruct);
}
uint8_t Key_Num=0;
void EXTI0_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line0)==SET)
	{
		
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)
		{
			Key_Num=1;
		}
	}
		
		
		EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI1_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line1)==SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
		{
			Key_Num=2;
		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}
void EXTI4_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line4)==SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)==0)
		{
			Key_Num=3;
		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}
void EXTI9_5_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line5)==SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0)
		{
			Key_Num=4;
		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line5);
}

