/*! ----------------------------------------------------------------------------
 * @file    deca_spi.c
 * @brief   SPI access functions
 *
 * @attention
 *
 * Copyright 2015-2020 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
 */
#include "main.h"
#include "mcc_generated_files/spi1.h"
#include "deca_spi.h"
#include "deca_device_api.h"
#include "port.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/eusart.h"

   /*clocked from 72MHz*/


/****************************************************************************//**
 *
 *                              DW1000 SPI section
 *
 *******************************************************************************/
/*! ------------------------------------------------------------------------------------------------------------------
 * Function: openspi()
 *
 * Low level abstract function to open and initialise access to the SPI device.
 * returns 0 for success, or -1 for error
 */
int openspi(/*SPI_TypeDef* SPIx*/)
{
    return 0;
} // end openspi()

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: closespi()
 *
 * Low level abstract function to close the the SPI device.
 * returns 0 for success, or -1 for error
 */
int closespi(void)
{
    return 0;
} // end closespi()




/*! ------------------------------------------------------------------------------------------------------------------
 * Function: writetospiwithcrc()
 *
 * Low level abstract function to write to the SPI when SPI CRC mode is used
 * Takes two separate byte buffers for write header and write data, and a CRC8 byte which is written last
 * returns 0 for success, or -1 for error
 */
int writetospiwithcrc(
                uint16_t      headerLength,
                const uint8_t *headerBuffer,
                uint32_t      bodyLength,
                const uint8_t *bodyBuffer,
                uint8_t       crc8)
{
/**< Put chip select line low */
    CS_SetLow();
    SPI1_WriteBlock((uint8_t *)headerBuffer, headerLength);
    SPI1_WriteBlock((uint8_t *)bodyBuffer, bodyLength);
    SPI1_WriteBlock((uint8_t *)&crc8, 1);
    CS_SetHigh();
    return 0;
} // end writetospiwithcrc()


/*! ------------------------------------------------------------------------------------------------------------------
 * Function: writetospi()
 *
 * Low level abstract function to write to the SPI
 * Takes two separate byte buffers for write header and write data
 * returns 0 for success, or -1 for error
 */
int writetospi(uint16_t       headerLength,
               const uint8_t  *headerBuffer,
               uint32_t       bodyLength,
               const uint8_t  *bodyBuffer)
{
    CS_SetLow();
    SPI1_WriteBlock((uint8_t *)headerBuffer, headerLength);
    SPI1_WriteBlock((uint8_t *)bodyBuffer, bodyLength);
    CS_SetHigh();
    return 0;
} // end writetospi()
/*! ------------------------------------------------------------------------------------------------------------------
 * Function: readfromspi()
 *
 * Low level abstract function to read from the SPI
 * Takes two separate byte buffers for write header and read data
 * returns the offset into read buffer where first byte of read data may be found,
 * or returns -1 if there was an error
 */
int readfromspi(uint16_t  headerLength,
				const uint8_t   *headerBuffer,
                uint32_t  readlength,
                uint8_t   *readBuffer)
{
    CS_SetLow();
    
    SPI1_WriteBlock((uint8_t *)headerBuffer, headerLength);
   
    SPI1_ReadBlock(readBuffer, readlength);
   
    CS_SetHigh();

    return 0;
} // end readfromspi()
/*! ------------------------------------------------------------------------------------------------------------------
* @fn spi_cs_low_delay()
*
* @brief This function sets the CS to '0' for ms delay and than raises it up
*
* input parameters:
* @param ms_delay - The delay for CS to be in '0' state
*
* no return value
*/
uint16_t spi_cs_low_delay(uint16_t delay_ms)
{
	/* Blocking: Check whether previous transfer has been finished */

	/* Process Locked */
    CS_SetLow();/**< Put chip select line low */
	Sleep(delay_ms);
    CS_SetHigh(); /**< Put chip select line high */
	/* Process Unlocked */
	return 0;
}


/****************************************************************************//**
 *
 *                              END OF DW1000 SPI section
 *
 *******************************************************************************/


