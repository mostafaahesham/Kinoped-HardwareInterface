#include "drv8910.h"
#include "config/default/peripheral/spi/spi_master/plib_spi_master_common.h"
#include "config/default/peripheral/spi/spi_master/plib_spi3_master.h"

SPI_TRANSFER_SETUP s = {5000000,SPI_CLOCK_PHASE_LEADING_EDGE,SPI_CLOCK_POLARITY_IDLE_LOW,SPI_DATA_BITS_16};

/*--------------------------*/
/*APIs Implementation*/
/*--------------------------*/
bool COMMAND_Send(bool operation,uint8_t address,uint8_t data,bool device_id)
{
    frame = operation << 14U | address << 8U | data;
    if(device_id == DRIVER_1)
    {
        GPIO_PinWrite(GPIO_PIN_RB5,LOW);
        SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
        GPIO_PinWrite(GPIO_PIN_RB5,HIGH);
        //delay > 400ns
        return true;
    }
    else if(device_id == DRIVER_2)
    {
        GPIO_PinWrite(GPIO_PIN_RB7,LOW);
        SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
        GPIO_PinWrite(GPIO_PIN_RB7,HIGH);  
        //delay > 400ns
        return true;
    }
}

bool PWM_Config(void)
{
    //Driver 1
    COMMAND_Send(WRITE,PWM_CTRL_REG_1_ADDR,PWM_CTRL_REG_1,DRIVER_1);
    COMMAND_Send(WRITE,PWM_CTRL_REG_2_ADDR,PWM_CTRL_REG_2,DRIVER_1);
    
    //Driver 2
    COMMAND_Send(WRITE,PWM_CTRL_REG_1_ADDR,PWM_CTRL_REG_1,DRIVER_2);
    COMMAND_Send(WRITE,PWM_CTRL_REG_2_ADDR,PWM_CTRL_REG_2,DRIVER_2);
    
    return true;
}

bool FW_Config(bool enable)
{
    if(enable)
    {
        //Driver 1
        COMMAND_Send(WRITE,FW_CTRL_REG_1_ADDR,FW_CTRL_REG_1,DRIVER_1);
        COMMAND_Send(WRITE,FW_CTRL_REG_2_ADDR,FW_CTRL_REG_2,DRIVER_1);

        //Driver 2
        COMMAND_Send(WRITE,FW_CTRL_REG_1_ADDR,FW_CTRL_REG_1,DRIVER_2);
        COMMAND_Send(WRITE,FW_CTRL_REG_2_ADDR,FW_CTRL_REG_2,DRIVER_2);
  
        return true;
    }
    else
    {
        //Driver 1
        COMMAND_Send(WRITE,FW_CTRL_REG_1_ADDR,~FW_CTRL_REG_1,DRIVER_1);
        COMMAND_Send(WRITE,FW_CTRL_REG_2_ADDR,~FW_CTRL_REG_2,DRIVER_1);

        //Driver 2
        COMMAND_Send(WRITE,FW_CTRL_REG_1_ADDR,~FW_CTRL_REG_1,DRIVER_2);
        COMMAND_Send(WRITE,FW_CTRL_REG_2_ADDR,~FW_CTRL_REG_2,DRIVER_2);
        
        return true; 
    }
}

bool PWM_Map(void)
{
    //Driver 1
    COMMAND_Send(WRITE,PWM_MAP_CTRL_REG_1_ADDR,PWM_MAP_CTRL_REG_1,DRIVER_1);
    COMMAND_Send(WRITE,PWM_MAP_CTRL_REG_2_ADDR,PWM_MAP_CTRL_REG_2,DRIVER_1);

    //Driver 2
    COMMAND_Send(WRITE,PWM_MAP_CTRL_REG_1_ADDR,PWM_MAP_CTRL_REG_1,DRIVER_2);
    COMMAND_Send(WRITE,PWM_MAP_CTRL_REG_2_ADDR,PWM_MAP_CTRL_REG_2,DRIVER_2);
    
    return true;
}

bool PWM_Freq(void)
{
    //Driver 1
    COMMAND_Send(WRITE,PWM_FREQ_CTRL_REG_ADDR,PWM_FREQ_CTRL_REG,DRIVER_1);
    
    //Driver 2
    COMMAND_Send(WRITE,PWM_FREQ_CTRL_REG_ADDR,PWM_FREQ_CTRL_REG,DRIVER_2);
    
    return true;
}

bool PWM_Duty(uint8_t motor,uint8_t duty)
{
    if(motor > 4U)
    {
       return false; 
    }
    else
    {
        CALCULATE_DUTY(duty);
        switch (motor)
        {
            case 1U:
                COMMAND_Send(WRITE,PWM_DUTY_CH1_REG_ADDR,duty_cycle,DRIVER_1);
                return true;
            case 2:
                COMMAND_Send(WRITE,PWM_DUTY_CH2_REG_ADDR,duty_cycle,DRIVER_1);
                return true;
            case 3:
                COMMAND_Send(WRITE,PWM_DUTY_CH1_REG_ADDR,duty_cycle,DRIVER_2);
                return true;
            case 4:
                COMMAND_Send(WRITE,PWM_DUTY_CH2_REG_ADDR,duty_cycle,DRIVER_2);
                return true;
            default:
                return false;
        }
    }
}

