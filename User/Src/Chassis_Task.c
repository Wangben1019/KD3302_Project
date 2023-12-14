#include "Chassis_Task.h"
//#include "CyberGear.h"

MI_Motor_s MI_Chassis_Motor[4];

Robot_Chassis_Exp_ Robot_Chassis_Exp;
Chassis_Motor_Speed Chassis_Motor_Exp;

Vision_goal_expect_s Vision_goal_expect;

uint8_t flag_chassis = 0;

void Chassis_Motor_Init(void);

TaskHandle_t Chassis_Task_Handle;
void Chassis_Control_Task(void *pvParameters)
{
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for(;;)
    {
        if(JoyGimbal_Control_Exp.Stop == 1)
        {
            Robot_Chassis_Exp.Expect_Speed_X = 0;
            Robot_Chassis_Exp.Expect_Speed_Y = 0;
            Robot_Chassis_Exp.Expect_Speed_Yaw = 0;
            
            Chassis_Motor_Exp.speed_1 = 0;
            Chassis_Motor_Exp.speed_2 = 0;
            Chassis_Motor_Exp.speed_3 = 0;
            Chassis_Motor_Exp.speed_4 = 0;
        }
        
        if(flag_chassis == 0)
        {
            Chassis_Motor_Init();
            flag_chassis = 1;
        }
        
//        if(Vision_goal_expect.Vision_goal_x > 0.3f)
//        {
//            Robot_Chassis_Exp.Expect_Speed_X = 3;
//        }
//        else if(Vision_goal_expect.Vision_goal_x < -0.3f)
//        {
//            Robot_Chassis_Exp.Expect_Speed_X = -3;
//        }
////        else if(Vision_goal_expect.Vision_goal_x != 0)
//        else
//        {
//            Robot_Chassis_Exp.Expect_Speed_X = 0;
//        }
//        
//        if(Vision_goal_expect.Vision_goal_y > 0.04f)
//        {
//            Robot_Chassis_Exp.Expect_Speed_Yaw = 1; 
//        }
//        else if(Vision_goal_expect.Vision_goal_y < -0.04f)
//        {
//            Robot_Chassis_Exp.Expect_Speed_Yaw = -1;
//        }
////        else if(Vision_goal_expect.Vision_goal_y != 0)
//        else
//        {
//            Robot_Chassis_Exp.Expect_Speed_Yaw = 0;
//        }
        
        Chassis_Motor_Exp.speed_1 = Robot_Chassis_Exp.Expect_Speed_X - Robot_Chassis_Exp.Expect_Speed_Y - Robot_Chassis_Exp.Expect_Speed_Yaw;
        Chassis_Motor_Exp.speed_2 = -Robot_Chassis_Exp.Expect_Speed_X - Robot_Chassis_Exp.Expect_Speed_Y - Robot_Chassis_Exp.Expect_Speed_Yaw;
        Chassis_Motor_Exp.speed_3 = Robot_Chassis_Exp.Expect_Speed_X + Robot_Chassis_Exp.Expect_Speed_Y - Robot_Chassis_Exp.Expect_Speed_Yaw;
        Chassis_Motor_Exp.speed_4 = -Robot_Chassis_Exp.Expect_Speed_X + Robot_Chassis_Exp.Expect_Speed_Y - Robot_Chassis_Exp.Expect_Speed_Yaw;

//        Chassis_Motor_Exp.speed_1 = 0;
//        Chassis_Motor_Exp.speed_2 = 0;
//        Chassis_Motor_Exp.speed_3 = 0;
//        Chassis_Motor_Exp.speed_4 = 0;
        
        if(MI_Chassis_Motor[0].motor_mode_state != RUN_MODE)
        {
            MI_motor_Enable(&MI_Chassis_Motor[0]);
            vTaskDelay(5);
        }
        
        if(MI_Chassis_Motor[1].motor_mode_state != RUN_MODE)
        {
            MI_motor_Enable(&MI_Chassis_Motor[1]);
            vTaskDelay(5);
        }
        
        if(MI_Chassis_Motor[2].motor_mode_state != RUN_MODE)
        {
            MI_motor_Enable(&MI_Chassis_Motor[2]);
            vTaskDelay(5);
        }
        
        if(MI_Chassis_Motor[3].motor_mode_state != RUN_MODE)
        {
            MI_motor_Enable(&MI_Chassis_Motor[3]);
            vTaskDelay(5);
        }
        
        
        MI_motor_SpeedControl(&MI_Chassis_Motor[0], Chassis_Motor_Exp.speed_1 * 5, 0.5);
        vTaskDelay(20);
        MI_motor_SpeedControl(&MI_Chassis_Motor[1], Chassis_Motor_Exp.speed_2 * 5, 0.5);
        vTaskDelay(20);
        MI_motor_SpeedControl(&MI_Chassis_Motor[2], Chassis_Motor_Exp.speed_3 * 5, 0.5);
        vTaskDelay(20);
        MI_motor_SpeedControl(&MI_Chassis_Motor[3], Chassis_Motor_Exp.speed_4 * 5, 0.5);
        vTaskDelay(20);

        vTaskDelayUntil(&xLastWakeTime,2);
    }
}

void Chassis_Motor_Init(void)
{
//    for(int i = 0;i < 5; i++)
//    {
        MI_motor_Enable(&MI_Chassis_Motor[0]);
        vTaskDelay(5);
        
        MI_motor_Enable(&MI_Chassis_Motor[1]);
        vTaskDelay(5);
        
        MI_motor_Enable(&MI_Chassis_Motor[2]);
        vTaskDelay(5);
        
        MI_motor_Enable(&MI_Chassis_Motor[3]);
        vTaskDelay(5);    
//    }
}
