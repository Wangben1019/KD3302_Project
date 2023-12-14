#include "Travel_Switch.h"

uint8_t Travel_Switch_State[9];

TaskHandle_t Read_Travel_Switch_Task_Handle;
void Read_Travel_Switch_Task(void *pvParameters)
{
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for(;;)
    {
        /* 0为遮挡 */
//        Travel_Switch_State[0] = Get_Travel_Switch_PA7;
//        Travel_Switch_State[1] = Get_Travel_Switch_PC4;
//        Travel_Switch_State[2] = Get_Travel_Switch_PC5;
//        Travel_Switch_State[3] = Get_Travel_Switch_PB0;
//        Travel_Switch_State[4] = Get_Travel_Switch_PB1;
//        Travel_Switch_State[5] = Get_Travel_Switch_PE9;
//        Travel_Switch_State[6] = Get_Travel_Switch_PE11;
//        Travel_Switch_State[7] = Get_Travel_Switch_PE13;
//        Travel_Switch_State[8] = Get_Travel_Switch_PE14;
        
        if(Get_Gimbal_X_Forward_Limit == 0)
        {
            Gimbal_Motor_X.Gimbal_Sport_Limit = Forward_Limit;
        }
        else if(Get_Gimbal_X_Backward == 0)
        {
            Gimbal_Motor_X.Gimbal_Sport_Limit = Backward_Limit;
        }
        else if(Get_Gimbal_X_Forward_Limit == 1 && Get_Gimbal_X_Backward == 1)
        {
            Gimbal_Motor_X.Gimbal_Sport_Limit = None_Limit;
        }
        
        if(Get_Gimbal_Y_Forward_Limit == 0)
        {
            Gimbal_Motor_Y.Gimbal_Sport_Limit = Forward_Limit;
        }
        else if(Get_Gimbal_Y_Backward == 0)
        {
            Gimbal_Motor_Y.Gimbal_Sport_Limit = Backward_Limit;
        }
        else if(Get_Gimbal_Y_Forward_Limit == 1 && Get_Gimbal_Y_Backward == 1)
        {
            Gimbal_Motor_Y.Gimbal_Sport_Limit = None_Limit;
        }
        
        if(Get_Gimbal_Z_Up == 0)
        {
            Gimbal_Motor_Z.Gimbal_Sport_Limit = Forward_Limit;
        }
        else if(Get_Gimbal_Z_Down == 0)
        {
            Gimbal_Motor_Z.Gimbal_Sport_Limit = Backward_Limit;
        }
        else if(Get_Gimbal_Z_Up == 1 && Get_Gimbal_Z_Down == 1)
        {
            Gimbal_Motor_Z.Gimbal_Sport_Limit = None_Limit;
        }
        
        vTaskDelayUntil(&xLastWakeTime,2);
    }
}
