/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "third_party/rtos/FreeRTOS/Source/include/FreeRTOS.h"
#include "third_party/rtos/FreeRTOS/Source/include/task.h"
#include "third_party/rtos/FreeRTOS/Source/include/semphr.h"
#include "third_party/rtos/FreeRTOS/Source/include/queue.h"
#include <stdio.h>  
#include <math.h>
#include "roscontrol.h"

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    leg = RIGHT_LEG;
    
    xTaskCreate(MAIN_INIT_TASK, "Main Initialization", 256, NULL, tskIDLE_PRIORITY + 2, NULL);
    /* Initialize all modules */
    vTaskStartScheduler();
//    SYS_Initialize ( NULL );

    while ( true )
    {     
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/