#ifndef _XTRDAC_H    /* Guard against multiple inclusion */
#define _XTRDAC_H
#endif

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include <stdint.h>
#include <math.h>
#include "third_party/rtos/FreeRTOS/Source/include/FreeRTOS.h"
#include "third_party/rtos/FreeRTOS/Source/include/task.h"
#include "config/default/peripheral/uart/plib_uart1.h"
#include "config/default/peripheral/adchs/plib_adchs.h"
#include "config/default/peripheral/mcpwm/plib_mcpwm.h"
/*------------------------*/
/*XTR Variables*/
/*-------------------------*/
#define NUM_ITVS  3U

#define idxROT_ITV    0                      
#define idxHIP_ITV    1
#define idxKNEE_ITV   2

#define MAX_PRESSURE     110.0

static uint8_t command_itv_pressures[NUM_ITVS];
/*------------------------*/
/*XTR APIs*/
/*-------------------------*/
#define CLIP(p) ( ((p) > MAX_PRESSURE) ? MAX_PRESSURE : ( (-(p) > MAX_PRESSURE) ? (-MAX_PRESSURE) : (p) ) )
#define CALCULATE_VOLT(p)   (uint16_t)((float)p*25.83 + 688.0)

extern void pressurizeItvs(uint8_t pressures[NUM_ITVS]);