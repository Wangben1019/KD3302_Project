#ifndef __XIAOMI_GYBERGEAR_MOTOR_H__
#define __XIAOMI_GYBERGEAR_MOTOR_H__

#include "Task_Init.h"

#define PI 3.1415926

#define Get_Motor_ID 0
#define Motor_Control 1
#define Motor_Feedback 2
#define Motor_Enable 3
#define Motor_Disable 4
#define Set_MechanicalZero_Pos 6
#define Set_Motor_ID 7
#define Get_Single_Parameter 17
#define Set_Single_Parameter 18
#define Error_Feedback 21

typedef union {
    uint8_t data[8];
    uint64_t Mcu_Flag;
} CyberGear_Mcu_Flag_Typedef;

typedef struct {
    uint8_t Motor_CAN_ID;
    uint8_t Master_CAN_ID;
} CyberGear_Can_Id_Typedef;

//typedef enum {
//    Feedback_Error,
//    Feedback_SUCCESS
//} Feedback_State_Typedef;

//typedef enum {
//    CyberGear_Reset = 0,
//    CyberGear_Cali,
//    CyberGear_Motor
//} CyberGear_Mode_Typedef;

typedef struct {
    uint8_t CyberGear_Mode: 2;
    uint8_t Uncalibrated_Info: 1;
    uint8_t HALL_Encode_Fault_Info: 1;
    uint8_t Magnetic_Encode_Fault_Info: 1;
    uint8_t Over_Temperature_Info: 1;
    uint8_t Over_Current_Info: 1;
    uint8_t Under_Voltage_Fault_Info: 1;
} __CyberGear_State_Typedef;

typedef union {
    uint8_t state;
    __CyberGear_State_Typedef _CyberGear_State_Typedef;
} CyberGear_State_Typedef;

typedef struct {
    CyberGear_Mcu_Flag_Typedef CyberGear_Mcu_Flag;
    CyberGear_Can_Id_Typedef CyberGear_Can_Id;
    CyberGear_State_Typedef CyberGear_State;
    
    float Real_Angle;
    uint16_t Angle;
    
    float Real_AngleVelocity;
    uint16_t AngleVelocity;
    
    float Real_Torque;
    uint16_t Torque;
    
    float Real_Temperature;
    uint16_t Temperature;
} XiaoMi_GyberGear_Motor_Typedef;

/**
 * @brief CyberGear电机反馈接收
 * @param hcan CAN句柄
 * @param[in] ExtId 目标电机CAN _ID 
 * @param[in] data CAN接收数据帧
 * @param[in] Dst CyberGear电机结构体
 */
void XiaoMi_CyberGear_Motor_Receive(CAN_HandleTypeDef *hcan, uint32_t ExtId, uint8_t *data, XiaoMi_GyberGear_Motor_Typedef *Dst);

/**
* @brief CyberGear电机控制
 * @param hcan CAN句柄
 * @param[in] motor_id 目标电机CAN _ID 
 * @param[in] motor_torque 力矩(0~65535) 对应(-12Nm~12Nm)
 * @param[in] exp_angle 目标角度[0~65535] 对应(-4π~4π)
 * @param[in] exp_anglevelocity  目标角速度[0~65535] 对应(-30rad/s~30rad/s) 
 * @param[in] Kp [0~65535] 对应(0.0~500.0)
 * @param[in] Kd [0~65535]对应(0.0~5.0) 
 */
void CyberGear_Motor_Control(CAN_HandleTypeDef *hcan, uint8_t motor_id, uint16_t motor_torque, uint16_t exp_angle, uint16_t exp_anglevelocity, uint16_t Kp, uint16_t Kd);

/**
 * @brief 获取CyberGear电机ID
 * @param hcan CAN句柄
 * @param[in] motor_id 目标电机CAN _ID 
 * @param[in] master_id 用来标识主CAN_ID
 */
void Get_Device_ID(CAN_HandleTypeDef *hcan, uint8_t motor_id, uint8_t master_id);

/**
 * @brief 使能CyberGear电机
 * @param hcan CAN句柄
 * @param[in] motor_id 目标电机CAN _ID 
 * @param[in] master_id 用来标识主CAN_ID
 */
void CyberGear_Motor_Enable(CAN_HandleTypeDef *hcan, uint8_t motor_id, uint8_t master_id);

/**
 * @brief 停止CyberGear电机
 * @param hcan CAN句柄
 * @param[in] motor_id 目标电机CAN _ID 
 * @param[in] master_id 用来标识主CAN_ID
 */
void CyberGear_Motor_Disable(CAN_HandleTypeDef *hcan, uint8_t motor_id, uint8_t master_id);

#endif 
