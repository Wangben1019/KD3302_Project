#ifndef __CHASSIS_TASK_H__
#define __CHASSIS_TASK_H__

#include "Task_Init.h"
#include "CyberGear.h"

typedef struct{
    double Expect_Speed_X;
    double Expect_Speed_Y;
    double Expect_Speed_Yaw;
} Robot_Chassis_Exp_;

extern Robot_Chassis_Exp_ Robot_Chassis_Exp;

extern MI_Motor_s MI_Chassis_Motor[4];

typedef struct
{
    float Vision_goal_x;
    float Vision_goal_y;
} Vision_goal_expect_s;
extern Vision_goal_expect_s Vision_goal_expect;

extern TaskHandle_t Chassis_Task_Handle;
void Chassis_Control_Task(void *pvParameters);

//void Chassis_Motor_Init(void);

#endif 
