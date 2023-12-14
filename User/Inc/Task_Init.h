#ifndef __TASK_INIT_H__
#define __TASK_INIT_H__

#include "main.h"
#include "can.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "cmsis_os.h"
#include "task.h"
#include "queue.h"

#include "CANDrive.h"
#include "Chassis.h"
#include "motor.h"
#include "PID.h"
#include "ramp.h"
#include "WatchDog.h"

#include "Gimbal_Task.h"
#include "Chassis_Task.h"
#include "Pressure_Sensor_Task.h"

//#include "XiaoMi_CyberGear_Motor.h"

void Start_Task(void);

#endif 
