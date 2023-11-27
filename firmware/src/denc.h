/* ************************************************************************** */
/** Descriptive File Name
  @Author
    Mostafa Hesham
 
  @Company
    Kinoped

  @File Name
    denc.h

  @Summary
    Driver to read the Leg Extension Lengths 

 */
/* ************************************************************************** */
/*
 * To use these functions, you should consider the following:
 * 1) Initialize 4 QEI Peripherals to read each encoder
 * 2) Encoder Count is incrementing during Extension & decrementing on retraction. 
 * 3) No Home, index, Limit Switches Pins Available so Homing will be Blind Retraction for an approximated amount of time
        or till the pulse count stops changing.
 * 4) For now the Extension & Retraction are gonna be done manually from an external DC motor driver.
 */
#ifndef _DENC_H
#define _DENC_H
#endif

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include <stdint.h>
#include <math.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    // can be modified to accept 'n' number of devices!
    HIP_DENC = 0, 
    KNEE_DENC = 1,          
}
DENC;
    
#define HIP_EXT      0
#define KNEE_EXT     1

#define NUM_SEGMENTS     2    //2 Extensions per Leg Segment

#define HIP_MAX_COUNT      (uint32_t)3000
#define KNEE_MAX_COUNT     (uint32_t)5000

#define MAX_LENGTH      (uint8_t)20  //in cm.  
    
#define HIP_MAX_LENGTH     (uint8_t)20U    //in cm.
#define KNEE_MAX_LENGTH    (uint8_t)20U    //in cm .
    
#define CACLCULATE_LENGTH(c) (uint8_t)((c * (uint32_t)MAX_LENGTH) / (uint32_t)5000)    
    
static uint8_t leg_lengths[NUM_SEGMENTS]; // Calculated Leg Length (in cm) to be Sent in the Telemetry Packet

/* denc_iniatialize: Starts the Specified QEI Module with desired MAx Pulse Count.
 * Arguments
    * denc : DENC 
    * pulse_count : uint32_t 
*/
extern void DENC_Initialize(DENC denc,uint32_t max_pulse_count);

/* update_lengths: Calculates Leg Segment Length & Passes it to the Telemetry Packet
 * Arguments
    * None : void 
*/
extern void UPDATELeg_Lengths(void);
/* *****************************************************************************
 End of File
 */
