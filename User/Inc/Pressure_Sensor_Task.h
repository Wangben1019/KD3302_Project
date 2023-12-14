#ifndef __PRESSURE_SENSOR_TASK_H__
#define __PRESSURE_SENSOR_TASK_H__

#include "Task_Init.h"

#define Sensor_DataLong 30

#define Jack_Down_Pressure_Thre1 10
#define Jack_Up_Pressure_Thre1   10
#define Jack_All_Pressure_Thre 300

#define Jack_Remote_Return_Time 16717
// 12730
// 12190
#define Jack_Up_Run_Time_Thre 14230   
// 9940
// 11010
#define Jack_Down_Run_Time_Thre 12440

#define Screwdriver_Work_Pressure_Min 100

#define Unscrew_Dis 40

#define Push_Drain_Pushing_Delay 9090
#define Draining_Oil_Delay 10000
#define Push_Tank_Delay 9090
#define Return_Draining_Pushing_Delay 9090
#define Return_Tank_Delay 9090

extern uint8_t Pressure_Sensor_Data_No1[Sensor_DataLong];
extern uint8_t Pressure_Sensor_Data_No2[Sensor_DataLong];
extern uint8_t Pressure_Sensor_Data_No3[Sensor_DataLong];

extern uint8_t draining_need_return;
extern uint8_t tank_need_return;

extern uint8_t jack_up_need_return;
extern uint8_t jack_down_need_return;

extern uint8_t get_xy_start_flag;
extern uint8_t get_x_end_flag;
extern uint8_t get_y_end_flag;

typedef enum
{
    Execute_Command = 0,        //!<@brief 执行PC指令，至找到油口
    Jack_Down_Run = 1,          //!<@brief 先下方千斤顶顶起，至阈值
    Jack_Up_Run = 2,            //!<@brief 而后上方千斤顶顶起，至阈值
    Jack_All_Run = 3,           //!<@brief 最后两个千斤顶同时顶起
    Jack_Finish = 4,
    
    Switch_Drain_Oil = 5,       //!<@brief 移动放油针至电动螺丝刀处
    Pushing_electric_push = 6,  //!<@brief 推动电推杆到指定位置
    Draining_Oil = 7,           //!<@brief 放油中，等待
    
    Push_Out_Tank = 8,           //!<@brief 推动电推杆，顶出外油箱
    Push_Return_Tank = 9,
    Finish_A_Reset = 10,
    Person_Control = 11
} System_Run_State_e;
extern System_Run_State_e System_Run_State;
extern System_Run_State_e Last_System_Run_State;

typedef enum
{
    None_ = 0,
    Doing_Return = 1,
    Finish_Return
} System_Reset_State_e;

typedef struct
{
    System_Reset_State_e Jack_Up_Reset_State;
    System_Reset_State_e Jack_Down_Reset_State;
}System_Reset_State_s;

typedef enum
{
    Sensor_State_Unknown = 0,   //!<@brief 未知状态
    ST,                         //!<@brief 重量稳定
    US,                         //!<@brief 重量不稳定
    OL,                         //!<@brief 超载
    Sensor_Error
} Sensor_State_e;

typedef enum
{
    Weight_Information_Unknown = 0,     //!<@brief 未知状态
    NT,                                 //!<@brief 净重
    GS                                  //!<@brief 毛重
} Weight_Information_e;

typedef struct
{
    Sensor_State_e Sensor_State;
    Weight_Information_e Weight_Information;
    int32_t Perssure;
    int32_t Last_Perssure;
} Pressure_Sensor_s;

extern Pressure_Sensor_s Pressure_Sensor_Screwdriver;
extern Pressure_Sensor_s Pressure_Sensor_Jack_Down;
extern Pressure_Sensor_s Pressure_Sensor_Jack_Up;

extern TaskHandle_t Master_Controller_Task_Handle;
void Master_Controller_Task(void *pvParameters);

void Pressure_Data_Decode(uint8_t *RxMsg, Pressure_Sensor_s *Dst);

#endif 
