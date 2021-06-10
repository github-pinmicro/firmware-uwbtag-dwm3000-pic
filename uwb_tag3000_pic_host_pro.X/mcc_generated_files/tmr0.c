/**
  TMR0 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr0.c

  @Summary
    This is the generated driver implementation file for the TMR0 driver using MPLAB(c) Code Configurator

  @Description
    This source file provides APIs for TMR0.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15
        Device            :  PIC16F18877
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "tmr0.h"
#include "mcc.h"
#include "../main.h"
#include "../pic_control_fun.h"
volatile uint16_t timer0ReloadVal;
/**
  Section: TMR0 APIs
*/

void TMR0_Initialize(void)
{
    // Set TMR0 to the options selected in the User Interface

    // T0OUTPS 1:1; T0EN disabled; T016BIT 8-bit; 
    T0CON0 = 0x01;

    // T0CS LFINTOSC; T0CKPS 1:512; T0ASYNC synchronised; 
    //T0CON1 = 0x89;
    T0CON1 = 0x97;
    
    // TMR0H 29; 
    TMR0H = ((TX_INTERVAL_MS / 100)*11)+(TX_INTERVAL_MS / 100);//0x1D;

    // TMR0L 0; 
    TMR0L = 0;//0x00;
    //timer0ReloadVal = 1280;
    timer0ReloadVal = (uint16_t) (TMR0H << 8) | TMR0L;
    // Clear Interrupt flag before enabling the interrupt
    PIR0bits.TMR0IF = 0;

    // Enabling TMR0 interrupt.
    PIE0bits.TMR0IE = 1;

    // Set Default Interrupt Handler
    //TMR0_SetInterruptHandler(TMR0_DefaultInterruptHandler);

    // Start TMR0
    //TMR0_StartTimer();
    T0CON0bits.T0EN = 1;
}

void TMR0_StartTimer(void)
{
    // Start the Timer by writing to TMR0ON bit
    T0CON0bits.T0EN = 1;
}

void TMR0_StopTimer(void)
{
    // Stop the Timer by writing to TMR0ON bit
    T0CON0bits.T0EN = 0;
}
void TMR0_Reload(void)
{
    //Write to the Timer1 register
    TMR0H = (timer0ReloadVal >> 8);
    TMR0L = timer0ReloadVal;
}
void TMR0_ISR(void)
{
    // clear the TMR0 interrupt flag
    //PIR0bits.TMR0IF = 0;
    CLRWDT();
}

/**
  End of File
*/