#include "Task_Init.h"
#include "Communication_About.h"
#include "Travel_Switch.h"

uint8_t task_return[5];
uint32_t task_highwatermark[5];

TaskHandle_t User_Debug_Task_Handle;
void User_Debug_Task(void *pvParameters);

void Start_Task(void)
{   
    /* 初始化CAN滤波器 */
    CanFilter_Init(&hcan1);
    CanFilter_Init(&hcan2);
    
    /* 启动CAN模块 */
    HAL_CAN_Start(&hcan1);
    HAL_CAN_Start(&hcan2);
    
    /* 启动CAN中断 */
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING);
    
    HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_Base_Start_IT(&htim2);
    
#if Debug_Remote
    Remote_DMA_Finish_Semaphore = xSemaphoreCreateBinary();
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart1, Remote_RxArr, Remote_Leng);
#endif
    
//    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
//    __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);
    
//    HAL_UART_Receive_DMA(&huart2, Pressure_Sensor_Data_No1, Sensor_DataLong);
//    HAL_UART_Receive_DMA(&huart4, Pressure_Sensor_Data_No1, Sensor_DataLong);
    HAL_UART_Receive_DMA(&huart5, Pressure_Sensor_Data_No1, Sensor_DataLong);
    HAL_UART_Receive_DMA(&huart3, Pressure_Sensor_Data_No2, Sensor_DataLong);
    HAL_UART_Receive_DMA(&huart6, Pressure_Sensor_Data_No3, Sensor_DataLong);
    
    HAL_NVIC_DisableIRQ(DMA1_Stream1_IRQn);
    HAL_NVIC_DisableIRQ(DMA1_Stream3_IRQn);
    HAL_NVIC_DisableIRQ(DMA1_Stream5_IRQn);
    HAL_NVIC_DisableIRQ(DMA1_Stream6_IRQn);
    
    HAL_NVIC_DisableIRQ(DMA2_Stream1_IRQn);
    HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);
    HAL_NVIC_DisableIRQ(DMA2_Stream6_IRQn);

//    Chassis_Motor_Init();
    MI_motor_Init(&MI_Chassis_Motor[0], &hcan1, 1);
    MI_motor_Init(&MI_Chassis_Motor[1], &hcan1, 2);
    MI_motor_Init(&MI_Chassis_Motor[2], &hcan1, 3);
    MI_motor_Init(&MI_Chassis_Motor[3], &hcan2, 3);
    
    GimbalMotor_Init();
    
    Gimbal_Vision_Control_Exp.Exp_Vision_X = 320;
    Gimbal_Vision_Control_Exp.Exp_Vision_Y = 240;
    
    task_return[0] = xTaskCreate(Gimbal_Control_Task, "Gimbal_Control_Task", 128, NULL, osPriorityNormal, &Gimbal_Control_Task_Handle);
    task_return[1] = xTaskCreate(Chassis_Control_Task, "Chassis_Control_Task", 256, NULL, osPriorityNormal, &Chassis_Task_Handle);
    task_return[2] = xTaskCreate(Master_Controller_Task, "Master_Controller_Task", 128, NULL, osPriorityNormal, &Master_Controller_Task_Handle);
#if Debug_Remote
    xTaskCreate(Remote_Task, "Remote_Task", 128, NULL, osPriorityNormal, &Remote_Task_Handle);
#endif
    task_return[3] = xTaskCreate(Read_Travel_Switch_Task, "Read_Travel_Switch_Task", 128, NULL, osPriorityNormal, &Read_Travel_Switch_Task_Handle);
    task_return[4] = xTaskCreate(User_Debug_Task, "User_Debug_Task", 128, NULL, osPriorityNormal, &User_Debug_Task_Handle);
}

void User_Debug_Task(void *pvParameters)
{
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for(;;)
    {
        task_highwatermark[0] = uxTaskGetStackHighWaterMark(Gimbal_Control_Task_Handle);
        task_highwatermark[1] = uxTaskGetStackHighWaterMark(Chassis_Task_Handle);
        task_highwatermark[2] = uxTaskGetStackHighWaterMark(Master_Controller_Task_Handle);
        task_highwatermark[3] = uxTaskGetStackHighWaterMark(Read_Travel_Switch_Task_Handle);
        task_highwatermark[4] = uxTaskGetStackHighWaterMark(User_Debug_Task_Handle);
        
        vTaskDelayUntil(&xLastWakeTime,2);
    }
}


