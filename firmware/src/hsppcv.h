/* ************************************************************************** */
/** Descriptive File Name
  @Author
    Salah Soliman
 
  @Company
    Kinoped

  @File Name
    hsppcv.h

  @Summary
    Driver to control the high speed proportional pneumatic control valve (HSPPCV) 

 */
/* ************************************************************************** */
/* Recommended frequency for this device to operate correctly is >2500 Hz
 * To use these functions, you should consider the following:
 * 1) Initialize three DEVICE structs to control each valve
 * 2) you cannot reverse the direction implicitly by changing the sign of the velocity 
 * i.e pass the direction and speed
 * 3) speed is always positive  */
#ifndef _HSPPCV_H
#define _HSPPCV_H
#endif


#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include <stdint.h>
#include <math.h>
#include "config/default/peripheral/mcpwm/plib_mcpwm.h"


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#define FSYSCLK 120000000U // should be a shared variable accross files!

#define NUM_VALVES   3U
    
#define idxROT_VALVE     0
#define idxHIP_VALVE     1
#define idxKNEE_VALVE    2
    
/* defines the devices
 Modify the values to comply with the exact channels used: MCPWM_CH_NUM channel
 */
typedef enum {
    // can be modified to accept 'n' number of devices!
    KNEE_HSPPCV_CH = MCPWM_CH_1, 
    HIP_HSPPCV_CH = MCPWM_CH_2,
    ROT_HSPPCV_CH = MCPWM_CH_3,          
}
DEVICE;

/* for now we'll assume that duty cycle > 50% is forward, and < 50% is backward, and =50% is stop!*/

static uint8_t command_hsppcv_duties[NUM_VALVES];

/* hsppcv_device_config: set the frequency and starts PWM
 * Arguments
    * None : void 
*/
extern void hsppcvDeviceConfig(void);

/* hsppcv_speed_control: set the speed (Air flow) for all valves
 * Arguments   
    * duties: array of desired duty cycles for each valve
*/
extern void hsppcvDutyCycleControl(uint8_t duties[NUM_VALVES]);
/* *****************************************************************************
 End of File
 */
