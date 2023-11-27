#include "denc.h"

void DENC_Initialize(DENC denc, uint32_t max_pulse_count)
{
    if(denc == HIP_DENC)
    {
        QEI1_PositionCountSet(max_pulse_count);
        QEI2_PositionCountSet(max_pulse_count);
    }
    else if(denc == KNEE_DENC)
    {
        QEI3_PositionCountSet(max_pulse_count);
        QEI4_PositionCountSet(max_pulse_count);
    }

    QEI1_Start();
    QEI2_Start();
    QEI3_Start();
    QEI4_Start();
    
    return;
}

void UPDATELeg_Lengths(void)
{
    static uint32_t hip_pulse_count;
    static uint32_t knee_pulse_count;
    
    hip_pulse_count = QEI1_PositionGet();
    knee_pulse_count = QEI3_PositionGet();
    
    leg_lengths[HIP_EXT] = CACLCULATE_LENGTH(hip_pulse_count);
    leg_lengths[KNEE_EXT] = CACLCULATE_LENGTH(knee_pulse_count);
}