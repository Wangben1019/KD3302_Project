#include "Gimbal_Task.h"
#include "math.h"

Gimbal_Working_Condition_e Gimbal_Working_Condition = Lock_A_Adjust;

JoyGimbal_Control_Exp_ JoyGimbal_Control_Exp;

Gimbal_Vision_Control_Exp_ Gimbal_Vision_Control_Exp;

Gimbal_Position_Info_s Gimbal_Position_Info;

float Pluse_CNT;

float move_x;
float move_y;
float move_z;

Gimbal_Motor_Typedef Gimbal_Motor_X;
Gimbal_Motor_Typedef Gimbal_Motor_Y;
Gimbal_Motor_Typedef Gimbal_Motor_Z;

uint8_t debug_flag = 0;
uint8_t dir_flag = 0;

uint8_t exp_dir_x = 0;
uint8_t exp_dir_y = 0;
uint8_t exp_dir_z = 0;

uint8_t Vision_Finish_Base_Time_Flag;

uint8_t gimbal_x_switch_finish = 0;
uint8_t gimbal_y_switch_finish = 0;

//uint8_t control_of_vision;
//uint8_t control_of_joy;
//uint8_t control_of_remote;

float Count_Pluse_Once(TIM_HandleTypeDef *htim, uint16_t xms, uint8_t dir, uint16_t mhz);
void Set_GimbalMotor_Speed(TIM_HandleTypeDef *htim, uint16_t exp);
void Set_GimbalMotor_Dir(Gimbal_Motor_Typedef *motor, uint8_t dir);
void GimbalMotor_Data_Update(Gimbal_Motor_Typedef *motor, uint16_t update_f);
void GimbalMotor_Control(Gimbal_Motor_Typedef *motor, float exp_point, float exp_joy);

TaskHandle_t Gimbal_Control_Task_Handle;
void Gimbal_Control_Task(void *pvParameters)
{
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    static uint16_t delay_ms = 2;
    for(;;)
    {
        move_x = Gimbal_Motor_X.Move_Dis;
        move_y = Gimbal_Motor_Y.Move_Dis;
        move_z = Gimbal_Motor_Z.Move_Dis;

//        GimbalMotor_StopSport(&Gimbal_Motor_Z);
//        
        if(JoyGimbal_Control_Exp.Stop == 1)
        {
            GimbalMotor_StopSport(&Gimbal_Motor_X);
            GimbalMotor_StopSport(&Gimbal_Motor_Y);
            GimbalMotor_StopSport(&Gimbal_Motor_Z);
        }
        else if((JoyGimbal_Control_Exp.Control_Screwdriver_Run == 1 && System_Run_State == Jack_Finish))
        {
            GimbalMotor_StopSport(&Gimbal_Motor_X);
            GimbalMotor_StopSport(&Gimbal_Motor_Y);
        }
        else
        {
            if(Gimbal_Working_Condition == Lock_A_Adjust)
            {
                if(Gimbal_Motor_X.Vision_Finish_Task_Flag == 0 || Gimbal_Motor_Y.Vision_Finish_Task_Flag == 0)
                {
                    Gimbal_Motor_X.All_Finish_Vision_Task_Flag = 0;
                    Gimbal_Motor_Y.All_Finish_Vision_Task_Flag = 0;
                    if(Gimbal_Motor_X.Gimbal_Sport_Limit == None_Limit)
                    {
                        GimbalMotor_Control(&Gimbal_Motor_X, Gimbal_Vision_Control_Exp.Exp_Vision_X, -JoyGimbal_Control_Exp.Expect_Sport_Y);
                    }
                    else if(Gimbal_Motor_X.Gimbal_Sport_Limit == Forward_Limit)
                    {
                        if(JoyGimbal_Control_Exp.Expect_Sport_Y <= -0.4f || ((Gimbal_Vision_Control_Exp.Exp_Vision_X - 320) > 10)) // 1
                        {
                            GimbalMotor_Control(&Gimbal_Motor_X, Gimbal_Vision_Control_Exp.Exp_Vision_X, -JoyGimbal_Control_Exp.Expect_Sport_Y);
                        }
                        else if(JoyGimbal_Control_Exp.Expect_Sport_Y >= 0.4f || ((Gimbal_Vision_Control_Exp.Exp_Vision_X - 320) < -10))  // 0
                        {
                            GimbalMotor_StopSport(&Gimbal_Motor_X);
                        }
                    }
                    else if(Gimbal_Motor_X.Gimbal_Sport_Limit == Backward_Limit)
                    {
                        if(JoyGimbal_Control_Exp.Expect_Sport_Y >= 0.4f || ((Gimbal_Vision_Control_Exp.Exp_Vision_X - 320) < -10)) // 0
                        {
                            GimbalMotor_Control(&Gimbal_Motor_X, Gimbal_Vision_Control_Exp.Exp_Vision_X, -JoyGimbal_Control_Exp.Expect_Sport_Y);
                        }
                        else if(JoyGimbal_Control_Exp.Expect_Sport_Y <= -0.4f || ((Gimbal_Vision_Control_Exp.Exp_Vision_X - 320) > 10)) // 1
                        {
                            GimbalMotor_StopSport(&Gimbal_Motor_X);
                        }
                    }
                    
                    if(Gimbal_Motor_Y.Gimbal_Sport_Limit == None_Limit)
                    {
                        GimbalMotor_Control(&Gimbal_Motor_Y, Gimbal_Vision_Control_Exp.Exp_Vision_Y, JoyGimbal_Control_Exp.Expect_Sport_X);
                    }
                    else if(Gimbal_Motor_Y.Gimbal_Sport_Limit == Backward_Limit)
                    {
                        if(JoyGimbal_Control_Exp.Expect_Sport_X >= 0.4f || ((Gimbal_Vision_Control_Exp.Exp_Vision_Y - 240) < -10)) // 0
                        {
                            GimbalMotor_Control(&Gimbal_Motor_Y, Gimbal_Vision_Control_Exp.Exp_Vision_Y, JoyGimbal_Control_Exp.Expect_Sport_X);
                        }
                        else if(JoyGimbal_Control_Exp.Expect_Sport_X <= -0.4f || ((Gimbal_Vision_Control_Exp.Exp_Vision_Y - 240) > 10)) // 1
                        {
                            GimbalMotor_StopSport(&Gimbal_Motor_Y);
                        }
                        
                    }
                    else if(Gimbal_Motor_Y.Gimbal_Sport_Limit == Forward_Limit)
                    {
                        if(JoyGimbal_Control_Exp.Expect_Sport_X <= -0.4f || ((Gimbal_Vision_Control_Exp.Exp_Vision_Y - 240) > 10)) // 1
                        {
                            GimbalMotor_Control(&Gimbal_Motor_Y, Gimbal_Vision_Control_Exp.Exp_Vision_Y, JoyGimbal_Control_Exp.Expect_Sport_X);
                        }
                        else if(JoyGimbal_Control_Exp.Expect_Sport_X >= 0.4f || ((Gimbal_Vision_Control_Exp.Exp_Vision_Y - 240) < -10)) // 0
                        {
                            GimbalMotor_StopSport(&Gimbal_Motor_Y);
                        }
                    }
                    
                    if(Gimbal_Motor_Z.Gimbal_Sport_Limit == None_Limit)
    //                if(1)
                    {
                        GimbalMotor_Control(&Gimbal_Motor_Z, Gimbal_Vision_Control_Exp.Exp_Vision_Z, JoyGimbal_Control_Exp.Expect_Sport_Z);
                    }
                    else if(Gimbal_Motor_Z.Gimbal_Sport_Limit == Forward_Limit)
    //                else if(0)
                    {
                        if(JoyGimbal_Control_Exp.Expect_Sport_Z <= -0.4f)
                        {
                            GimbalMotor_Control(&Gimbal_Motor_Z, Gimbal_Vision_Control_Exp.Exp_Vision_Z, JoyGimbal_Control_Exp.Expect_Sport_Z);
                        }
                        else if(JoyGimbal_Control_Exp.Expect_Sport_Z >= 0.4f)
                        {
                           GimbalMotor_StopSport(&Gimbal_Motor_Z); 
                        }
                        
                    }
                    else if(Gimbal_Motor_Z.Gimbal_Sport_Limit == Backward_Limit)
    //                else if(0)
                    {
                        if(JoyGimbal_Control_Exp.Expect_Sport_Z >= 0.4f)
                        {
                            GimbalMotor_Control(&Gimbal_Motor_Z, Gimbal_Vision_Control_Exp.Exp_Vision_Z, JoyGimbal_Control_Exp.Expect_Sport_Z);
                        }
                        else if(JoyGimbal_Control_Exp.Expect_Sport_Z <= -0.4f)
                        {
                           GimbalMotor_StopSport(&Gimbal_Motor_Z); 
                        }
                    }                
                }
                else
                {
                    Gimbal_Motor_X.All_Finish_Vision_Task_Flag = 1;
                    Gimbal_Motor_Y.All_Finish_Vision_Task_Flag = 1;
                    
                    if(Gimbal_Motor_X.Vision_Switch_Finish == 1 && Gimbal_Motor_Y.Vision_Switch_Finish == 1)
                    {
                        Gimbal_Motor_X.All_Vision_Switch_Finish = 1;
                        Gimbal_Motor_Y.All_Vision_Switch_Finish = 1;
                    }   
                    
                    if(Gimbal_Motor_X.Gimbal_Sport_Limit == None_Limit)
                    {
                        GimbalMotor_Control(&Gimbal_Motor_X, 320, -JoyGimbal_Control_Exp.Expect_Sport_Y);
                    }
                    else if(Gimbal_Motor_X.Gimbal_Sport_Limit == Forward_Limit)
                    {
                        if(JoyGimbal_Control_Exp.Expect_Sport_Y <= -0.4f)
                        {
                            GimbalMotor_Control(&Gimbal_Motor_X, 320, -JoyGimbal_Control_Exp.Expect_Sport_Y);
                        }
                        else if(JoyGimbal_Control_Exp.Expect_Sport_Y >= 0.4f)
                        {
                            GimbalMotor_StopSport(&Gimbal_Motor_X);
                        }
                    }
                    else if(Gimbal_Motor_X.Gimbal_Sport_Limit == Backward_Limit)
                    {
                        if(JoyGimbal_Control_Exp.Expect_Sport_Y >= 0.4f)
                        {
                            GimbalMotor_Control(&Gimbal_Motor_X, 320, -JoyGimbal_Control_Exp.Expect_Sport_Y);
                        }
                        else if(JoyGimbal_Control_Exp.Expect_Sport_Y <= -0.4f)
                        {
                            GimbalMotor_StopSport(&Gimbal_Motor_X);
                        }
                    }
                    
                    if(Gimbal_Motor_Y.Gimbal_Sport_Limit == None_Limit)
                    {
                        GimbalMotor_Control(&Gimbal_Motor_Y, 240, JoyGimbal_Control_Exp.Expect_Sport_X);
                    }
                    else if(Gimbal_Motor_Y.Gimbal_Sport_Limit == Backward_Limit)
                    {
                        if(JoyGimbal_Control_Exp.Expect_Sport_X >= 0.4f)
                        {
                            GimbalMotor_Control(&Gimbal_Motor_Y, 240, JoyGimbal_Control_Exp.Expect_Sport_X);
                        }
                        else if(JoyGimbal_Control_Exp.Expect_Sport_X <= -0.4f)
                        {
                            GimbalMotor_StopSport(&Gimbal_Motor_Y);
                        }
                        
                    }
                    else if(Gimbal_Motor_Y.Gimbal_Sport_Limit == Forward_Limit)
                    {
                        if(JoyGimbal_Control_Exp.Expect_Sport_X <= -0.4f)
                        {
                            GimbalMotor_Control(&Gimbal_Motor_Y, 240, JoyGimbal_Control_Exp.Expect_Sport_X);
                        }
                        else if(JoyGimbal_Control_Exp.Expect_Sport_X >= 0.4f)
                        {
                            GimbalMotor_StopSport(&Gimbal_Motor_Y);
                        }
                    }
                    
                    if(Gimbal_Motor_Z.Gimbal_Sport_Limit == None_Limit)
    //                if(1)
                    {
                        GimbalMotor_Control(&Gimbal_Motor_Z, Gimbal_Vision_Control_Exp.Exp_Vision_Z, JoyGimbal_Control_Exp.Expect_Sport_Z);
                    }
                    else if(Gimbal_Motor_Z.Gimbal_Sport_Limit == Forward_Limit)
    //                else if(0)
                    {
                        if(JoyGimbal_Control_Exp.Expect_Sport_Z <= -0.4f)
                        {
                            GimbalMotor_Control(&Gimbal_Motor_Z, Gimbal_Vision_Control_Exp.Exp_Vision_Z, JoyGimbal_Control_Exp.Expect_Sport_Z);
                        }
                        else if(JoyGimbal_Control_Exp.Expect_Sport_Z >= 0.4f)
                        {
                           GimbalMotor_StopSport(&Gimbal_Motor_Z); 
                        }
                        
                    }
                    else if(Gimbal_Motor_Z.Gimbal_Sport_Limit == Backward_Limit)
    //                else if(0)
                    {
                        if(JoyGimbal_Control_Exp.Expect_Sport_Z >= 0.4f)
                        {
                            GimbalMotor_Control(&Gimbal_Motor_Z, Gimbal_Vision_Control_Exp.Exp_Vision_Z, JoyGimbal_Control_Exp.Expect_Sport_Z);
                        }
                        else if(JoyGimbal_Control_Exp.Expect_Sport_Z <= -0.4f)
                        {
                           GimbalMotor_StopSport(&Gimbal_Motor_Z); 
                        }
                    }
                }

            }
            else if(Gimbal_Working_Condition == Auto_Switch_Drain_Oil && System_Run_State == Switch_Drain_Oil)
            {
                if(__fabs(Gimbal_Motor_X.Move_Dis - Gimbal_Position_Info.Gimbal_X_Start_Pose) < Switch_Exp_Dis_X)
                {
                    Set_GimbalMotor_Dir(&Gimbal_Motor_X, 0);
                    GimbalMotor_StartSport(&Gimbal_Motor_X);
                    if(Gimbal_Motor_X.Gimbal_Sport_Limit == Forward_Limit)
                    {
                        GimbalMotor_StopSport(&Gimbal_Motor_X);
                    }
                }
                else
                {
                    if(get_x_end_flag == 0)
                    {
                        Gimbal_Position_Info.Gimbal_X_End_Pose = Gimbal_Motor_X.Move_Dis;
                        get_x_end_flag = 1;
                    }
                    GimbalMotor_StopSport(&Gimbal_Motor_X);
                    gimbal_x_switch_finish = 1;
//                    System_Run_State = 
                }
                
                if(__fabs(Gimbal_Motor_Y.Move_Dis - Gimbal_Position_Info.Gimbal_Y_Start_Pose) < Switch_Exp_Dis_Y)
                {
                    Set_GimbalMotor_Dir(&Gimbal_Motor_Y, 0);
                    GimbalMotor_StartSport(&Gimbal_Motor_Y);
                    if(Gimbal_Motor_Y.Gimbal_Sport_Limit == Forward_Limit)
                    {
                        GimbalMotor_StopSport(&Gimbal_Motor_Y);
                    }
                }
                else
                {
                    if(get_y_end_flag == 0)
                    {
                        Gimbal_Position_Info.Gimbal_Y_End_Pose = Gimbal_Motor_Y.Move_Dis;
                        get_y_end_flag = 1;
                    }
                    GimbalMotor_StopSport(&Gimbal_Motor_Y);
                    gimbal_y_switch_finish = 1;
                }
                
                if(gimbal_x_switch_finish == 1 && gimbal_y_switch_finish == 1)
                {
                    get_xy_start_flag = 0;
                    get_y_end_flag = 0;
                    get_x_end_flag = 0;
                    System_Run_State = Pushing_electric_push;
                    Last_System_Run_State = Switch_Drain_Oil;
                    gimbal_x_switch_finish = 0;
                    gimbal_y_switch_finish = 0;
                }
            }
            else if(Gimbal_Working_Condition == Reset)
            {
                
            }
        }
        
        GimbalMotor_Data_Update(&Gimbal_Motor_X, delay_ms);
        GimbalMotor_Data_Update(&Gimbal_Motor_Y, delay_ms);
        GimbalMotor_Data_Update(&Gimbal_Motor_Z, delay_ms);

        
        
        vTaskDelayUntil(&xLastWakeTime,delay_ms);
    }
}

/* 修改转向 */
void Set_GimbalMotor_Dir(Gimbal_Motor_Typedef *motor, uint8_t dir)
{
    if(dir == 0 || dir == 1)
    {
        HAL_GPIO_WritePin(motor->Dir_Gpio, motor->GPIO_Pin, (GPIO_PinState)dir);
    }
    
}

/* 使能电机运动 */
void GimbalMotor_StartSport(Gimbal_Motor_Typedef *motor)
{
    HAL_TIM_PWM_Start(motor->htim, motor->Channel);
    __HAL_TIM_SET_COMPARE(motor->htim, motor->Channel, (motor->htim->Instance->ARR / 2));
    motor->Motor_State = Sport;
}

/* 失能电机运动 */
void GimbalMotor_StopSport(Gimbal_Motor_Typedef *motor)
{
    HAL_TIM_PWM_Stop(motor->htim, motor->Channel);
    motor->Motor_State = Stop;
}

/* 计算脉冲数 */
float Count_Pluse_Once(TIM_HandleTypeDef *htim, uint16_t xms, uint8_t dir, uint16_t mhz)
{
    uint32_t temp_psc = ((htim->Instance->PSC) + 1);
    uint32_t temp_arr = ((__HAL_TIM_GetAutoreload(htim)) + 1);
    uint32_t temp_hz = mhz * 1000000;
    uint32_t temp_f = temp_hz / (temp_psc * temp_arr);
    return ((dir == 1) ? (-((float)(temp_f * xms * 0.001 / 2))) : ((float)(temp_f * xms * 0.001 / 2)));
}
    
/* speed:0-10 */
/* 设置电机转速 */
void Set_GimbalMotor_Speed(TIM_HandleTypeDef *htim, uint16_t exp)
{
    if(exp > 10)
    {
        exp = 10;
    }
    if(exp == 0)
    {
        HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
        HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
        HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
    }
    uint32_t arr_exp = 100 / exp;
    
    __HAL_TIM_SetAutoreload(htim, arr_exp);
}

/* 更新电机状态结构体 */
void GimbalMotor_Data_Update(Gimbal_Motor_Typedef *motor, uint16_t update_f)
{
    if(motor->Motor_State == Sport)
    {
        motor->dir = (((HAL_GPIO_ReadPin(motor->Dir_Gpio, motor->GPIO_Pin)) == GPIO_PIN_RESET) ? 0 : 1);
        motor->Pluse_CNT += Count_Pluse_Once(&htim4, update_f, motor->dir, 84);
        if(motor->struct_class == 1 || motor->struct_class == 2)
        {
            motor->Move_Dis = motor->Pluse_CNT / 10000 * 10;
        }
        else if(motor->struct_class == 3)
        {
            motor->Move_Dis = motor->Pluse_CNT / 30000 * 10;
        }
        
    }
    
}

/* 电机控制 */
void GimbalMotor_Control(Gimbal_Motor_Typedef *motor, float exp_point, float exp_joy)
{
    if(motor->All_Finish_Vision_Task_Flag == 1 && motor->All_Vision_Switch_Finish == 0)
    {
        if(motor->struct_class == 1)    // X
        {
            if(__fabs(motor->Finish_Vision_Point - motor->Move_Dis) <= Vision_Exp_Dis_X)
            {
                Set_GimbalMotor_Dir(motor, 1);
                GimbalMotor_StartSport(motor);
                if(motor->Gimbal_Sport_Limit == Forward_Limit)
                {
                    GimbalMotor_StopSport(motor);
                }
            }
            else
            {
                GimbalMotor_StopSport(motor);
                motor->Vision_Switch_Finish = 1;
            }
        }
        else if(motor->struct_class == 2)   // Y
        {
            if(__fabs(motor->Move_Dis - motor->Finish_Vision_Point) <= Vision_Exp_Dis_Y)
            {
                Set_GimbalMotor_Dir(motor, 1);
                GimbalMotor_StartSport(motor);
                if(motor->Gimbal_Sport_Limit == Backward_Limit)
                {
                    GimbalMotor_StopSport(motor);
                }
            }
            else
            {
                GimbalMotor_StopSport(motor);
                motor->Vision_Switch_Finish = 1;
            }
        }
    }
    else
    {
        if(exp_joy > 0.4f)
        {
            motor->GimbalMotor_Sport_Flag = Control_Sport_PC;
            motor->exp_dir = 0;
            Set_GimbalMotor_Dir(motor, motor->exp_dir);
            GimbalMotor_StartSport(motor);
            motor->Joy_Control_Flag = 1;
        }
        else if(exp_joy < -0.4f)
        {
            motor->GimbalMotor_Sport_Flag = Control_Sport_PC;
            motor->exp_dir = 1;
            Set_GimbalMotor_Dir(motor, motor->exp_dir);
            GimbalMotor_StartSport(motor);
            motor->Joy_Control_Flag = 1;
        }
        else
        {
            GimbalMotor_StopSport(motor);
            motor->Joy_Control_Flag = 0;
        }
    }
    
#if Debug_Remote
//    if(motor->GimbalMotor_Sport_Flag == Control_Sport_Remote)
//    {
//        Set_GimbalMotor_Dir(motor, motor->exp_dir);
//        GimbalMotor_StartSport(motor);
//    }
//    else if(motor->GimbalMotor_Sport_Flag == Control_Stop_Remote)
//    {
//        GimbalMotor_StopSport(motor);
//    }
    
#endif    
    
    /* 视觉锁定运动优先级 < 手柄控制运动优先级 */
    
    if(motor->struct_class == 1)    // X
    {
        if(exp_point - 320 >= 10)
        {
            motor->GimbalMotor_Sport_Flag = Control_Sport_PC;
            motor->exp_dir = 1;
            Set_GimbalMotor_Dir(motor, motor->exp_dir);
            GimbalMotor_StartSport(motor);
            motor->Vision_Finish_Task_Flag = 0;
            motor->Vision_Control_Flag = 1;
        }
        else if(exp_point - 320 <= -10)
        {
            motor->GimbalMotor_Sport_Flag = Control_Sport_PC;
            motor->exp_dir = 0;
            Set_GimbalMotor_Dir(motor, motor->exp_dir);
            GimbalMotor_StartSport(motor);
            motor->Vision_Finish_Task_Flag = 0;
            motor->Vision_Control_Flag = 1;
        }
        else
        {
            if(Vision_Finish_Base_Time_Flag == 1)
            {
                if(motor->Vision_Control_Flag == 1)
                {
                    if(motor->Vision_Finish_Task_Flag != 1)
                        GimbalMotor_StopSport(motor);
                    motor->Vision_Finish_Task_Flag = 1; /* 也可能是没识别到，没通信，懒得写，也懒得改，都差不多，反正刹车用的 */
                }
                if(motor->Get_Finish_Point_Flag == 0)
                {
                    motor->Finish_Vision_Point = motor->Move_Dis;
                    motor->Get_Finish_Point_Flag = 1;
                }    
            }
                         
            
        }
    }
    else if(motor->struct_class == 2) // Y
    {
        if(exp_point - 240 >= 10)
        {
            motor->GimbalMotor_Sport_Flag = Control_Sport_PC;
            motor->exp_dir = 1;
            Set_GimbalMotor_Dir(motor, motor->exp_dir);
            GimbalMotor_StartSport(motor);
            motor->Vision_Finish_Task_Flag = 0;
            motor->Vision_Control_Flag = 1;
        }
        else if(exp_point - 240 <= -10)
        {
            motor->GimbalMotor_Sport_Flag = Control_Sport_PC;
            motor->exp_dir = 0;
            Set_GimbalMotor_Dir(motor, motor->exp_dir);
            GimbalMotor_StartSport(motor);
            motor->Vision_Finish_Task_Flag = 0;
            motor->Vision_Control_Flag = 1;
        }
        else
        {
            if(Vision_Finish_Base_Time_Flag == 1)
            {
                if(motor->Vision_Control_Flag == 1)
                {   
                    if(motor->Vision_Finish_Task_Flag != 1)
                        GimbalMotor_StopSport(motor);
                    motor->Vision_Finish_Task_Flag = 1; /* 也可能是没识别到，没通信，懒得写，也懒得改，都差不多，反正刹车用的 */
                }
                if(motor->Get_Finish_Point_Flag == 0)
                {
                    motor->Finish_Vision_Point = motor->Move_Dis;
                    motor->Get_Finish_Point_Flag = 1;
                }  
            }
                           
            
        }
    }
    else if(motor->struct_class == 3)
    {
        motor->Vision_Finish_Task_Flag = 1;
//        if(exp_point >= 0.05f)
//        {
//            motor->GimbalMotor_Sport_Flag = Control_Sport_PC;
//            motor->exp_dir = 0;
//            Set_GimbalMotor_Dir(motor, motor->exp_dir);
//            GimbalMotor_StartSport(motor);
//            motor->Vision_Finish_Task_Flag = 0;
//        }
//        else if(exp_point <= -0.05f)
//        {
//            motor->GimbalMotor_Sport_Flag = Control_Sport_PC;
//            motor->exp_dir = 1;
//            Set_GimbalMotor_Dir(motor, motor->exp_dir);
//            GimbalMotor_StartSport(motor);
//            motor->Vision_Finish_Task_Flag = 0;
//        }
//        else
//        {
//            motor->Vision_Finish_Task_Flag = 1; /* 也可能是没识别到，没通信，懒得写，也懒得改，都差不多，反正刹车用的 */
//        }
    }
    
    
    
//    if(motor->Vision_Finish_Task_Flag == 1 && motor->Joy_Control_Flag == 0)
//    {
//        GimbalMotor_StopSport(motor);
//    }
    
//    if(motor->GimbalMotor_Sport_Flag == Control_Sport_Remote || motor->GimbalMotor_Sport_Flag == Control_Sport_PC || motor->GimbalMotor_Sport_Flag == Control_Sport_Joy)
//    {
//        Set_GimbalMotor_Dir(motor, motor->exp_dir);
//        GimbalMotor_StartSport(motor);
//    }
//    else if(motor->GimbalMotor_Sport_Flag == Control_Stop_Remote || motor->GimbalMotor_Sport_Flag == Control_Stop_PC || motor->GimbalMotor_Sport_Flag == Control_Stop_Joy || None_Sport)
//    {
//        GimbalMotor_StopSport(motor);
//    }
//    
//    control_of_vision = 0;
//    control_of_joy = 0;
//    control_of_remote = 0;
}

/* 电机初始化 */
void GimbalMotor_Init()
{
    Gimbal_Motor_X.Dir_Gpio = GPIOA;
    Gimbal_Motor_X.GPIO_Pin = GPIO_PIN_4;
    Gimbal_Motor_X.htim = &htim4;
    Gimbal_Motor_X.Channel = TIM_CHANNEL_1;
    Gimbal_Motor_X.Motor_State = Stop;
    Gimbal_Motor_X.struct_class = 1;
    
    Gimbal_Motor_Y.Dir_Gpio = GPIOA;
    Gimbal_Motor_Y.GPIO_Pin = GPIO_PIN_5;
    Gimbal_Motor_Y.htim = &htim4;
    Gimbal_Motor_Y.Channel = TIM_CHANNEL_2;
    Gimbal_Motor_Y.Motor_State = Stop;
    Gimbal_Motor_Y.struct_class = 2;
    
    Gimbal_Motor_Z.Dir_Gpio = GPIOA;
    Gimbal_Motor_Z.GPIO_Pin = GPIO_PIN_6;
    Gimbal_Motor_Z.htim = &htim4;
    Gimbal_Motor_Z.Channel = TIM_CHANNEL_3;
    Gimbal_Motor_Z.Motor_State = Stop;
    Gimbal_Motor_Z.struct_class = 3;
}

