
#ifndef _AENC_H    /* Guard against multiple inclusion */
#define _AENC_H
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
#include "xtrdac.h"

#define NUM_AENC  3U //will be 6 when other 3 encoders are used

#define idx_ROTENC   0
#define idx_HIPENC   1
#define idx_KNEEENC  2

#define AN0    ADCHS_CH0
#define AN2    ADCHS_CH2
#define AN3    ADCHS_CH3

static uint16_t raw_readings[NUM_AENC];

extern void enableModules(void);
extern void disableModules(void);
extern void conversionStart(void);
extern void getRawReadings(uint16_t aenc_readings[NUM_AENC]);