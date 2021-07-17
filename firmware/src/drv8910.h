#ifndef _DRV8910_H    /* Guard against multiple inclusion */
#define _DRV8910_H
#endif

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include <stdint.h>

#define HIGH     1U
#define LOW      0U
/*------------------------*/
/*DRV8910 Variables*/
/*-------------------------*/
static uint8_t  duty_cycle;

static uint16_t frame;
static uint16_t response;
/*-----------------------*/
#define WRITE     0U
#define READ      1U

#define FORWARD       1U
#define BACKWARDS     0U
/*------------------*/
/*Register Addresses*/
/*----------------------------------*/
#define IC_STAT_ADDR           0x00U //Read Only
#define CONFIG_REG_ADDR        0x07U

#define OP_CTRL_REG_1_ADDR     0x08U
#define OP_CTRL_REG_2_ADDR     0x09U
#define OP_CTRL_REG_3_ADDR     0x0AU

#define PWM_CTRL_REG_1_ADDR      0x0BU
#define PWM_CTRL_REG_2_ADDR      0x0CU

#define FW_CTRL_REG_1_ADDR     0x0DU
#define FW_CTRL_REG_2_ADDR     0x0EU

#define PWM_MAP_CTRL_REG_1_ADDR     0x0FU
#define PWM_MAP_CTRL_REG_2_ADDR     0x10U
#define PWM_MAP_CTRL_REG_3_ADDR     0x11U

#define PWM_FREQ_CTRL_REG_ADDR     0x12U

#define PWM_DUTY_CH1_REG_ADDR     0x13U
#define PWM_DUTY_CH2_REG_ADDR     0x14U
/*-------------------------------------*/
/*Register Data*/
/*-------------------------*/
#define IC_STAT_REG      0U //for Reference 
#define CONFIG_REG       0x1BU //DRV8910 and some faults reporting configurations
/*
 * Operation Control Registers
 * 1,4,5,6,7,9 Motor 1 PWM ch 1
 * 2,3,8,10 Motor 2 PWM ch 
 * 
 * 1,5,7 Motor 1 High , 4,6,9 Motor 1 Low
 * 2,8 Motor 2 High , 3,10 Motor 2 Low
*/
#define OP_CTRL_REG_1     0x5AU // HB 1,2 High , HB 3,4 Low
#define OP_CTRL_REG_2     0xA6U // HB 5,7,8 High , HB 6 Low
#define OP_CTRL_REG_3     0x05U // HB 9,10 Low
 
#define PWM_CTRL_REG_1     0xFFU //HB 1-8  PWM Mode
#define PWM_CTRL_REG_2     0xC3U //HB 9-10 PWM Mode , PWM Generators 1-2 Enabled

#define FW_CTRL_REG_1     0xFFU //Active FW on HB 1-8
#define FW_CTRL_REG_2     0x0FU //Active FW on HB 9-10

#define PWM_MAP_CTRL_REG_1     0x14U //1,4 PWM ch 1 , 2,3 PWM ch 2
#define PWM_MAP_CTRL_REG_2     0x40U //5,6,7 PWM ch 1 , 8 PWM ch 2
#define PWM_MAP_CTRL_REG_3     0x04U //9 PWM ch 1 , 10 PWM ch 2

#define PWM_FREQ_CTRL_REG     0x0FU // PWM ch 1 , ch 2 FREQ 2000 Hz
/*---------------------------------*/
/*DRV8910 APIs*/
/*-----------------------------------------*/
#define CALCULATE_DUTY(d)     duty_cycle = (d/(float)100)*(float)255

bool PWM_Config(void);
bool FW_Config(bool enable);
bool PWM_Map(void);
bool PWM_Write(uint8_t motor,uint8_t duty);

