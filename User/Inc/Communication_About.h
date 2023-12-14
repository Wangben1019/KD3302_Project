#ifndef __COMMUNICATION_ABOUT_H__
#define __COMMUNICATION_ABOUT_H__

#include "Task_Init.h"

#if Debug_Remote
#define Remote_Leng 30
extern uint8_t Remote_RxArr[Remote_Leng];

extern SemaphoreHandle_t Remote_DMA_Finish_Semaphore;
extern TaskHandle_t Remote_Task_Handle;
void Remote_Task(void *pvParameters);
#endif

typedef struct
{
    uint8_t Push_Control_Info_Draining;
    uint8_t Push_Control_Info_Tank;
    uint8_t Jack_Control_Info_Down;
    uint8_t Jack_Control_Info_Up;
    uint8_t Screwdriver_Control_Info;
    uint8_t none1;
    uint8_t none2;
    uint8_t none3;
} Control_Info_s;

typedef union
{
    uint8_t data[8];
    Control_Info_s Control_Info;
}Transmit_Can_Info_u;
extern Transmit_Can_Info_u Transmit_Can_Info;

#endif 
