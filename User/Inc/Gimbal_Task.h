#ifndef __GIMBAL_TASK_H__
#define __GIMBAL_TASK_H__

#include "Task_Init.h"

// 98
#define Switch_Exp_Dis_X 30
// 182
#define Switch_Exp_Dis_Y 50

// 120
#define Vision_Exp_Dis_X 134
// 70
#define Vision_Exp_Dis_Y 75

typedef enum 
{
    Lock_A_Adjust = 0,
    Auto_Switch_Drain_Oil,
    Reset
} Gimbal_Working_Condition_e;
extern Gimbal_Working_Condition_e Gimbal_Working_Condition;

typedef struct{
    float Expect_Sport_X;
    float Expect_Sport_Y;
    float Expect_Sport_Z;
    float set_finish_lock;
    int8_t Stop;
    int8_t Control_Screwdriver_Run;
    int8_t Reset_Push_Draining;
    int8_t Reset_Push_Tank;
    int8_t Reset_Jack_Down;
    int8_t Reset_Jack_Up;
    int8_t Set_Jack_Wroking;
    int8_t Set_Push_Working;
} JoyGimbal_Control_Exp_;
extern JoyGimbal_Control_Exp_ JoyGimbal_Control_Exp;

typedef struct
{
    float Exp_Vision_X;
    float Exp_Vision_Y;
    float Exp_Vision_Z;
} Gimbal_Vision_Control_Exp_;
extern Gimbal_Vision_Control_Exp_ Gimbal_Vision_Control_Exp;

extern float Pluse_CNT;

extern uint8_t exp_dir_x;
extern uint8_t exp_dir_y;
extern uint8_t exp_dir_z;
extern uint8_t Vision_Finish_Base_Time_Flag;

//extern uint8_t control_of_vision;
//extern uint8_t control_of_joy;
//extern uint8_t control_of_remote;

typedef enum
{
    None_Limit = 0,
    Forward_Limit,
    Backward_Limit,
} Gimbal_Sport_Limit_e;

typedef enum
{
    None_Sport = 0,
    Control_Sport_Remote,
    Control_Stop_Remote,
    Control_Sport_PC,
    Control_Stop_PC,
    Control_Sport_Joy,
    Control_Stop_Joy
}GimbalMotor_Sport_Flag_e;

typedef enum
{
    Stop = 0,
    Sport,
} GimbalMotor_State_e;

typedef struct
{
    uint8_t dir;
    uint8_t exp_dir;
    float Pluse_CNT;
    float Move_Dis;     /* 单位 mm */
    TIM_HandleTypeDef *htim;
    uint32_t Channel;
    GPIO_TypeDef* Dir_Gpio;
    uint16_t GPIO_Pin;
    GimbalMotor_State_e Motor_State;
    GimbalMotor_Sport_Flag_e GimbalMotor_Sport_Flag;
    uint8_t Vision_Finish_Task_Flag;
    uint8_t Joy_Control_Flag;
    Gimbal_Sport_Limit_e Gimbal_Sport_Limit;
    uint8_t struct_class;
    uint8_t All_Finish_Vision_Task_Flag;
    float Finish_Vision_Point;
    uint8_t Get_Finish_Point_Flag;
    uint8_t Vision_Switch_Finish;
    uint8_t All_Vision_Switch_Finish;
    uint8_t Vision_Control_Flag;
} Gimbal_Motor_Typedef;

typedef struct
{
    float Gimbal_Z_Up_Start_Pose;   //!<@brief Z轴开始拧油口的位置
    float Gimbal_Z_Down_End_Pose;   //!<@brief Z轴拧完油口的位置
    float D_Gimbal_Z_Move_Dis;      //!<@brief 拧完油口时Z轴走过的距离
    float Gimbal_X_Start_Pose;      //!<@brief 开始切换油口与放油针时X轴位置
    float Gimbal_X_End_Pose;        //!<@brief 切换完毕油口与放油针时X轴位置
    float Gimbal_Y_Start_Pose;      //!<@brief 开始切换油口与放油针时Y轴位置
    float Gimbal_Y_End_Pose;        //!<@brief 切换完毕油口与放油针时Y轴位置
} Gimbal_Position_Info_s;
extern Gimbal_Position_Info_s Gimbal_Position_Info;

extern Gimbal_Motor_Typedef Gimbal_Motor_X;
extern Gimbal_Motor_Typedef Gimbal_Motor_Y;
extern Gimbal_Motor_Typedef Gimbal_Motor_Z;

extern TaskHandle_t Gimbal_Control_Task_Handle;
void Gimbal_Control_Task(void *pvParameters);

extern TaskHandle_t Pluse_Count_Task;
void Pluse_Count(void *pvParameters);

void Set_GimbalMotor_Dir(Gimbal_Motor_Typedef *motor, uint8_t dir);
void GimbalMotor_StartSport(Gimbal_Motor_Typedef *motor);
void GimbalMotor_StopSport(Gimbal_Motor_Typedef *motor);

void GimbalMotor_Init(void);

#endif 
