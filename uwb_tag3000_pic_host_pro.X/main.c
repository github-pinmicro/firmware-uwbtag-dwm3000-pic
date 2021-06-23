/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F18877
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/spi1.h"
#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include "mcc_generated_files/eusart.h"
#include "port.h"
#include "decawave.h"
#include "main.h"
#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_spi.h"
#include "port.h"
#include "pic_i2c.h"
#include "pic_control_fun.h"
#include "bq25895.h"
#include "mcc_generated_files/tmr0.h"
#include "mcc_generated_files/interrupt_manager.h"

bool timer0_triggered;
uint8_t ADDR[8] = {0x01, 0x00, 0x53, 0xFE, 0xFF, 0xD6, 0xF5, 0xC8};
uint8_t enable_sleep = 1;

uint16_t firmware_version[3] = {0x01, 0x01, 0x01};
/*
                         Main application
 */
void main(void)
{
    SYSTEM_Initialize();// initialize the device
    CLRWDT();
    #if (PRINT_LOG)
    EUSART_Initialize();
    printf_string("\n\rUWB_TAG_DWM3000_PIC");
    #endif

 
    /*while(1)
    { 
        CLRWDT();
        __delay_ms(500);
    }*/
    set_MSSSP1_mode(MSSP1_I2C);//PMIC init
    init_bq25895();
    __delay_ms(50);
 
    WUP_SetPushPull();
    WUP_SetLow();
    CS_SetPushPull();
    CS_SetHigh();
    RST_SetOpenDrain(); 
    RST_SetHigh();
   
    set_MSSSP1_mode(MSSP1_SPI);//DWM3000 init
    __delay_ms(2);
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    TMR0_Initialize();
    CLRWDT();
    dw_main();
    while(1)
    { 
        #if (PRINT_LOG)
        P_WD_Toggle();
        //printf_string("\n\rTMR_int");
        #endif 
        pmic_status_read(); 
        set_MSSSP1_mode(MSSP1_SPI);
        transmit_beacon_pkt();
        set_led_status();
        while(!P_SW_GetValue());
        pic_time_counter_ms += TX_INTERVAL_MS;
        INTERRUPT_GlobalInterruptEnable();
        if(1 == enable_sleep)
        {
            NOP();
            NOP();
            NOP();
            NOP();
            NOP();
            NOP();
            NOP();
            NOP();
            NOP();
            NOP(); 
            SLEEP();// PIC enter to sleep mode
            NOP();
            NOP();
            NOP();
            NOP();
            NOP();
            NOP();
        }       
        //__delay_ms(TX_INTERVAL_MS);
            
    }
}
//void read_uwb_reg(void)
//{
//   uint32_t read_data = 0;
//   uint16_t read_data1 = 0;
//   uint8_t read_data2 = 0;
////   dwt_write8bitoffsetreg(XTAL_ID, 0, 0x08);
//   read_data2 = dwt_read8bitoffsetreg(XTAL_ID, 0);
//   printf_string("\n\rXTAL_ID : ");
//   print_uint8_t(read_data2);
//   read_data2 = 0;
//   
//  // dwt_write32bitoffsetreg(SYS_CFG_ID, 0, 0x00000188);
//   read_data = dwt_read32bitoffsetreg(SYS_CFG_ID, 0);
//   printf_string("\n\rSYS_CFG_ID : ");
//   print_uint32_t(read_data);
//   read_data = 0;
//   
//   read_data1 = dwt_read16bitoffsetreg(STS_CONFIG_LO_ID, 2);
//   printf_string("\n\rSTS_CONFIG_LO_ID : ");
//   print_uint16_t(read_data1);
//   read_data1 = 0;
//   
//   read_data2 = dwt_read8bitoffsetreg(DTUNE0_ID, 0);
//   printf_string("\n\rDTUNE0_ID : ");
//   print_uint8_t(read_data2);
//   read_data2 = 0;
//   
//   read_data2 = dwt_read8bitoffsetreg(STS_CFG0_ID, 0);
//   printf_string("\n\rSTS_CFG0_ID : ");
//   print_uint8_t(read_data2);
//   read_data2 = 0;
//   
//   read_data = dwt_read32bitoffsetreg(DTUNE3_ID, 0);
//   printf_string("\n\rDTUNE3_ID : ");
//   print_uint32_t(read_data);
//   read_data = 0;
//   
//   read_data2 = dwt_read8bitoffsetreg(TX_FCTRL_HI_ID, 1);
//   printf_string("\n\rTX_FCTRL_HI_ID : ");
//   print_uint8_t(read_data2);
//   read_data2 = 0;
//   
//   read_data = dwt_read32bitoffsetreg(CHAN_CTRL_ID, 0);
//   printf_string("\n\rCHAN_CTRL_ID : ");
//   print_uint32_t(read_data);
//   read_data = 0;
//   
//   read_data = dwt_read32bitoffsetreg(TX_FCTRL_ID, 0);
//   printf_string("\n\rTX_FCTRL_ID : ");
//   print_uint32_t(read_data);
//   read_data = 0;
//   
//   read_data1 = dwt_read16bitoffsetreg(DTUNE0_ID, 2);
//   printf_string("\n\rDTUNE0_ID : ");
//   print_uint16_t(read_data1);
//   read_data1 = 0;
//   
//   read_data = dwt_read32bitoffsetreg(TX_CTRL_HI_ID, 0);
//   printf_string("\n\rTX_CTRL_HI_ID : ");
//   print_uint32_t(read_data);
//   read_data = 0;
//   
//   read_data1 = dwt_read16bitoffsetreg(PLL_CFG_ID, 0);
//   printf_string("\n\rPLL_CFG_ID : ");
//   print_uint16_t(read_data1);
//   read_data1 = 0;
//  
//   read_data = dwt_read32bitoffsetreg(RX_CTRL_HI_ID, 0);
//   printf_string("\n\rRX_CTRL_HI_ID : ");
//   print_uint32_t(read_data);
//   read_data = 0;
//   
//   read_data2 = dwt_read8bitoffsetreg(LDO_RLOAD_ID, 1);
//   printf_string("\n\rLDO_RLOAD_ID : ");
//   print_uint8_t(read_data2);
//   read_data2 = 0;
//   
//   read_data2 = dwt_read8bitoffsetreg(TX_CTRL_LO_ID, 2);
//   printf_string("\n\rTX_CTRL_LO_ID : ");
//   print_uint8_t(read_data2);
//   read_data2 = 0;
//   
//   read_data2 = dwt_read8bitoffsetreg(PLL_CAL_ID, 0);
//   printf_string("\n\rPLL_CAL_ID : ");
//   print_uint8_t(read_data2);
//   read_data2 = 0;
//   
//   read_data2 = dwt_read8bitoffsetreg(SYS_STATUS_ID, 0);
//   printf_string("\n\rSYS_STATUS_ID : ");
//   print_uint8_t(read_data2);
//   read_data2 = 0;
//   
//   read_data1 = dwt_read16bitoffsetreg(CLK_CTRL_ID, 0x0);
//   printf_string("\n\rCLK_CTRL_ID : ");
//   print_uint16_t(read_data1);
//   read_data1 = 0;
//  
//   read_data2 = dwt_read8bitoffsetreg(SEQ_CTRL_ID, 0x01);
//   printf_string("\n\rSEQ_CTRL_ID : ");
//   print_uint8_t(read_data2);
//   read_data2 = 0;
//}
#if (PRINT_LOG)
void print_uint8_t(uint8_t val)
{
   char print_data[20];
   sprintf(print_data,"val: %02X",val);
   printf_string(print_data);  
}
void print_uint16_t(uint16_t val)
{
  
   char print_data[20];
   sprintf(print_data,"val: %04X",val);
   printf_string(print_data);  
}

void print_uint32_t(uint32_t val)
{
    uint16_t values[2];
    char print_data[20];
    values[0] = (val >> 16) & 0xFFFF;
    values[1] = val & 0xFFFF;
    sprintf(print_data,"val: %04X%04X",values[0],values[1]);
    printf_string(print_data);
}
void print_uint64_t(uint64_t num)
{
    uint16_t numary[4];
    char print_data[50];
    numary[0] = (num.Msb >> 16) & 0xFFFF;
    numary[1] = num.Msb & 0xFFFF;
    numary[2] = (num.Lsb >> 16) & 0xFFFF;
    numary[3] = num.Lsb & 0xFFFF;

    sprintf(print_data,"\n\ruint64_num: %04X%04X%04X%04X", numary[0],numary[1],numary[2],numary[3]);
    printf_string(print_data);
}
#endif
void Sleep(uint32_t time)
{
    for(volatile uint32_t i=0; i<time; i++)
    {
        __delay_ms(1);
    }
}
/**
 End of File
*/