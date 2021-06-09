/*! ----------------------------------------------------------------------------
 * @file    port.c
 * @brief   HW specific definitions and functions for portability
 *
 * @attention
 *
 * Copyright 2016 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
 */

#include "port.h"
#include "main.h"
#include "deca_device_api.h"
#include "mcc_generated_files/pin_manager.h"


/****************************************************************************//**
 *
 *                              APP global variables
 *
 *******************************************************************************/


/****************************************************************************//**
 *
 *                  Port private variables and function prototypes
 *
 *******************************************************************************/

/****************************************************************************//**
 *
 *                              Time section
 *
 *******************************************************************************/

/* @fn    portGetTickCnt
 * @brief wrapper for to read a SysTickTimer, which is incremented with
 *        CLOCKS_PER_SEC frequency.
 *        The resolution of time32_incr is usually 1/1000 sec.
 * */



/* @fn    usleep
 * @brief precise usleep() delay
 * */
/*int usleep(useconds_t usec)
{
    int i,j;
#pragma GCC ivdep
    for(i=0;i<usec;i++)
    {
#pragma GCC ivdep
        for(j=0;j<2;j++)
        {
            __NOP();
            __NOP();
        }
    }
    return 0;
}
*/

/* @fn    Sleep  __INLINE
 * @brief Sleep delay in ms using SysTick timer
 * */
//void Sleep(uint32_t x)
//{
//    //__delay_ms(x);
//    sleep_delay(x);
//}

/****************************************************************************//**
 *
 *                              END OF Time section
 *
 *******************************************************************************/

/****************************************************************************//**
 *
 *                              Configuration section
 *
 *******************************************************************************/
/* @fn    peripherals_init
 * */
int peripherals_init (void)
{
    /* All has been initialized in the CubeMx code, see main.c */
    return 0;
}


/* @fn    spi_peripheral_init
 * */
void spi_peripheral_init()
{

    /* SPI's has been initialized in the CubeMx code, see main.c */

    port_LCD_RS_clear();

    port_LCD_RW_clear();
}




/****************************************************************************//**
 *
 *                          End of configuration section
 *
 *******************************************************************************/

/****************************************************************************//**
 *
 *                          DW1000 port section
 *
 *******************************************************************************/

/* @fn      reset_DW1000
 * @brief   DW_RESET pin on DW1000 has 2 functions
 *          In general it is output, but it also can be used to reset the digital
 *          part of DW1000 by driving this pin low.
 *          Note, the DW_RESET pin should not be driven high externally.
 * */
void reset_DWIC(void)
{
    RST_SetLow();
    Sleep(1);
    RST_SetHigh();
    Sleep(2);
}

/* @fn      setup_DW1000RSTnIRQ
 * @brief   setup the DW_RESET pin mode
 *          0 - output Open collector mode
 *          !0 - input mode with connected EXTI0 IRQ
 * */
void setup_DWICRSTnIRQ(int enable)
{
    RST_SetOpenDrain(); 
    RST_SetHigh();
}

/*! ------------------------------------------------------------------------------------------------------------------
* @fn wakeup_device_with_io()
*
* @brief This function wakes up the device by toggling io with a delay.
*
* input None
*
* output -None
*
*/
void wakeup_device_with_io(void)
{
	WUP_SetHigh();
	Sleep(1);
    WUP_SetLow();
}   
/* @fn      port_is_boot1_low
 * @brief   check the BOOT1 pin status.
 * @return  1 if ON and 0 for OFF
 * */
int port_is_boot1_low(void)
{
    //return ((GPIO_ReadInputDataBit(TA_BOOT1_GPIO, TA_BOOT1))?(0):(1));
}

/* @fn      port_is_boot1_low
 * @brief   check the BOOT1 pin status.
 * @return  1 if ON and 0 for OFF
 * */
int port_is_boot1_on(uint16_t x)
{
    //return ((GPIO_ReadInputDataBit(TA_BOOT1_GPIO, TA_BOOT1))?(0):(1));
}

/* @fn      port_is_switch_on
 * @brief   check the switch status.
 *          when switch (S1) is 'on' the pin is low
 * @return  1 if ON and 0 for OFF
 * */
int port_is_switch_on(uint16_t GPIOpin)
{
    //return ((GPIO_ReadInputDataBit(TA_SW1_GPIO, GPIOpin))?(0):(1));
}

/* @fn      port_wakeup_dw1000
 * @brief   "slow" waking up of DW1000 using DW_CS only
 * */
void port_wakeup_dw_ic(void)
{
    CS_SetLow();
    Sleep(1);
    CS_SetHigh();
    Sleep(7);                       //wait 7ms for DW1000 XTAL to stabilise
}
/* @fn      port_set_dw_ic_spi_slowrate
 * @brief   set 2.25MHz
 *          note: hspi3 is clocked from 72MHz
 * */
void port_set_dw_ic_spi_slowrate(void)
{
//	hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
//	HAL_SPI_Init(&hspi3);
}

/* @fn      port_set_dw_ic_spi_fastrate
 * @brief   set 18MHz
 *          note: hspi3 is clocked from 72MHz
 * */
void port_set_dw_ic_spi_fastrate(void)
{
//	hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
//	HAL_SPI_Init(&hspi3);
}

/* @fn      port_LCD_RS_set
 * @brief   wrapper to set LCD_RS pin
 * */

/****************************************************************************//**
 *
 *                          End APP port section
 *
 *******************************************************************************/



/****************************************************************************//**
 *
 *                              IRQ section
 *
 *******************************************************************************/




/****************************************************************************//**
 *
 *                              END OF IRQ section
 *
 *******************************************************************************/



/****************************************************************************//**
 *
 *                              USB report section
 *
 *******************************************************************************/

/* DW1000 IRQ handler definition. */

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn port_set_deca_isr()
 *
 * @brief This function is used to install the handling function for DW1000 IRQ.
 *
 * NOTE:
 *   - As EXTI9_5_IRQHandler does not check that port_deca_isr is not null, the user application must ensure that a
 *     proper handler is set by calling this function before any DW1000 IRQ occurs!
 *   - This function makes sure the DW1000 IRQ line is deactivated while the handler is installed.
 *
 * @param deca_isr function pointer to DW1000 interrupt handler to install
 *
 * @return none
 */



/****************************************************************************//**
 *
 *******************************************************************************/

