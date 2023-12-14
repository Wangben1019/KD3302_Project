/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Communication_About.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint32_t push_drain_delay_cnt;
uint32_t draining_delay_cnt;
uint32_t push_tank_delay_cnt;
uint32_t return_drain_cnt;
uint32_t return_tank_cnt;

uint8_t Jack_Up_Remote_Return;
uint8_t Jack_Down_Remote_Return;
uint32_t Jack_Up_Remote_cnt;
uint32_t Jack_Down_Remote_cnt;

int32_t Jack_Up_FordwareRunTime;
int32_t Jack_Down_FordwareRunTime;

uint32_t Vision_Interrupt_cnt;
uint32_t Joy_Interrupt_cnt;
uint32_t Navigation_Interrupt_cnt;
uint32_t Goal_Interrupt_cnt;

uint32_t Vision_Control_Gimbal_cnt;

//uint32_t jack_down_run_time;
//uint32_t jack_up_run_time;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim12;
extern DMA_HandleTypeDef hdma_uart5_rx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern DMA_HandleTypeDef hdma_usart6_rx;
extern DMA_HandleTypeDef hdma_usart6_tx;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
extern TIM_HandleTypeDef htim8;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */

  /* USER CODE END DMA1_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart5_rx);
  /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */

  /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */

  /* USER CODE END DMA1_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart3_rx);
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

  /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream3 global interrupt.
  */
void DMA1_Stream3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream3_IRQn 0 */

  /* USER CODE END DMA1_Stream3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart3_tx);
  /* USER CODE BEGIN DMA1_Stream3_IRQn 1 */

  /* USER CODE END DMA1_Stream3_IRQn 1 */
}

/**
  * @brief This function handles CAN1 RX0 interrupts.
  */
void CAN1_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN1_RX0_IRQn 0 */

  /* USER CODE END CAN1_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan1);
  /* USER CODE BEGIN CAN1_RX0_IRQn 1 */

  /* USER CODE END CAN1_RX0_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
  */
void TIM1_UP_TIM10_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */
    
    if(Gimbal_Motor_X.Vision_Control_Flag == 1 && Gimbal_Motor_Y.Vision_Control_Flag == 1)
    {
        if((__fabs(Gimbal_Vision_Control_Exp.Exp_Vision_X - 320) < 10) && (__fabs(Gimbal_Vision_Control_Exp.Exp_Vision_Y - 240) < 10))
        {
            Vision_Control_Gimbal_cnt++;
        }
        else 
        {
            Vision_Control_Gimbal_cnt = 0;
            Vision_Finish_Base_Time_Flag = 0;
        }
        
        if(Vision_Control_Gimbal_cnt == 1000)
        {
            Vision_Control_Gimbal_cnt = 0;
            Vision_Finish_Base_Time_Flag = 1;
        }
    }
    else
    {
        Vision_Control_Gimbal_cnt = 0;
        Vision_Finish_Base_Time_Flag = 0;
    }
//    if(Transmit_Can_Info.Control_Info.Jack_Control_Info_Down == 1)
//    {
//        jack_down_run_time++;
//    }
//    if(Transmit_Can_Info.Control_Info.Jack_Control_Info_Up == 1)
//    {
//        jack_up_run_time++;
//    }
        
    if(System_Run_State == Pushing_electric_push)
    {
        push_drain_delay_cnt++;
        if(push_drain_delay_cnt >= Push_Drain_Pushing_Delay)
        {
            System_Run_State = Draining_Oil;
            Last_System_Run_State = Pushing_electric_push;
            push_drain_delay_cnt = 0;
        }
    }
    else if(System_Run_State == Draining_Oil)
    {
        draining_delay_cnt++;
        if(draining_delay_cnt >= Draining_Oil_Delay)
        {
            System_Run_State = Push_Out_Tank;
            Last_System_Run_State = Draining_Oil;
            draining_delay_cnt = 0;
            draining_need_return = 1;
        }
    }
    else if(System_Run_State == Push_Out_Tank)
    {
        push_tank_delay_cnt++;
        if(push_tank_delay_cnt >= Push_Tank_Delay)
        {
            System_Run_State = Push_Return_Tank;
            Last_System_Run_State = Push_Out_Tank;
            push_tank_delay_cnt = 0;
            tank_need_return = 1;
        }
    }
    
    if(draining_need_return == 1)
    {
        return_drain_cnt++;
        if(return_drain_cnt >= Return_Draining_Pushing_Delay)
        {
            draining_need_return = 0;
            return_drain_cnt = 0;
            
        }
    }
    
    if(tank_need_return == 1)
    {
        return_tank_cnt++;
        if(return_tank_cnt >= Return_Tank_Delay)
        {
            tank_need_return = 0;
            return_tank_cnt = 0;
        }
    }
    
    /* 记录千斤顶向外工作时�? */
    if(Transmit_Can_Info.Control_Info.Jack_Control_Info_Down == 1)
    {
        Jack_Down_FordwareRunTime++;
    }
    /* 基于工作时间将千斤顶缩回 */
    else if(Transmit_Can_Info.Control_Info.Jack_Control_Info_Down == 2 && Jack_Down_Remote_Return == 0)
    {
        Jack_Down_FordwareRunTime--;
        if(System_Run_State == Finish_A_Reset && Jack_Down_FordwareRunTime <= 0)
        {
            jack_down_need_return = 0;
            Transmit_Can_Info.Control_Info.Jack_Control_Info_Down = 0;
        }
    }
    
    /* 记录千斤顶向外工作时�? */
    if(Transmit_Can_Info.Control_Info.Jack_Control_Info_Up == 1)
    {
        Jack_Up_FordwareRunTime++;
    }
    /* 基于工作时间将千斤顶缩回 */
    else if(Transmit_Can_Info.Control_Info.Jack_Control_Info_Up == 2 && Jack_Up_Remote_Return == 0)
    {
        Jack_Up_FordwareRunTime--;
        if(System_Run_State == Finish_A_Reset && Jack_Up_FordwareRunTime <= 0)
        {
            jack_up_need_return = 0;
            Transmit_Can_Info.Control_Info.Jack_Control_Info_Up = 0;
        }
    }
        
  /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
    /* 防止PC控制信号残留 */
    Vision_Interrupt_cnt++;
    if(Vision_Interrupt_cnt == 500)
    {
        Gimbal_Vision_Control_Exp.Exp_Vision_X = 320;
        Gimbal_Vision_Control_Exp.Exp_Vision_Y = 240;
        Gimbal_Vision_Control_Exp.Exp_Vision_Z = 0;
        Gimbal_Motor_X.Vision_Control_Flag = 0;
        Gimbal_Motor_Y.Vision_Control_Flag = 0;
        Vision_Interrupt_cnt = 0;
    }
    
//    Joy_Interrupt_cnt++;
//    if(Joy_Interrupt_cnt == 500)
//    {
////        JoyGimbal_Control_Exp.Control_Screwdriver_Run = 0;
////        JoyGimbal_Control_Exp.Expect_Sport_X = 0;
////        JoyGimbal_Control_Exp.Expect_Sport_Y = 0;
////        JoyGimbal_Control_Exp.Expect_Sport_Z = 0;
////        JoyGimbal_Control_Exp.Stop = 0;
////        Joy_Interrupt_cnt = 0;
//    }
    
    Navigation_Interrupt_cnt++;
    if(Navigation_Interrupt_cnt == 500)
    {
//        Robot_Chassis_Exp.Expect_Speed_X = 0;
//        Robot_Chassis_Exp.Expect_Speed_Y = 0;
//        Robot_Chassis_Exp.Expect_Speed_Yaw = 0;
        Navigation_Interrupt_cnt = 0;
    }
    
    Goal_Interrupt_cnt++;
    if(Goal_Interrupt_cnt == 500)
    {
        Vision_goal_expect.Vision_goal_x = 0;
        Vision_goal_expect.Vision_goal_y = 0;
        Goal_Interrupt_cnt = 0;
    }
    
    if(JoyGimbal_Control_Exp.Reset_Jack_Down == 1)
    {
        Jack_Down_Remote_Return = 1;
        System_Run_State = Person_Control;
    }
    if(JoyGimbal_Control_Exp.Reset_Jack_Up == 1)
    {
        Jack_Up_Remote_Return = 1;
        System_Run_State = Person_Control;
    }
    
    if(Jack_Down_Remote_Return == 1)
    {
        Transmit_Can_Info.Control_Info.Jack_Control_Info_Down = 2;
        Jack_Down_Remote_cnt++;
        if(Jack_Down_Remote_cnt >= Jack_Remote_Return_Time)
        {
            Transmit_Can_Info.Control_Info.Jack_Control_Info_Down = 0;
            Jack_Down_Remote_cnt = 0;
            Jack_Down_Remote_Return = 0;
        }
    }
    if(Jack_Up_Remote_Return == 1)
    {
        Transmit_Can_Info.Control_Info.Jack_Control_Info_Up = 2;
        Jack_Up_Remote_cnt++;
        if(Jack_Up_Remote_cnt >= Jack_Remote_Return_Time)
        {
            Transmit_Can_Info.Control_Info.Jack_Control_Info_Up = 0;
            Jack_Up_Remote_Return = 0;
            Jack_Up_Remote_cnt = 0;
        }
    }
  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */
    
  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */

  /* USER CODE END TIM4_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
#if Debug_Remote
    BaseType_t xHigherPriorityTaskWoken;
    if ((__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET) && __HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        (void)huart1.Instance->SR;
        (void)huart1.Instance->DR;

        HAL_UART_DMAStop(&huart1);
        (void)hdma_usart1_rx.Instance->NDTR;

        HAL_UART_Receive_DMA(&huart1, Remote_RxArr, Remote_Leng);
        xSemaphoreGiveFromISR(Remote_DMA_Finish_Semaphore,&xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
#endif
#if 0    
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
#endif
  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
    if ((__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE) != RESET) && __HAL_UART_GET_IT_SOURCE(&huart3, UART_IT_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart3);
        (void)huart3.Instance->SR;
        (void)huart3.Instance->DR;

        HAL_UART_DMAStop(&huart3);
        (void)hdma_usart3_rx.Instance->NDTR;
        
        Pressure_Data_Decode(Pressure_Sensor_Data_No2, &Pressure_Sensor_Jack_Down);
        
        HAL_UART_Receive_DMA(&huart3, Pressure_Sensor_Data_No2, Sensor_DataLong);
    }
#if 0
  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */
#endif
  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles TIM8 break interrupt and TIM12 global interrupt.
  */
void TIM8_BRK_TIM12_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_BRK_TIM12_IRQn 0 */

  /* USER CODE END TIM8_BRK_TIM12_IRQn 0 */
  HAL_TIM_IRQHandler(&htim12);
  /* USER CODE BEGIN TIM8_BRK_TIM12_IRQn 1 */

  /* USER CODE END TIM8_BRK_TIM12_IRQn 1 */
}

/**
  * @brief This function handles TIM8 update interrupt and TIM13 global interrupt.
  */
void TIM8_UP_TIM13_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 0 */

  /* USER CODE END TIM8_UP_TIM13_IRQn 0 */
  HAL_TIM_IRQHandler(&htim8);
  /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 1 */

  /* USER CODE END TIM8_UP_TIM13_IRQn 1 */
}

/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler(void)
{
  /* USER CODE BEGIN TIM5_IRQn 0 */

  /* USER CODE END TIM5_IRQn 0 */
  HAL_TIM_IRQHandler(&htim5);
  /* USER CODE BEGIN TIM5_IRQn 1 */

  /* USER CODE END TIM5_IRQn 1 */
}

/**
  * @brief This function handles UART5 global interrupt.
  */
void UART5_IRQHandler(void)
{
  /* USER CODE BEGIN UART5_IRQn 0 */
    if ((__HAL_UART_GET_FLAG(&huart5, UART_FLAG_IDLE) != RESET) && __HAL_UART_GET_IT_SOURCE(&huart5, UART_IT_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart5);
        (void)huart5.Instance->SR;
        (void)huart5.Instance->DR;

        HAL_UART_DMAStop(&huart5);
        (void)hdma_uart5_rx.Instance->NDTR;
        
        Pressure_Data_Decode(Pressure_Sensor_Data_No1, &Pressure_Sensor_Screwdriver);
        
        HAL_UART_Receive_DMA(&huart5, Pressure_Sensor_Data_No1, Sensor_DataLong);
    }
#if 0
  /* USER CODE END UART5_IRQn 0 */
  HAL_UART_IRQHandler(&huart5);
  /* USER CODE BEGIN UART5_IRQn 1 */
#endif
  /* USER CODE END UART5_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream1 global interrupt.
  */
void DMA2_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream1_IRQn 0 */

  /* USER CODE END DMA2_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart6_rx);
  /* USER CODE BEGIN DMA2_Stream1_IRQn 1 */

  /* USER CODE END DMA2_Stream1_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA2_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */

  /* USER CODE END DMA2_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
  /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

  /* USER CODE END DMA2_Stream2_IRQn 1 */
}

/**
  * @brief This function handles CAN2 RX1 interrupt.
  */
void CAN2_RX1_IRQHandler(void)
{
  /* USER CODE BEGIN CAN2_RX1_IRQn 0 */

  /* USER CODE END CAN2_RX1_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan2);
  /* USER CODE BEGIN CAN2_RX1_IRQn 1 */

  /* USER CODE END CAN2_RX1_IRQn 1 */
}

/**
  * @brief This function handles USB On The Go FS global interrupt.
  */
void OTG_FS_IRQHandler(void)
{
  /* USER CODE BEGIN OTG_FS_IRQn 0 */

  /* USER CODE END OTG_FS_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
  /* USER CODE BEGIN OTG_FS_IRQn 1 */

  /* USER CODE END OTG_FS_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream6 global interrupt.
  */
void DMA2_Stream6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream6_IRQn 0 */

  /* USER CODE END DMA2_Stream6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart6_tx);
  /* USER CODE BEGIN DMA2_Stream6_IRQn 1 */

  /* USER CODE END DMA2_Stream6_IRQn 1 */
}

/**
  * @brief This function handles USART6 global interrupt.
  */
void USART6_IRQHandler(void)
{
  /* USER CODE BEGIN USART6_IRQn 0 */
    if ((__HAL_UART_GET_FLAG(&huart6, UART_FLAG_IDLE) != RESET) && __HAL_UART_GET_IT_SOURCE(&huart6, UART_IT_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart6);
        (void)huart6.Instance->SR;
        (void)huart6.Instance->DR;

        HAL_UART_DMAStop(&huart6);
        (void)hdma_usart6_rx.Instance->NDTR;
        
        Pressure_Data_Decode(Pressure_Sensor_Data_No3, &Pressure_Sensor_Jack_Up);
        
        HAL_UART_Receive_DMA(&huart6, Pressure_Sensor_Data_No3, Sensor_DataLong);
    }
#if 0
  /* USER CODE END USART6_IRQn 0 */
  HAL_UART_IRQHandler(&huart6);
  /* USER CODE BEGIN USART6_IRQn 1 */
#endif
  /* USER CODE END USART6_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
