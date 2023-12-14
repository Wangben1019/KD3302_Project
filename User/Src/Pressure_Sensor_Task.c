#include "Pressure_Sensor_Task.h"
#include "Communication_About.h"

uint8_t Pressure_Sensor_Data_No1[Sensor_DataLong];
uint8_t Pressure_Sensor_Data_No2[Sensor_DataLong];
uint8_t Pressure_Sensor_Data_No3[Sensor_DataLong];

uint8_t draining_need_return;
uint8_t tank_need_return;

uint8_t jack_up_need_return;
uint8_t jack_down_need_return;

Pressure_Sensor_s Pressure_Sensor_Screwdriver;
Pressure_Sensor_s Pressure_Sensor_Jack_Down;
Pressure_Sensor_s Pressure_Sensor_Jack_Up;

System_Run_State_e System_Run_State = Execute_Command;
System_Run_State_e Last_System_Run_State = Execute_Command;

uint8_t down_finish_one_tages = 0;
uint8_t up_finish_one_tages = 0;
uint8_t Screwdriver_Start_Flag = 0;
uint8_t jack_return_flag;
//float Gimbal_Z_Start_Dis = 0;
//float D_Gimbal_Z_Move_Dis = 0;

uint8_t push_return_tank_remote_flag;
uint8_t push_return_draining_remote_flag;

uint8_t get_xy_start_flag;
uint8_t get_x_end_flag;
uint8_t get_y_end_flag;

uint32_t pressure_max; 
uint8_t succeed_working;

TaskHandle_t Master_Controller_Task_Handle;
void Master_Controller_Task(void *pvParameters)
{
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for(;;)
    {
        if(JoyGimbal_Control_Exp.set_finish_lock == -1)
        {
            __HAL_TIM_SET_AUTORELOAD(&htim4, 49);
            succeed_working = 0;
        }
        else if(JoyGimbal_Control_Exp.set_finish_lock == 1)
        {
            __HAL_TIM_SET_AUTORELOAD(&htim4, 1053);
            succeed_working = 1;
        }
        
        if(JoyGimbal_Control_Exp.Reset_Push_Tank == 1)
        {
            tank_need_return = 1;
            push_return_tank_remote_flag = 1;
        }
        
        if(JoyGimbal_Control_Exp.Reset_Push_Draining == 1)
        {
            draining_need_return = 1;
            push_return_draining_remote_flag = 1;
        }
        
        if(push_return_draining_remote_flag == 1)
        {
            if(draining_need_return == 1)
            {
                Transmit_Can_Info.Control_Info.Push_Control_Info_Draining = 2;
            }
            else if(draining_need_return == 0)
            {
                Transmit_Can_Info.Control_Info.Push_Control_Info_Draining = 0;
                push_return_draining_remote_flag = 0;
            }    
        }
        
        if(push_return_tank_remote_flag == 1)
        {
            if(tank_need_return == 1)
            {
                Transmit_Can_Info.Control_Info.Push_Control_Info_Tank = 2;
            }
            else if(tank_need_return == 0)
            {
                Transmit_Can_Info.Control_Info.Push_Control_Info_Tank = 0;
                push_return_tank_remote_flag = 0;
            }    
        }
        
        
        if(JoyGimbal_Control_Exp.Set_Jack_Wroking == 1)
        {
            System_Run_State = Jack_Down_Run;
        }
        
        if(JoyGimbal_Control_Exp.Set_Push_Working == 1)
        {
            System_Run_State = Pushing_electric_push;
        }
        
        /* 锁定到油口，下千斤顶准备工作 */
        if(Gimbal_Vision_Control_Exp.Exp_Vision_X != 320 || Gimbal_Vision_Control_Exp.Exp_Vision_Y != 240)
        {
            if(System_Run_State == Execute_Command)
            {
//                System_Run_State = Jack_Down_Run;
                Last_System_Run_State = Execute_Command;
            }
        }
        
        /* 下千斤顶工作 */
        if(System_Run_State == Jack_Down_Run)
        {
            /* 小于阈值，顶出 */
            if(Pressure_Sensor_Jack_Down.Perssure <= Jack_Down_Pressure_Thre1)
            {
                Transmit_Can_Info.Control_Info.Jack_Control_Info_Down = 1;
            }
            /* 大于阈值，停止顶出，并切换下一个状态，上千斤顶准备工作 */
            else 
            {
                Transmit_Can_Info.Control_Info.Jack_Control_Info_Down = 0;
                System_Run_State = Jack_Up_Run;
                Last_System_Run_State = Jack_Down_Run;
            }
        }
        /* 上千斤顶工作 */
        else if(System_Run_State == Jack_Up_Run)
        {
            /* 小于阈值，顶出 */
            if(Pressure_Sensor_Jack_Up.Perssure <= Jack_Up_Pressure_Thre1)
            {
                Transmit_Can_Info.Control_Info.Jack_Control_Info_Up = 1;
            }
            /* 大于阈值，停止顶出，并切换下一个状态，双千斤顶准备二次顶出 */
            else 
            {
                Transmit_Can_Info.Control_Info.Jack_Control_Info_Up = 0;
                System_Run_State = Jack_All_Run;
                Last_System_Run_State = Jack_Up_Run;
            }
        }
        else if(System_Run_State == Jack_All_Run)
        {
            /* 使上下千斤顶分别达到阈值 */
            if(Pressure_Sensor_Jack_Down.Perssure <= Jack_All_Pressure_Thre && down_finish_one_tages == 0)
            {
                Transmit_Can_Info.Control_Info.Jack_Control_Info_Down = 1;
            }
            else 
            {
                Transmit_Can_Info.Control_Info.Jack_Control_Info_Down = 0;
                down_finish_one_tages = 1;
            }
            
            if(Pressure_Sensor_Jack_Up.Perssure <= Jack_All_Pressure_Thre && up_finish_one_tages == 0)
            {
                Transmit_Can_Info.Control_Info.Jack_Control_Info_Up = 1;
            }
            else 
            {
                Transmit_Can_Info.Control_Info.Jack_Control_Info_Up = 0;
                up_finish_one_tages = 1;
            }
            
            /* 分别达到阈值后，完成工作，准备微调对准油口 */
            if(down_finish_one_tages == 1 && up_finish_one_tages == 1)
            {
                System_Run_State = Jack_Finish;
                Last_System_Run_State = Jack_All_Run;
                down_finish_one_tages = 0;
                up_finish_one_tages = 0;
                jack_return_flag = 0;
            }
        }
        
        /* 千斤顶准备完毕，并且油口对准成功，开始下一步工作 */
        if(JoyGimbal_Control_Exp.Control_Screwdriver_Run == 1)
        {
            /* 转动电动螺丝刀 */
            Transmit_Can_Info.Control_Info.Screwdriver_Control_Info = 1;
            /* 第一次转动，记录Z轴初值 */
//            if(Screwdriver_Start_Flag == 0)
//            {
//                Gimbal_Position_Info.Gimbal_Z_Up_Start_Pose = Gimbal_Motor_Z.Move_Dis;
//                Screwdriver_Start_Flag = 1;
//            }
//            if(pressure_max < Pressure_Sensor_Screwdriver.Perssure)
//                pressure_max = Pressure_Sensor_Screwdriver.Perssure;
//            else if(pressure_max > Pressure_Sensor_Screwdriver.Perssure && Pressure_Sensor_Screwdriver.Perssure > 150)
//                succeed_working = 1;
//            if(JoyGimbal_Control_Exp.set_finish_lock == 1)
//            {
//                Gimbal_Position_Info.Gimbal_Z_Up_Start_Pose = Gimbal_Motor_Z.Move_Dis;
//                succeed_working = 1;
//            }
            
//            __HAL_TIM_SET_AUTORELOAD(&htim4, 1053);
            /* 电动螺丝刀压力传感器大于工作阈值，证明电动螺丝刀开始拧螺丝 */
            if(succeed_working == 1)
            {
                /* 拧螺丝的同时Z轴配合向下 */
                if(Screwdriver_Start_Flag == 0)
                {
                    Gimbal_Position_Info.Gimbal_Z_Up_Start_Pose = Gimbal_Motor_Z.Move_Dis;
                    Screwdriver_Start_Flag = 1;
                }
                Set_GimbalMotor_Dir(&Gimbal_Motor_Z, 1);
                GimbalMotor_StartSport(&Gimbal_Motor_Z);
                Gimbal_Position_Info.D_Gimbal_Z_Move_Dis = Gimbal_Position_Info.Gimbal_Z_Up_Start_Pose - Gimbal_Motor_Z.Move_Dis;
            }
            /* 计算Z轴走过的距离 */          
            
        }
        else if(JoyGimbal_Control_Exp.Control_Screwdriver_Run == 0)
        {
            Transmit_Can_Info.Control_Info.Screwdriver_Control_Info = 0;
        }
        /* Z轴走过的距离大于应该拧下的距离，则视为拧完 */
        /* 可以利用压力传感器，但是有别的情况 */
        if(__fabs(Gimbal_Position_Info.D_Gimbal_Z_Move_Dis) > Unscrew_Dis && System_Run_State == Jack_Finish)
        {
            /* 停止转动电动螺丝刀 */
            Transmit_Can_Info.Control_Info.Screwdriver_Control_Info = 0;
            /* Z轴停止移动 */
            GimbalMotor_StopSport(&Gimbal_Motor_Z);
            __HAL_TIM_SET_AUTORELOAD(&htim4, 49);
            Jack_Up_Remote_Return = 1;
            Jack_Down_Remote_Return = 1;
            //vTask_Delay(1000);
            /* 进入下一个状态，切换放油针至电动螺丝刀处 */
            System_Run_State = Switch_Drain_Oil;
            Gimbal_Working_Condition = Auto_Switch_Drain_Oil;
            Last_System_Run_State = Jack_Finish;
            /* 记录X，Y轴开始切换的位置 */
            if(get_xy_start_flag == 0)
            {
                Gimbal_Position_Info.Gimbal_X_Start_Pose = Gimbal_Motor_X.Move_Dis;
                Gimbal_Position_Info.Gimbal_Y_Start_Pose = Gimbal_Motor_Y.Move_Dis;
                get_xy_start_flag = 1;
            }
        }
        
        if(System_Run_State == Pushing_electric_push)
        {
            Transmit_Can_Info.Control_Info.Push_Control_Info_Draining = 1;
        }
        else if(System_Run_State == Draining_Oil)
        {
            Transmit_Can_Info.Control_Info.Push_Control_Info_Draining = 0;
        }
        else if(System_Run_State == Push_Out_Tank)
        {
            Transmit_Can_Info.Control_Info.Push_Control_Info_Tank = 1;
            if(draining_need_return == 1)
            {
                Transmit_Can_Info.Control_Info.Push_Control_Info_Draining = 2;
            }
            else if(draining_need_return == 0)
            {
                Transmit_Can_Info.Control_Info.Push_Control_Info_Draining = 0;
            }
        }
        else if(System_Run_State == Push_Return_Tank)
        {
            if(tank_need_return == 1)
            {
                Transmit_Can_Info.Control_Info.Push_Control_Info_Tank = 2;
            }
            else if(tank_need_return == 0)
            {
                Transmit_Can_Info.Control_Info.Push_Control_Info_Tank = 0;
            }
        }
        
        else if(System_Run_State == Finish_A_Reset)
        {
            Gimbal_Working_Condition = Lock_A_Adjust;
            Gimbal_Motor_X.All_Finish_Vision_Task_Flag = 0;
            Gimbal_Motor_X.All_Vision_Switch_Finish = 0;
            Gimbal_Motor_X.Vision_Finish_Task_Flag = 0;
            Gimbal_Motor_X.Vision_Switch_Finish = 0;
            Gimbal_Motor_X.Vision_Control_Flag = 0;
            Gimbal_Motor_X.Get_Finish_Point_Flag = 0;
            
            Gimbal_Motor_Y.All_Finish_Vision_Task_Flag = 0;
            Gimbal_Motor_Y.All_Vision_Switch_Finish = 0;
            Gimbal_Motor_Y.Vision_Finish_Task_Flag = 0;
            Gimbal_Motor_Y.Vision_Switch_Finish = 0;
            Gimbal_Motor_Y.Vision_Control_Flag = 0;
            Gimbal_Motor_Y.Get_Finish_Point_Flag = 0;

            if(jack_return_flag == 0)
            {
                jack_up_need_return = 1;
                jack_down_need_return = 1;
                jack_return_flag = 1;
            }
            Gimbal_Working_Condition = Lock_A_Adjust;
            
//            if(Last_System_Run_State == Push_Out_Tank)
//            {
//                if(tank_need_return == 1)
//                {
//                    Transmit_Can_Info.Control_Info.Push_Control_Info_Tank = 2;
//                }
//                else if(tank_need_return == 0)
//                {
//                    Transmit_Can_Info.Control_Info.Push_Control_Info_Tank = 0;
//                }
//            }
            if(jack_up_need_return == 1)
            {
                Transmit_Can_Info.Control_Info.Jack_Control_Info_Up = 2;
            }
            else if(jack_up_need_return == 0)
            {
                Transmit_Can_Info.Control_Info.Jack_Control_Info_Up = 0;
            }
            
            if(jack_down_need_return == 1)
            {
                Transmit_Can_Info.Control_Info.Jack_Control_Info_Down = 2;
            }
            else if(jack_down_need_return == 0)
            {
                Transmit_Can_Info.Control_Info.Jack_Control_Info_Down = 0;
            }
        }
        
        if(Pressure_Sensor_Jack_Down.Perssure > 10000)
        {
            Transmit_Can_Info.Control_Info.Jack_Control_Info_Down = 2;
        }
        if(Pressure_Sensor_Jack_Down.Perssure > 20000)
        {
            Transmit_Can_Info.Control_Info.Jack_Control_Info_Down = 0;
        }
        if(Pressure_Sensor_Jack_Up.Perssure > 10000)
        {
            Transmit_Can_Info.Control_Info.Jack_Control_Info_Up = 2;
        }
        if(Pressure_Sensor_Jack_Up.Perssure > 20000)
        {
            Transmit_Can_Info.Control_Info.Jack_Control_Info_Up = 0;            
        }
        
        if(Jack_Up_FordwareRunTime >= Jack_Up_Run_Time_Thre)
        {
            Transmit_Can_Info.Control_Info.Jack_Control_Info_Up = 0;
            up_finish_one_tages = 1;
        }
        if(Jack_Down_FordwareRunTime >= Jack_Down_Run_Time_Thre)
        {
            Transmit_Can_Info.Control_Info.Jack_Control_Info_Down = 0;
            down_finish_one_tages = 1;
        }
        
        if(down_finish_one_tages == 1 && up_finish_one_tages == 1)
        {
            System_Run_State = Jack_Finish;
            Last_System_Run_State = Jack_All_Run;
            down_finish_one_tages = 0;
            up_finish_one_tages = 0;
            jack_return_flag = 0;
        }
        
        if(Pressure_Sensor_Jack_Down.Sensor_State == Sensor_State_Unknown)
        {
            Transmit_Can_Info.Control_Info.Jack_Control_Info_Down = 0;
        }
        if(Pressure_Sensor_Jack_Up.Sensor_State == Sensor_State_Unknown)
        {
            Transmit_Can_Info.Control_Info.Jack_Control_Info_Up = 0;
        }
        
        Transmit_Can_Info.Control_Info.none1 = 1;
        Transmit_Can_Info.Control_Info.none2 = 2;
        Transmit_Can_Info.Control_Info.none3 = 3;
        CAN_Send_StdDataFrame(&hcan1, 0x222, (uint8_t *)&Transmit_Can_Info);

        vTaskDelayUntil(&xLastWakeTime,10);
    }
}

uint32_t pow_user(uint8_t input, uint8_t power)
{
    uint32_t end = 1;
    for(int i = 0;i < power;i++)
    {
        end *= input;
    }
    return end;
}

/* 单位：N */

void Pressure_Data_Decode(uint8_t *RxMsg, Pressure_Sensor_s *Dst)
{
    Dst->Last_Perssure = Dst->Perssure;
    uint8_t lable = 11;
    uint8_t cnt = 0;
    uint32_t number = 0;
    uint8_t uint = 0;
    if(RxMsg[14] == 0x0D && RxMsg[15] == 0x0A)
    {
        /* 解析返回压力(单位：N) */
        while(RxMsg[lable] != 0x20)
        {
            uint = (RxMsg[lable] - '0');
            number += uint * pow_user(10, cnt);
            cnt++;
            lable--;
        }
        
        int16_t diff = Dst->Last_Perssure - number;
//        if(diff < 1500)
            Dst->Perssure = number;
        
        /* 写入压力传感器工作状态 */
        if(RxMsg[0] == 'S' && RxMsg[1] == 'T')
        {
            Dst->Sensor_State = ST;
        }
        else if(RxMsg[0] == 'U' && RxMsg[1] == 'S')
        {
            Dst->Sensor_State = US;
        }
        else if(RxMsg[0] == 'O' && RxMsg[1] == 'L')
        {
            Dst->Sensor_State = OL;
        }
        else
        {
            Dst->Sensor_State = Sensor_State_Unknown;
        }
        
        /* 写入压力传感器重量信息 */
        if(RxMsg[3] == 'N' && RxMsg[4] == 'T')
        {
            Dst->Weight_Information = NT;
        }
        else if(RxMsg[3] == 'G' && RxMsg[4] == 'S')
        {
            Dst->Weight_Information = GS;
        }
        else 
        {
            Dst->Weight_Information = Weight_Information_Unknown;
        }
    }
}
