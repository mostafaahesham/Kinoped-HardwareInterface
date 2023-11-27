#include "roscontrol.h"
#include "config/default/peripheral/uart/plib_uart1.h"

void MAIN_INIT_TASK(void *argument)
{
    SYS_Initialize ( NULL );
    
    hsppcvDeviceConfig();
    
    xTaskCreate(toROSControl, "Controller To ROS", 512, NULL, tskIDLE_PRIORITY + 2, &xToROSControl);
    xTaskCreate(fromROSControl, "ROS To Controller", 512, NULL, tskIDLE_PRIORITY + 1, &xFromROSControl);
    
    xTaskCreate(pressureControl, "ITVs Pressurization", 512, NULL, tskIDLE_PRIORITY + 1, &xPressureControl);
    xTaskCreate(jointControl, "Joint Actuation", 512, NULL, tskIDLE_PRIORITY + 1,&xJointControl);
    
    vTaskDelete(NULL);
}

void updateTelemetryMessage(uint8_t telem_msg[TELEM_MESSAGE_SIZE],uint8_t leg_id)
{
    conversionStart();   
    
    getRawReadings(raw_readings);
    
    telem_msg[idx_SOF] = SOF;
    telem_msg[idx_LEGID] = leg_id;
    telem_msg[idx_EOF] = EOF;
    
    telem_msg[idxROT_POS16] = raw_readings[idx_ROTENC];
    telem_msg[idxROT_POS8] = raw_readings[idx_ROTENC] >> 8;
    
    telem_msg[idxHIP_POS16] = raw_readings[idx_HIPENC];
    telem_msg[idxHIP_POS8] = raw_readings[idx_HIPENC] >> 8;
    
    telem_msg[idxKNEE_POS16] = raw_readings[idx_KNEEENC];
    telem_msg[idxKNEE_POS8] = raw_readings[idx_KNEEENC] >> 8;
    
    return;
}

void updateCommandMessage(uint8_t cmd_msg[CMD_MESSAGE_SIZE])
{    
    command_itv_pressures[idxROT_ITV] = cmd_msg[idxROT_PRESSURE];
    command_itv_pressures[idxHIP_ITV] = cmd_msg[idxHIP_PRESSURE];
    command_itv_pressures[idxKNEE_ITV] = cmd_msg[idxKNEE_PRESSURE];
    
    command_hsppcv_duties[idxROT_VALVE] = cmd_msg[idxROT_DUTY];
    command_hsppcv_duties[idxHIP_VALVE] = cmd_msg[idxHIP_DUTY];
    command_hsppcv_duties[idxKNEE_VALVE] = cmd_msg[idxKNEE_DUTY];
    
    return;
}

void toROSControl(void *argument)
{
    commands_reception_status = xSemaphoreCreateMutex();
    
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = LOOP_FREQUENCY;
    xLastWakeTime = xTaskGetTickCount();
    
    while(1)
    {
        if(xSemaphoreTake(commands_reception_status,(TickType_t )50) == pdTRUE)
        {
            updateTelemetryMessage(telemetry_message,leg);

            UART1_Write((void*)telemetry_message,TELEM_MESSAGE_SIZE);
            
            xSemaphoreGive(commands_reception_status);

            vTaskDelayUntil(&xLastWakeTime,xFrequency / portTICK_PERIOD_MS);
        }
    }
}

void fromROSControl(void *argument)
{
    while(1)
    {
        if(xSemaphoreTake(commands_reception_status,(TickType_t)50) == pdTRUE)
        {
            if(UART1_Read((void*)command_message,CMD_MESSAGE_SIZE))
            {
                if(command_message[idx_SOF] == SOF && command_message[idx_EOF] == EOF)
                {
                    updateCommandMessage(command_message);
                }
                else
                {
                    command_itv_pressures[0] = 0;
                    command_itv_pressures[1] = 0;
                    command_itv_pressures[2] = 0;
                    
                    command_hsppcv_duties[0] = 50;
                    command_hsppcv_duties[1] = 50;
                    command_hsppcv_duties[2] = 50;
                    
                    UART1_ErrorClear();
                }
            }
            xSemaphoreGive(commands_reception_status);
        }
    }
}

void pressureControl(void *argument)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = ITVs_PRESSURIZATION_FREQUENCY;
    xLastWakeTime = xTaskGetTickCount();
    
    while(1)
    {
        pressurizeItvs(command_itv_pressures); 
        vTaskDelayUntil(&xLastWakeTime,xFrequency / portTICK_PERIOD_MS);
    }
}

void jointControl(void *argument)
{
    while(1)
    {
        hsppcvDutyCycleControl(command_hsppcv_duties);
    }
}