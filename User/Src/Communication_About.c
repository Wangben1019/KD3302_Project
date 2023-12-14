#include "Communication_About.h"
#include "Chassis_Task.h"
#include "remote.h"

static void CANRxDecode(CAN_RxHeaderTypeDef rx_header,uint8_t rx_data[8]);

Transmit_Can_Info_u Transmit_Can_Info; 

#if Debug_Remote

uint8_t Remote_RxArr[Remote_Leng];

SemaphoreHandle_t Remote_DMA_Finish_Semaphore;
TaskHandle_t Remote_Task_Handle;
void Remote_Task(void *pvParameters)
{
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for(;;)
    {
        xSemaphoreTake(Remote_DMA_Finish_Semaphore, portMAX_DELAY);
        Remote_Rx(Remote_RxArr);
        vTaskDelayUntil(&xLastWakeTime,2);
    }
}

void RemoteControlProcess(Remote *rc) 
{
    Robot_Chassis_Exp.Expect_Speed_Yaw = -(rc->ch2 - 1024) / 100;
    Robot_Chassis_Exp.Expect_Speed_X = (rc->ch1 - 1024) / 100;
    Robot_Chassis_Exp.Expect_Speed_Y = -(rc->ch0 - 1024) / 100;
    
//    if(rc->ch0 > 1354)
//    {
//        Robot_Chassis_Exp.Expect_Speed_Yaw = -4;
//    }
//    else if(rc->ch0 < 694)
//    {
//        Robot_Chassis_Exp.Expect_Speed_Yaw = 4;
//    }
//    else
//    {
//        Robot_Chassis_Exp.Expect_Speed_Yaw = 0;
//    }
//    
//    if(rc->ch1 > 1354)
//    {
//        Robot_Chassis_Exp.Expect_Speed_X = 4;
//    }
//    else if(rc->ch1 < 694)
//    {
//        Robot_Chassis_Exp.Expect_Speed_X = -4;
//    }
//    else
//    {
//        Robot_Chassis_Exp.Expect_Speed_X = 0;
//    }
    
    /* X轴 */
//    if(rc->ch2 > 1354)
//    {
//        Gimbal_Motor_X.GimbalMotor_Sport_Flag = Control_Sport_Remote;
//        Gimbal_Motor_X.exp_dir = 0;
////        control_of_remote = 1;
//    }
//    else if(rc->ch2 < 694)
//    {
//        Gimbal_Motor_X.GimbalMotor_Sport_Flag = Control_Sport_Remote;
//        Gimbal_Motor_X.exp_dir = 1;
////        control_of_remote = 1;
//    }
//    else
//    {
//        Gimbal_Motor_X.GimbalMotor_Sport_Flag = Control_Stop_Remote;
////        control_of_remote = 1;
//    }
//    
//    if(rc->ch3 > 1354)
//    {
//        Gimbal_Motor_Y.GimbalMotor_Sport_Flag = Control_Sport_Remote;
//        Gimbal_Motor_Y.exp_dir = 0;
////        control_of_remote = 1;
//    }
//    else if(rc->ch3 < 694)
//    {
//        Gimbal_Motor_Y.GimbalMotor_Sport_Flag = Control_Sport_Remote;
//        Gimbal_Motor_Y.exp_dir = 1;
////        control_of_remote = 1;
//    }
//    else
//    {
//        Gimbal_Motor_Y.GimbalMotor_Sport_Flag = Control_Stop_Remote;
////        control_of_remote = 1;
//    }
//    
//    if(rc->s1 == 2) // Z轴向上
//    {
//        Gimbal_Motor_Z.GimbalMotor_Sport_Flag = Control_Sport_Remote;
//        Gimbal_Motor_Z.exp_dir = 0;
////        control_of_remote = 1;
//    }
//    else if(rc->s1 == 1) // Z轴向下
//    {
//        Gimbal_Motor_Z.GimbalMotor_Sport_Flag = Control_Sport_Remote;
//        Gimbal_Motor_Z.exp_dir = 1;
////        control_of_remote = 1;
//    }
//    else if(rc->s1 == 3)
//    {
//        Gimbal_Motor_Z.GimbalMotor_Sport_Flag = Control_Stop_Remote;
////        control_of_remote = 1;
//    }
}

#endif

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);//获取CAN数据
    CANRxDecode(rx_header,rx_data);//CAN数据解码
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &rx_header, rx_data);//获取CAN数据
    CANRxDecode(rx_header,rx_data);//CAN数据解码
}

static void CANRxDecode(CAN_RxHeaderTypeDef rx_header,uint8_t rx_data[8])
{
    if (rx_header.IDE == CAN_ID_EXT) {//小米电机解码
        RxCAN_info_s RxCAN_info;//用于存储小米电机反馈的数据
        memcpy(&RxCAN_info,&rx_header.ExtId,4);//将扩展标识符的内容解码成对应内容

//        uint16_t decode_temp_mi;//小米电机反馈数据解码缓冲
        if(RxCAN_info.communication_type == 0){//通信类型0的反馈帧解码
            RxCAN_info_type_0_s RxCAN_info_type_0;
            memcpy(&RxCAN_info_type_0,&rx_header.ExtId,4);//将扩展标识符的内容解码成通信类型0的对应内容
            memcpy(&RxCAN_info_type_0.MCU_id,rx_data,8);//获取MCU标识符
        }else if(RxCAN_info.communication_type == 2){//通信类型2的反馈帧解码
            RxCAN_info_type_2_s RxCAN_info_type_2;
            memcpy(&RxCAN_info_type_2,&rx_header.ExtId,4);//将扩展标识符的内容解码成通信类型2的对应内容
            MI_motor_RxDecode(&RxCAN_info_type_2,rx_data);//通信类型2的数据解码

//            MI_Motor[RxCAN_info_type_2.motor_id].RxCAN_info = RxCAN_info_type_2;
//            MI_Motor[RxCAN_info_type_2.motor_id].motor_mode_state = RxCAN_info_type_2.mode_state;
            MI_Chassis_Motor[RxCAN_info_type_2.motor_id - 1].RxCAN_info = RxCAN_info_type_2;
            MI_Chassis_Motor[RxCAN_info_type_2.motor_id - 1].motor_mode_state = RxCAN_info_type_2.mode_state;

        }else if(RxCAN_info.communication_type == 17){//通信类型17的反馈帧解码
            RxCAN_info_type_17_s RxCAN_info_type_17;
            memcpy(&RxCAN_info_type_17,&rx_header.ExtId,4);//将扩展标识符的内容解码成通信类型17的对应内容
            memcpy(&RxCAN_info_type_17.index,&rx_data[0],2);//获取查找的参数索引码
            memcpy(&RxCAN_info_type_17.param,&rx_data[4],4);//获取查找的参数信息
        }


    }
}

void VCOMM_CallBack(uint8_t fun_code, uint16_t id, uint8_t* data, uint8_t len) {
//	printf("fun_code=%02X, id=%04x, len=%d\r\n", fun_code, id, len);
    if(fun_code == 0x01 && id == 0x01 && len == sizeof(Robot_Chassis_Exp_))
    {
        memcpy(&Robot_Chassis_Exp, data, sizeof(Robot_Chassis_Exp_));
        Navigation_Interrupt_cnt = 0;
    }
    else if(fun_code == 0x02 && id == 0x02 && len == sizeof(JoyGimbal_Control_Exp_))
    {
        /* y左划为正 */
        memcpy(&JoyGimbal_Control_Exp, data, sizeof(JoyGimbal_Control_Exp_));
//        if(JoyGimbal_Control_Exp.Expect_Sport_X != 0 || JoyGimbal_Control_Exp.Expect_Sport_Y != 0)
//        {
//            control_of_joy = 1;
//        }
//        Joy_Interrupt_cnt = 0;
        
    }
    else if(fun_code == 0x03 && id == 0x03 && len == sizeof(Gimbal_Vision_Control_Exp_))
    {
        memcpy(&Gimbal_Vision_Control_Exp, data, sizeof(Gimbal_Vision_Control_Exp_));
        
        Vision_Interrupt_cnt = 0;
        
    }
    else if(fun_code == 0x04 && id == 0x04 && len == sizeof(Vision_goal_expect_s))
    {
        memcpy(&Vision_goal_expect, data, sizeof(Vision_goal_expect_s));
        Goal_Interrupt_cnt = 0;
    }
}
