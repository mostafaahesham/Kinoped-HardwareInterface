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

struct MOTOR
{
    uint8_t driver_id;
    uint8_t motor_id;
    uint8_t action[3];
};
/*------------------------*/
/*DRV8910 Variables*/
/*-------------------------*/
static uint8_t  duty_cycle;

static uint16_t frame;
static uint16_t response;
/*-----------------------*/
#define WRITE     0U
#define READ      1U

#define STOP          1U
#define FORWARD       2U
#define BACKWARDS     3U

#define OP_CTRL_REG_1     0U 
#define OP_CTRL_REG_2     1U 
#define OP_CTRL_REG_3     2U 

#define PA0622_1      1U
#define PA0622_2      2U
#define PA06180_1     3U
#define PA06180_2     4U  

#define DRIVER_1     0U
#define DRIVER_2     1U
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
/*Motor 1 Direction Masks*/
#define M1_MASK1_FWD     0x42U                                      
#define M1_MASK1_BWD     0x81U

#define M1_MASK2_FWD     0x26U
#define M1_MASK2_BWD     0x19U

#define M1_MASK3_FWD     0x01U
#define M1_MASK3_BWD     0x02U
/*Motor 2 Direction Masks*/
#define M2_MASK1_FWD     0x18U                                      
#define M2_MASK1_BWD     0x24U

#define M2_MASK2_FWD     0x80U
#define M2_MASK2_BWD     0x40U

#define M2_MASK3_FWD     0x04U
#define M2_MASK3_BWD     0x08U
 
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

bool COMMAND_Send(bool operation,uint8_t address,uint8_t data,bool driver_id);

bool PWM_Config(void);
bool FW_Config(bool enable);
bool PWM_Map(void);
bool PWM_Freq(void);
bool PWM_Duty(uint8_t motor , uint8_t duty);
bool MOTOR_Control(struct MOTOR motor);


