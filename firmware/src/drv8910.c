#include "drv8910.h"
#include "config/default/peripheral/spi/spi_master/plib_spi_master_common.h"
#include "config/default/peripheral/spi/spi_master/plib_spi3_master.h"

SPI_TRANSFER_SETUP s = {5000000,SPI_CLOCK_PHASE_LEADING_EDGE,SPI_CLOCK_POLARITY_IDLE_LOW,SPI_DATA_BITS_16};
/*--------------------------*/
/*APIs Implementation*/
/*--------------------------*/
bool COMMAND_Send(bool operation,uint8_t address,uint8_t data,bool driver_id)
{
    frame = operation << 14U | address << 8U | data;
    if(driver_id == DRIVER_1)
    {
        GPIO_PinWrite(GPIO_PIN_RB5,LOW);
        SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
        GPIO_PinWrite(GPIO_PIN_RB5,HIGH);
        //delay > 400ns
        return true;
    }
    else if(driver_id == DRIVER_2)
    {
        GPIO_PinWrite(GPIO_PIN_RB7,LOW);
        SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
        GPIO_PinWrite(GPIO_PIN_RB7,HIGH);  
        //delay > 400ns
        return true;
    }
    return false;
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
    return false;
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

bool MOTOR_Control(struct MOTOR motor)
{
    if(motor.motor_id > 4U || motor.driver_id > 1U)
    {
        return false;
    }
    else
    {
        if(motor.motor_id == PA0622_1 || motor.motor_id == PA0622_2)
        {
            COMMAND_Send(WRITE,PWM_DUTY_CH1_REG_ADDR,motor.duty_cycle,motor.driver_id);
            
            COMMAND_Send(WRITE,OP_CTRL_REG_1_ADDR,motor.action[OP_CTRL_REG_1],motor.driver_id);
            COMMAND_Send(WRITE,OP_CTRL_REG_2_ADDR,motor.action[OP_CTRL_REG_2],motor.driver_id);
            COMMAND_Send(WRITE,OP_CTRL_REG_3_ADDR,motor.action[OP_CTRL_REG_3],motor.driver_id);
        }
        else if(motor.motor_id == PA06180_1 || motor.motor_id == PA06180_2)
        {
            COMMAND_Send(WRITE,PWM_DUTY_CH2_REG_ADDR,motor.duty_cycle,motor.driver_id);
            
            COMMAND_Send(WRITE,OP_CTRL_REG_1_ADDR,motor.action[OP_CTRL_REG_1],motor.driver_id);
            COMMAND_Send(WRITE,OP_CTRL_REG_2_ADDR,motor.action[OP_CTRL_REG_2],motor.driver_id);
            COMMAND_Send(WRITE,OP_CTRL_REG_3_ADDR,motor.action[OP_CTRL_REG_3],motor.driver_id);
        }
        
        return true;
    }
    return false;
}