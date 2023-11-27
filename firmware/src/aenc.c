#include "aenc.h"
#include "hsppcv.h"

void enableModules(void)
{
    ADCHS_ModulesEnable(ADCHS_MODULE0_MASK); // Enables ADC Module 0
    ADCHS_ModulesEnable(ADCHS_MODULE2_MASK); // Enables ADC Module 2
    ADCHS_ModulesEnable(ADCHS_MODULE3_MASK); // Enables ADC Module 3
    
    return;   
}

void disableModules(void)
{
    ADCHS_ModulesDisable(ADCHS_MODULE0_MASK); // Disable ADC Module 0
    ADCHS_ModulesDisable(ADCHS_MODULE2_MASK); // Disable ADC Module 2
    ADCHS_ModulesDisable(ADCHS_MODULE3_MASK); // Disable ADC Module 3
    
    return; 
}

void conversionStart(void)
{
    ADCHS_ChannelConversionStart(AN0); // Start Conversion for channel 0
    ADCHS_ChannelConversionStart(AN2); // Start Conversion for channel 2
    ADCHS_ChannelConversionStart(AN3); // Start Conversion for channel 3
    
    return;
}

void getRawReadings(uint16_t aenc_readings[NUM_AENC])
{
    aenc_readings[idx_ROTENC] = ADCHS_Filter2DataGet();
    aenc_readings[idx_HIPENC] = ADCHS_Filter1DataGet();
    aenc_readings[idx_KNEEENC] = ADCHS_Filter3DataGet();
    
    return;
}