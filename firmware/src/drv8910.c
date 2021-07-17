#include "drv8910.h"
#include "config/default/peripheral/spi/spi_master/plib_spi_master_common.h"
#include "config/default/peripheral/spi/spi_master/plib_spi3_master.h"

SPI_TRANSFER_SETUP s = {120000000,SPI_CLOCK_PHASE_LEADING_EDGE,SPI_CLOCK_POLARITY_IDLE_LOW,SPI_DATA_BITS_16};

/*--------------------------*/
/*APIs Implementation*/
/*--------------------------*/
bool PWM_Config(void)
{
    //Driver 1
    frame = WRITE << 14U | PWM_CTRL_REG_1_ADDR << 8U | PWM_CTRL_REG_1;
    GPIO_PinWrite(GPIO_PIN_RB5,LOW);
    SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
    GPIO_PinWrite(GPIO_PIN_RB5,HIGH);
    //delay > 400ns
    frame = WRITE << 14U | PWM_CTRL_REG_2_ADDR << 8U | PWM_CTRL_REG_2;
    GPIO_PinWrite(GPIO_PIN_RB5,LOW);
    SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
    GPIO_PinWrite(GPIO_PIN_RB5,HIGH);
    //delay > 400ns
    
    //Driver 2
    frame = WRITE << 14U | PWM_CTRL_REG_1_ADDR << 8U | PWM_CTRL_REG_1;
    GPIO_PinWrite(GPIO_PIN_RB7,LOW);
    SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
    GPIO_PinWrite(GPIO_PIN_RB7,HIGH);
    //delay > 400ns
    frame = WRITE << 14U | PWM_CTRL_REG_2_ADDR << 8U | PWM_CTRL_REG_2;
    GPIO_PinWrite(GPIO_PIN_RB7,LOW);
    SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
    GPIO_PinWrite(GPIO_PIN_RB7,HIGH);
    //delay > 400ns
    
    return true;
}

bool FW_Config(bool enable)
{
    if(enable)
    {
       //Driver 1
       frame = WRITE << 14U | FW_CTRL_REG_1_ADDR << 8U | FW_CTRL_REG_1;  
       GPIO_PinWrite(GPIO_PIN_RB5,LOW);
       SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
       GPIO_PinWrite(GPIO_PIN_RB5,HIGH);
       //delay > 400ns
       frame = WRITE << 14U | FW_CTRL_REG_2_ADDR << 8U | FW_CTRL_REG_1;
       GPIO_PinWrite(GPIO_PIN_RB5,LOW);
       SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
       GPIO_PinWrite(GPIO_PIN_RB5,HIGH);
       //delay > 400ns
       
       //Driver 2
       frame = WRITE << 14U | FW_CTRL_REG_1_ADDR << 8U | FW_CTRL_REG_1;  
       GPIO_PinWrite(GPIO_PIN_RB7,LOW);
       SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
       GPIO_PinWrite(GPIO_PIN_RB7,HIGH);
       //delay > 400ns
       frame = WRITE << 14U | FW_CTRL_REG_2_ADDR << 8U | FW_CTRL_REG_2;  
       GPIO_PinWrite(GPIO_PIN_RB7,LOW);
       SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
       GPIO_PinWrite(GPIO_PIN_RB7,HIGH);
       //delay > 400ns
       return true;
    }
    else
    {
       //Driver 1
       frame = WRITE << 14U | FW_CTRL_REG_1_ADDR << 8U | ~FW_CTRL_REG_1;  
       GPIO_PinWrite(GPIO_PIN_RB5,LOW);
       SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
       GPIO_PinWrite(GPIO_PIN_RB5,HIGH);
       //delay > 400ns
       frame = WRITE << 14U | FW_CTRL_REG_2_ADDR << 8U | ~FW_CTRL_REG_1;
       GPIO_PinWrite(GPIO_PIN_RB5,LOW);
       SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
       GPIO_PinWrite(GPIO_PIN_RB5,HIGH);
       //delay > 400ns
       
       //Driver 2
       frame = WRITE << 14U | FW_CTRL_REG_1_ADDR << 8U | ~FW_CTRL_REG_1;  
       GPIO_PinWrite(GPIO_PIN_RB7,LOW);
       SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
       GPIO_PinWrite(GPIO_PIN_RB7,HIGH);
       //delay > 400ns
       frame = WRITE << 14U | FW_CTRL_REG_2_ADDR << 8U | ~FW_CTRL_REG_2;  
       GPIO_PinWrite(GPIO_PIN_RB7,LOW);
       SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
       GPIO_PinWrite(GPIO_PIN_RB7,HIGH);
       //delay > 400ns
       return true; 
    }
}

bool PWM_Map(void)
{
    
}

bool PWM_Write(uint8_t motor,uint8_t duty)
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
                frame = WRITE << 14U | PWM_DUTY_CH1_REG_ADDR << 8U | duty; 
                
                GPIO_PinWrite(GPIO_PIN_RB5,LOW);
                SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
                GPIO_PinWrite(GPIO_PIN_RB5,HIGH);
                //delay > 400ns 
                return true;
            case 2:
                frame = WRITE << 14U | PWM_DUTY_CH2_REG_ADDR << 8U | duty;
                
                GPIO_PinWrite(GPIO_PIN_RB5,LOW);
                SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
                GPIO_PinWrite(GPIO_PIN_RB5,HIGH);
                //delay > 400ns 
                return true;
            case 3:
                frame = WRITE << 14U | PWM_DUTY_CH1_REG_ADDR << 8U | duty; 
                
                GPIO_PinWrite(GPIO_PIN_RB7,LOW);
                SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
                GPIO_PinWrite(GPIO_PIN_RB7,HIGH);
                //delay > 400ns 
                return true;
            case 4:
                frame = WRITE << 14U | PWM_DUTY_CH2_REG_ADDR << 8U | duty; 
                
                GPIO_PinWrite(GPIO_PIN_RB7,LOW);
                SPI3_WriteRead((void*)frame,16U,(void*)response,16U);
                GPIO_PinWrite(GPIO_PIN_RB7,HIGH);
                //delay > 400ns 
                return true;
            default:
                return false;
        }
    }
}
