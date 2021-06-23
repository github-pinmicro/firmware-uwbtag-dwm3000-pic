/**
  Generated Pin Manager File

  Company:
    Microchip Technology Inc.

  File Name:
    pin_manager.c

  Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F18877
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
        MPLAB             :  MPLAB X 5.45

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.
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

#include "pin_manager.h"
#include "stdbool.h"
#include "../main.h"
void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */
    LATE = 0x00;
    LATD = 0x00;
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;

    /**
    TRISx registers
    */
    TRISE = 0x07;
    TRISA = 0x07;
    TRISB = 0xEF;
    TRISC = 0xDF;
    TRISD = 0xF0;

    /**
    ANSELx registers
    */
    ANSELD = 0xF0;
    ANSELC = 0x00;
    ANSELB = 0xFE;
    ANSELE = 0x07;
    ANSELA = 0x07;

    /**
    WPUx registers
    */
    WPUD = 0x00;
    WPUE = 0x00;
    WPUB = 0x00;
    WPUA = 0x00;
    WPUC = 0x18;

    /**
    ODx registers
    */
    ODCONE = 0x00;
    ODCONA = 0x00;
    ODCONB = 0x00;
    ODCONC = 0x00;
    ODCOND = 0x00;
#if (1)
    bool state = (bool) GIE;
    GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS
    
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC7 = 1; 
    RXPPS = 0x17;   //RC7->EUSART:RX; 
    RC5PPS = 0x10;   //RC5->EUSART:TX;
    
    /*SSP1CLKPPS = 0x12;   //RC2->MSSP1:SCK1;    
    RC1PPS = 0x15;   //RC1->MSSP1:SDO1;    
    RC2PPS = 0x14;   //RC2->MSSP1:SCK1;    
    SSP1DATPPS = 0x10;   //RC0->MSSP1:SDI1;*/   

    /*RC3PPS = 0x14;   //RC3->MSSP1:SCL1;
    RC4PPS = 0x15;   //RC4->MSSP1:SDA1;
    SSP1CLKPPS = 0x13;   //RC3->MSSP1:SCL1;
    SSP1DATPPS = 0x14;   //RC4->MSSP1:SDA1;*/
    

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS
    GIE = state;
#endif
    //TRISCbits.TRISC6 = 0;
    EN_3V0_SetLow();
    //EV_3V3_SetHigh();
    EV_3V3_SetDigitalInput();
    
    LED_R1_SetLow();
    LED_G1_SetLow();
    LED_B1_SetLow();
    Sleep(300);
    LED_R1_SetHigh();
    LED_G1_SetHigh();
    LED_B1_SetHigh(); 
}
void PIN_MANAGER_IOC(void)
{   
}

/**
 End of File
*/