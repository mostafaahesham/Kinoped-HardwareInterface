#include "xtrdac.h"

void pressurizeItvs(uint8_t pressures[NUM_ITVS])
{
    /*Writes 12 bit value to the corresponding DACs
     * VARS:
     * pressure range [0-86 psi]
     */
    static uint16_t volts[NUM_ITVS];
    
    pressures[idxROT_ITV] = CLIP(pressures[idxROT_ITV]);
    pressures[idxHIP_ITV] = CLIP(pressures[idxHIP_ITV]);
    pressures[idxKNEE_ITV] = CLIP(pressures[idxKNEE_ITV]);
    
    volts[idxROT_ITV] = CALCULATE_VOLT(pressures[idxROT_ITV]);
    volts[idxHIP_ITV] = CALCULATE_VOLT(pressures[idxHIP_ITV]);
    volts[idxKNEE_ITV] = CALCULATE_VOLT(pressures[idxKNEE_ITV]);
 
    DAC2CONbits.DACDAT = volts[idxROT_ITV];
    DAC3CONbits.DACDAT = volts[idxHIP_ITV];
    DAC1CONbits.DACDAT = volts[idxKNEE_ITV];
    
    return;
}