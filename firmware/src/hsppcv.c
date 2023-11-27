/* ************************************************************************** */
/** Descriptive File Name
  @Author 
    Salah Soliman
   
  @Company
    Kinoped

  @File Name
    hsppcv.c

  @Summary
    Driver to control the high speed proportional pneumatic control valve (HSPPCV) 

 */
/* ************************************************************************** */

#include "hsppcv.h"

void hsppcvDeviceConfig(void)
{   
       MCPWM_Start();
       MCPWM_PrimaryPeriodSet( (uint16_t)(FSYSCLK/(2150 * (uint16_t)pow(2, PTCONbits.PCLKDIV))));
       
       return;
}
void hsppcvDutyCycleControl(uint8_t duties[NUM_VALVES])
{       
        MCPWM_ChannelPrimaryDutySet(ROT_HSPPCV_CH,65536.0 * ((float)duties[idxROT_VALVE]/100.0));
        MCPWM_ChannelPrimaryDutySet(HIP_HSPPCV_CH,65536.0 * ((float)duties[idxHIP_VALVE]/100.0));
        MCPWM_ChannelPrimaryDutySet(KNEE_HSPPCV_CH,65536.0 * ((float)duties[idxKNEE_VALVE]/100.0));
        
        return;
}
