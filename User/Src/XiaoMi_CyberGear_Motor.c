#include "XiaoMi_CyberGear_Motor.h"

void XiaoMi_CyberGear_Motor_Receive(CAN_HandleTypeDef *hcan, uint32_t ExtId, uint8_t *data, XiaoMi_GyberGear_Motor_Typedef *Dst)
{
    uint8_t communication_type = ExtId >> 24;
    if(communication_type == Get_Motor_ID)
    {
        Dst->CyberGear_Can_Id.Motor_CAN_ID = ExtId >> 8;
        for(int i = 0;i < 8;i++)
        {
            Dst->CyberGear_Mcu_Flag.data[i] = *(data + i);
        }
    }
    else if(communication_type == Motor_Feedback)
    {
        Dst->CyberGear_Can_Id.Motor_CAN_ID = ExtId >> 8;
        Dst->CyberGear_State.state = ExtId >> 16;
        Dst->Angle = data[0] << 8 | data[1];
        Dst->Real_Angle = (Dst->Angle - 32767) / 32768 * 4 * PI;
        Dst->AngleVelocity = data[2] << 8 | data[3];
        Dst->Real_AngleVelocity = (Dst->AngleVelocity - 32767) / 32768 * 30;
        Dst->Torque = data[4] << 8 | data[5];
        Dst->Real_Torque = (Dst->Torque - 32767) / 32768 * 12;
        Dst->Temperature = data[6] << 8 | data[7];
        Dst->Real_Temperature = Dst->Temperature / 10;
    }
}

void CyberGear_Motor_Control(CAN_HandleTypeDef *hcan, uint8_t motor_id, uint16_t motor_torque, uint16_t exp_angle, uint16_t exp_anglevelocity, uint16_t Kp, uint16_t Kd)
{
    uint32_t temp_id = 0;
    uint8_t data[8] = {0};
    temp_id = Motor_Control << 24 | motor_torque << 8 | motor_id;
    data[0] = exp_angle >> 8;
    data[1] = exp_angle;
    data[2] = exp_anglevelocity >> 8;
    data[3] = exp_anglevelocity;
    data[4] = Kp >> 8;
    data[5] = Kp;
    data[6] = Kd >> 8;
    data[7] = Kd;
    
    CAN_Send_StdDataFrame_Ext(hcan, temp_id, data);
}

void Get_Device_ID(CAN_HandleTypeDef *hcan, uint8_t motor_id, uint8_t master_id)
{
    uint32_t temp_id = 0;
    uint8_t data[8] = {0};
    temp_id = (master_id << 8 | motor_id);
    CAN_Send_StdDataFrame_Ext(hcan, temp_id, data);
}

void CyberGear_Motor_Enable(CAN_HandleTypeDef *hcan, uint8_t motor_id, uint8_t master_id)
{
    uint32_t temp_id = 0;
    uint8_t data[8] = {0};
    temp_id = Motor_Enable << 24 | master_id << 8 | motor_id;
    CAN_Send_StdDataFrame_Ext(hcan, temp_id, data);
}

void CyberGear_Motor_Disable(CAN_HandleTypeDef *hcan, uint8_t motor_id, uint8_t master_id)
{
    uint32_t temp_id = 0;
    uint8_t data[8] = {0};
    temp_id = Motor_Disable << 24 | master_id << 8 | motor_id;
    CAN_Send_StdDataFrame_Ext(hcan, temp_id, data);
}
