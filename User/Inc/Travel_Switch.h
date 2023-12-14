#ifndef __TRAVEL_SWITCH_H__
#define __TRAVEL_SWITCH_H__

#include "Task_Init.h"

#define Get_Travel_Switch_PA7 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)
#define Get_Gimbal_X_Mid Get_Travel_Switch_PA7

#define Get_Travel_Switch_PC4 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4)
#define Get_Gimbal_X_Forward_Limit Get_Travel_Switch_PC4

#define Get_Travel_Switch_PC5 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5)
#define Get_Gimbal_X_Backward Get_Travel_Switch_PC5

#define Get_Travel_Switch_PE9 HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9)
#define Get_Gimbal_Y_Mid Get_Travel_Switch_PE9

#define Get_Travel_Switch_PE11 HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11)
#define Get_Gimbal_Y_Forward_Limit Get_Travel_Switch_PE11

#define Get_Travel_Switch_PE13 HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13)
#define Get_Gimbal_Y_Backward Get_Travel_Switch_PE13

#define Get_Travel_Switch_PE14 HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14)
#define Get_Gimbal_Z_Mid Get_Travel_Switch_PE14

#define Get_Travel_Switch_PB0 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)
#define Get_Gimbal_Z_Up Get_Travel_Switch_PB0

#define Get_Travel_Switch_PB1 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)
#define Get_Gimbal_Z_Down Get_Travel_Switch_PB1


extern TaskHandle_t Read_Travel_Switch_Task_Handle;
void Read_Travel_Switch_Task(void *pvParameters);

#endif 
