#ifndef _DRV8910_H    /* Guard against multiple inclusion */
#define _DRV8910_H
#endif

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include <stdint.h>

#define WRITE     0U
#define READ      1U
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
/*--------------------------------------*/
/*Register Data*/
/*-------------------------------*/
#define IC_STAT_REG      0U //for Reference 
#define CONFIG_REG       0x1BU //DRV8910 
/*
 * Operation Control Registers
    1,2,3->HIGH   Motor 1
    4,5,6->LOW    Motor 1
    7,8,9->HIGH   Motor 2
    10,11,12->LOW Motor 2
*/
#define OP_CTRL_REG_1     0x6AU // HB 1-3 High side enabled , HB 4 Low side enabled
#define OP_CTRL_REG_2     0xA5U // HB 5-6 Low side enabled , HB 7 High side enabled
#define OP_CTRL_REG_3     0x05U // HB 8 High side enable, HB 9-10 Low side enable
 
#define PWM_CTRL_REG_1     0xFFU //HB 1-8  PWM Mode
#define PWM_CTRL_REG_2     0xC3U //HB 9-10 PWM Mode , PWM Generators 1-2 Enabled

#define FW_CTRL_REG_1     0xFFU //Active FW on HB 1-8
#define FW_CTRL_REG_2     0x0FU //Active FW on HB 9-10

#define PWM_MAP_CTRL_REG_1     0x00U //HB 1-4 mapped to PWM ch 1 
#define PWM_MAP_CTRL_REG_2     0x50U //HB 5-6 mapped to pwm ch 1 , HB 7-8 mapped to pwm ch 2
#define PWM_MAP_CTRL_REG_3     0x05U //HB 9-10 mapped to pwm ch 2

#define PWM_FREQ_CTRL_REG     0xFFU // PWM ch1-ch4 FREQ 2000 Hz
/*---------------------------------*/
/*DRV8910 APIs*/
/*-----------------------------------------*/





