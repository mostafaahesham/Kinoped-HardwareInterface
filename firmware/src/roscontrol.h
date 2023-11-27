/* ************************************************************************** */
/** Descriptive File Name
  @Author
    Mostafa Hesham
 
  @Company
    Kinoped

  @File Name
    roscontrol.h

  @Summary
    Driver to Handle Data Exchange Between ROS & PCB using UART1

 */
/* ************************************************************************** */
/* Communication Baud Rate is 115200
 * To use these functions, you should consider the following:
 * 1) Initialize 2 ROS Message Objects for Data Exchange
 * 2) Message Consists of 13 Bytes with a SOF & EOF to preserve the Message Order Structure
*/

#ifndef _ROSCONTROL_H    /* Guard against multiple inclusion */
#define _ROSCONTROL_H
#endif

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include <stdint.h>
#include <math.h>
#include "xtrdac.h"
#include "aenc.h"
#include "denc.h"
#include "hsppcv.h"
#include "third_party/rtos/FreeRTOS/Source/include/FreeRTOS.h"
#include "third_party/rtos/FreeRTOS/Source/include/task.h"
#include "third_party/rtos/FreeRTOS/Source/include/semphr.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
/************************/
/*Message Parameters*/
/************************/
#define CMD_MESSAGE_SIZE         9U
#define TELEM_MESSAGE_SIZE       9U   
    
#define SOF     0xFBU
#define EOF     0xFEU 
    
#define RIGHT_LEG          1U
#define LEFT_LEG           2U  
    
#define LOOP_FREQUENCY     20U
#define ITVs_PRESSURIZATION_FREQUENCY     100U   
/************************************/
/*Message Structure*/
/*Joint Positions are raw uint16_t values*/
/*HSPPCV Duty Cycle : 0 ~ 100%*/
/*ITV Pressure  : 0 ~ 110 psi*/
/************************************/    
#define idx_SOF     0
#define idx_LEGID   1
#define idx_EOF     8
/***********************************/
/*Command message indicies*/
/***********************************/
#define idxROT_PRESSURE     2  
#define idxHIP_PRESSURE     3
#define idxKNEE_PRESSURE    4

#define idxROT_DUTY         5
#define idxHIP_DUTY         6
#define idxKNEE_DUTY        7
/***********************************/
/*Telemetry message indicies*/
/***********************************/
#define idxROT_POS16           2
#define idxROT_POS8            3
#define idxHIP_POS16           4
#define idxHIP_POS8            5
#define idxKNEE_POS16          6
#define idxKNEE_POS8           7
/***********************************/
/*Message Variables*/
/***********************************/
static uint8_t leg;    
    
static uint8_t command_message[CMD_MESSAGE_SIZE];
static uint8_t telemetry_message[TELEM_MESSAGE_SIZE];
/******************************************/
/*RTOS Handles*/
/******************************************/
static xTaskHandle xToROSControl;
static xTaskHandle xFromROSControl;
static xTaskHandle xJointControl;
static xTaskHandle xPressureControl;

static SemaphoreHandle_t commands_reception_status;
/******************************************/
/*Function Declarations*/
/******************************************/
extern void MAIN_INIT_TASK(void *argument);

extern void updateTelemetryMessage(uint8_t telem_msg[TELEM_MESSAGE_SIZE],uint8_t leg_id);
extern void updateCommandMessage(uint8_t cmd_msg[CMD_MESSAGE_SIZE]);

extern void toROSControl(void *argument);
extern void fromROSControl(void *argument);

extern void pressureControl(void *argument);
extern void jointControl(void *argument);
/******************************************/