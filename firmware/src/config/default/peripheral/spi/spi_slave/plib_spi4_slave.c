/*******************************************************************************
  SPI PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spi4_slave.c

  Summary:
    SPI4 Slave Source File

  Description:
    This file has implementation of all the interfaces provided for particular
    SPI peripheral.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019-2020 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
#include "plib_spi4_slave.h"
#include "peripheral/gpio/plib_gpio.h"
#include <string.h>
// *****************************************************************************
// *****************************************************************************
// Section: SPI4 Slave Implementation
// *****************************************************************************
// *****************************************************************************
#define SPI4_CS_PIN                      GPIO_PIN_RB6


#define SPI4_READ_BUFFER_SIZE            256
#define SPI4_WRITE_BUFFER_SIZE           256

static uint8_t SPI4_ReadBuffer[SPI4_READ_BUFFER_SIZE];
static uint8_t SPI4_WriteBuffer[SPI4_WRITE_BUFFER_SIZE];


/* Global object to save SPI Exchange related data */
SPI_SLAVE_OBJECT spi4Obj;

#define SPI4_CON_CKP                        (0 << _SPI4CON_CKP_POSITION)
#define SPI4_CON_CKE                        (1 << _SPI4CON_CKE_POSITION)
#define SPI4_CON_MODE_32_MODE_16            (0 << _SPI4CON_MODE16_POSITION)
#define SPI4_CON_ENHBUF                     (1 << _SPI4CON_ENHBUF_POSITION)
#define SPI4_CON_STXISEL                    (3 << _SPI4CON_STXISEL_POSITION)
#define SPI4_CON_SRXISEL                    (1 << _SPI4CON_SRXISEL_POSITION)
#define SPI4_CON_SSEN                       (1 << _SPI4CON_SSEN_POSITION)

#define SPI4_ENABLE_RX_INT()                IEC6SET = 0x40000000
#define SPI4_CLEAR_RX_INT_FLAG()            IFS6CLR = 0x40000000

#define SPI4_DISABLE_TX_INT()               IEC6CLR = 0x80000000L
#define SPI4_ENABLE_TX_INT()                IEC6SET = 0x80000000L
#define SPI4_CLEAR_TX_INT_FLAG()            IFS6CLR = 0x80000000L

#define SPI4_ENABLE_ERR_INT()               IEC6SET = 0x20000000
#define SPI4_CLEAR_ERR_INT_FLAG()           IFS6CLR = 0x20000000

/* Forward declarations */
static void SPI4_CS_Handler(GPIO_PIN pin, uintptr_t context);

void SPI4_Initialize ( void )
{
    /* Disable SPI4 Interrupts */
    IEC6CLR = 0x20000000;
    IEC6CLR = 0x40000000;
    IEC6CLR = 0x80000000L;

    /* STOP and Reset the SPI */
    SPI4CON = 0;

    /* Clear SPI4 Interrupt flags */
    IFS6CLR = 0x20000000;
    IFS6CLR = 0x40000000;
    IFS6CLR = 0x80000000L;

    /* CLear the receiver overflow error flag */
    SPI4STATCLR = _SPI4STAT_SPIROV_MASK;

    /*
    SRXISEL = 1 (Receive buffer is not empty)
    STXISEL = 3 (Transmit buffer is not full)
    MSTEN = 0
    CKP = 0
    CKE = 1
    MODE<32,16> = 0
    ENHBUF = 1
    */

    SPI4CONSET = (SPI4_CON_ENHBUF | SPI4_CON_MODE_32_MODE_16 | SPI4_CON_CKE | SPI4_CON_CKP | SPI4_CON_SSEN | SPI4_CON_STXISEL | SPI4_CON_SRXISEL);

    /* Enable generation of interrupt on receiver overflow */
    SPI4CON2SET = _SPI4CON2_SPIROVEN_MASK;

    spi4Obj.rdInIndex = 0;
    spi4Obj.wrOutIndex = 0;
    spi4Obj.nWrBytes = 0;
    spi4Obj.errorStatus = SPI_SLAVE_ERROR_NONE;
    spi4Obj.callback = NULL ;
    spi4Obj.transferIsBusy = false ;
    spi4Obj.csInterruptPending = false;
    spi4Obj.rxInterruptActive = false;


    /* Register callback and enable notifications on Chip Select logic level change */
    GPIO_PinInterruptCallbackRegister(SPI4_CS_PIN, SPI4_CS_Handler, (uintptr_t)NULL);
    GPIO_PinInterruptEnable(SPI4_CS_PIN);

    /* Enable SPI4 RX and Error Interrupts. TX interrupt will be enabled when a SPI write is submitted. */
    SPI4_ENABLE_RX_INT();
    SPI4_ENABLE_ERR_INT();

    /* Enable SPI4 */
    SPI4CONSET = _SPI4CON_ON_MASK;
}

/* For 16-bit/32-bit mode, the "size" must be specified in terms of 16-bit/32-bit words */
size_t SPI4_Read(void* pRdBuffer, size_t size)
{
    size_t rdSize = size;
    uint32_t rdInIndex = spi4Obj.rdInIndex;

    if (rdSize > rdInIndex)
    {
        rdSize = rdInIndex;
    }

    memcpy(pRdBuffer, SPI4_ReadBuffer, rdSize);

    return rdSize;
}

/* For 16-bit/32-bit mode, the "size" must be specified in terms of 16-bit/32-bit words */
size_t SPI4_Write(void* pWrBuffer, size_t size )
{
    size_t wrSize = size;

    SPI4_DISABLE_TX_INT();

    if (wrSize > SPI4_WRITE_BUFFER_SIZE)
    {
        wrSize = SPI4_WRITE_BUFFER_SIZE;
    }

    memcpy(SPI4_WriteBuffer, pWrBuffer, wrSize);

    spi4Obj.nWrBytes = wrSize;
    spi4Obj.wrOutIndex = 0;

    /* Fill up the FIFO as long as there are empty elements */
    while ((!(SPI4STAT & _SPI4STAT_SPITBF_MASK)) && (spi4Obj.wrOutIndex < spi4Obj.nWrBytes))
    {
        SPI4BUF = SPI4_WriteBuffer[spi4Obj.wrOutIndex++];
    }

    /* Enable TX interrupt */
    SPI4_ENABLE_TX_INT();

    return wrSize;
}

/* For 16-bit/32-bit mode, the return value is in terms of 16-bit/32-bit words */
size_t SPI4_ReadCountGet(void)
{
    return spi4Obj.rdInIndex;
}

/* For 16-bit/32-bit mode, the return value is in terms of 16-bit/32-bit words */
size_t SPI4_ReadBufferSizeGet(void)
{
    return SPI4_READ_BUFFER_SIZE;
}

/* For 16-bit/32-bit mode, the return value is in terms of 16-bit/32-bit words */
size_t SPI4_WriteBufferSizeGet(void)
{
    return SPI4_WRITE_BUFFER_SIZE;
}

void SPI4_CallbackRegister(SPI_SLAVE_CALLBACK callBack, uintptr_t context )
{
    spi4Obj.callback = callBack;

    spi4Obj.context = context;
}

/* The status is returned as busy when CS is asserted */
bool SPI4_IsBusy(void)
{
    return spi4Obj.transferIsBusy;
}


SPI_SLAVE_ERROR SPI4_ErrorGet(void)
{
    SPI_SLAVE_ERROR errorStatus = spi4Obj.errorStatus;

    spi4Obj.errorStatus = SPI_SLAVE_ERROR_NONE;

    return errorStatus;
}

static void SPI4_CS_Handler(GPIO_PIN pin, uintptr_t context)
{
    bool activeState = 0;

    if (GPIO_PinRead((GPIO_PIN)SPI4_CS_PIN) == activeState)
    {
        /* CS is asserted */
        spi4Obj.transferIsBusy = true;

    }
    else
    {
        /* Give application callback only if RX interrupt is not preempted and RX interrupt is not pending to be serviced */

        if ((spi4Obj.rxInterruptActive == false) && ((IFS6 & _IFS6_SPI4RXIF_MASK) == 0))
        {
            /* CS is de-asserted */
            spi4Obj.transferIsBusy = false;

            spi4Obj.wrOutIndex = 0;
            spi4Obj.nWrBytes = 0;

            if(spi4Obj.callback != NULL)
            {
                spi4Obj.callback(spi4Obj.context);
            }

            /* Clear the read index. Application must read out the data by calling SPI4_Read API in the callback */
            spi4Obj.rdInIndex = 0;
        }
        else
        {
            /* If CS interrupt is serviced by either preempting the RX interrupt or RX interrupt is pending to be serviced,
             * then delegate the responsibility of giving application callback to the RX interrupt handler */

            spi4Obj.csInterruptPending = true;
        }
    }
}

void SPI4_FAULT_InterruptHandler (void)
{
    spi4Obj.errorStatus = (SPI4STAT & _SPI4STAT_SPIROV_MASK);

    /* Clear the receive overflow flag */
    SPI4STATCLR = _SPI4STAT_SPIROV_MASK;

    SPI4_CLEAR_ERR_INT_FLAG();
}

void SPI4_TX_InterruptHandler (void)
{
    /* Fill up the FIFO as long as there are empty elements */
    while ((!(SPI4STAT & _SPI4STAT_SPITBF_MASK)) && (spi4Obj.wrOutIndex < spi4Obj.nWrBytes))
    {
        SPI4BUF = SPI4_WriteBuffer[spi4Obj.wrOutIndex++];
    }

    /* Clear the transmit interrupt flag */
    SPI4_CLEAR_TX_INT_FLAG();

    if (spi4Obj.wrOutIndex == spi4Obj.nWrBytes)
    {
        /* Nothing to transmit. Disable transmit interrupt. The last byte sent by the master will be shifted out automatically*/
        SPI4_DISABLE_TX_INT();
    }
}

void SPI4_RX_InterruptHandler (void)
{
    uint32_t receivedData = 0;

    spi4Obj.rxInterruptActive = true;

    while (!(SPI4STAT & _SPI4STAT_SPIRBE_MASK))
    {
        /* Receive buffer is not empty. Read the received data. */
        receivedData = SPI4BUF;

        if (spi4Obj.rdInIndex < SPI4_READ_BUFFER_SIZE)
        {
            SPI4_ReadBuffer[spi4Obj.rdInIndex++] = receivedData;
        }
    }

    /* Clear the receive interrupt flag */
    SPI4_CLEAR_RX_INT_FLAG();

    spi4Obj.rxInterruptActive = false;

    /* Check if CS interrupt occured before the RX interrupt and that CS interrupt delegated the responsibility to give
     * application callback to the RX interrupt */

    if (spi4Obj.csInterruptPending == true)
    {
        spi4Obj.csInterruptPending = false;
        spi4Obj.transferIsBusy = false;

        spi4Obj.wrOutIndex = 0;
        spi4Obj.nWrBytes = 0;

        if(spi4Obj.callback != NULL)
        {
            spi4Obj.callback(spi4Obj.context);
        }

        /* Clear the read index. Application must read out the data by calling SPI4_Read API in the callback */
        spi4Obj.rdInIndex = 0;
    }
}

