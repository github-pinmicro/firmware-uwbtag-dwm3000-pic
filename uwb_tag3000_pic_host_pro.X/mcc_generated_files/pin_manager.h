/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F18877
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
        MPLAB 	          :  MPLAB X 5.45	
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set WUP aliases
#define WUP_TRIS                 TRISAbits.TRISA6
#define WUP_LAT                  LATAbits.LATA6
#define WUP_PORT                 PORTAbits.RA6
#define WUP_WPU                  WPUAbits.WPUA6
#define WUP_OD                   ODCONAbits.ODCA6
#define WUP_ANS                  ANSELAbits.ANSA6
#define WUP_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define WUP_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define WUP_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define WUP_GetValue()           PORTAbits.RA6
#define WUP_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define WUP_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)
#define WUP_SetPullup()          do { WPUAbits.WPUA6 = 1; } while(0)
#define WUP_ResetPullup()        do { WPUAbits.WPUA6 = 0; } while(0)
#define WUP_SetPushPull()        do { ODCONAbits.ODCA6 = 0; } while(0)
#define WUP_SetOpenDrain()       do { ODCONAbits.ODCA6 = 1; } while(0)
#define WUP_SetAnalogMode()      do { ANSELAbits.ANSA6 = 1; } while(0)
#define WUP_SetDigitalMode()     do { ANSELAbits.ANSA6 = 0; } while(0)

// get/set CS aliases
#define CS_TRIS                 TRISAbits.TRISA7
#define CS_LAT                  LATAbits.LATA7
#define CS_PORT                 PORTAbits.RA7
#define CS_WPU                  WPUAbits.WPUA7
#define CS_OD                   ODCONAbits.ODCA7
#define CS_ANS                  ANSELAbits.ANSA7
#define CS_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define CS_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define CS_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define CS_GetValue()           PORTAbits.RA7
#define CS_SetDigitalInput()    do { TRISAbits.TRISA7 = 1; } while(0)
#define CS_SetDigitalOutput()   do { TRISAbits.TRISA7 = 0; } while(0)
#define CS_SetPullup()          do { WPUAbits.WPUA7 = 1; } while(0)
#define CS_ResetPullup()        do { WPUAbits.WPUA7 = 0; } while(0)
#define CS_SetPushPull()        do { ODCONAbits.ODCA7 = 0; } while(0)
#define CS_SetOpenDrain()       do { ODCONAbits.ODCA7 = 1; } while(0)
#define CS_SetAnalogMode()      do { ANSELAbits.ANSA7 = 1; } while(0)
#define CS_SetDigitalMode()     do { ANSELAbits.ANSA7 = 0; } while(0)

// get/set RST aliases
#define RST_TRIS                 TRISBbits.TRISB4
#define RST_LAT                  LATBbits.LATB4
#define RST_PORT                 PORTBbits.RB4
#define RST_WPU                  WPUBbits.WPUB4
#define RST_OD                   ODCONBbits.ODCB4
#define RST_ANS                  ANSELBbits.ANSB4
#define RST_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define RST_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define RST_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define RST_GetValue()           PORTBbits.RB4
#define RST_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define RST_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define RST_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define RST_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define RST_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define RST_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define RST_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define RST_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set RC0 procedures
#define RC0_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define RC0_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define RC0_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define RC0_GetValue()              PORTCbits.RC0
#define RC0_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define RC0_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define RC0_SetPullup()             do { WPUCbits.WPUC0 = 1; } while(0)
#define RC0_ResetPullup()           do { WPUCbits.WPUC0 = 0; } while(0)
#define RC0_SetAnalogMode()         do { ANSELCbits.ANSC0 = 1; } while(0)
#define RC0_SetDigitalMode()        do { ANSELCbits.ANSC0 = 0; } while(0)

// get/set RC1 procedures
#define RC1_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define RC1_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define RC1_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define RC1_GetValue()              PORTCbits.RC1
#define RC1_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define RC1_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define RC1_SetPullup()             do { WPUCbits.WPUC1 = 1; } while(0)
#define RC1_ResetPullup()           do { WPUCbits.WPUC1 = 0; } while(0)
#define RC1_SetAnalogMode()         do { ANSELCbits.ANSC1 = 1; } while(0)
#define RC1_SetDigitalMode()        do { ANSELCbits.ANSC1 = 0; } while(0)

// get/set RC2 procedures
#define RC2_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define RC2_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define RC2_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define RC2_GetValue()              PORTCbits.RC2
#define RC2_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define RC2_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define RC2_SetPullup()             do { WPUCbits.WPUC2 = 1; } while(0)
#define RC2_ResetPullup()           do { WPUCbits.WPUC2 = 0; } while(0)
#define RC2_SetAnalogMode()         do { ANSELCbits.ANSC2 = 1; } while(0)
#define RC2_SetDigitalMode()        do { ANSELCbits.ANSC2 = 0; } while(0)

// get/set RC5 procedures
#define RC5_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define RC5_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define RC5_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define RC5_GetValue()              PORTCbits.RC5
#define RC5_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define RC5_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define RC5_SetPullup()             do { WPUCbits.WPUC5 = 1; } while(0)
#define RC5_ResetPullup()           do { WPUCbits.WPUC5 = 0; } while(0)
#define RC5_SetAnalogMode()         do { ANSELCbits.ANSC5 = 1; } while(0)
#define RC5_SetDigitalMode()        do { ANSELCbits.ANSC5 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()              PORTCbits.RC7
#define RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetPullup()             do { WPUCbits.WPUC7 = 1; } while(0)
#define RC7_ResetPullup()           do { WPUCbits.WPUC7 = 0; } while(0)
#define RC7_SetAnalogMode()         do { ANSELCbits.ANSC7 = 1; } while(0)
#define RC7_SetDigitalMode()        do { ANSELCbits.ANSC7 = 0; } while(0)

// get/set LED_R1 aliases
#define LED_R1_SetHigh()            do { LATDbits.LATD1 = 1; } while(0)
#define LED_R1_SetLow()             do { LATDbits.LATD1 = 0; } while(0)
#define LED_R1_Toggle()             do { LATDbits.LATD1 = ~LATDbits.LATD1; } while(0)
#define LED_R1_GetValue()           PORTDbits.RD1
// get/set LED_B1 aliases
#define LED_B1_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define LED_B1_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define LED_B1_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define LED_B1_GetValue()           PORTAbits.RA3
// get/set LED_G1 aliases
#define LED_G1_SetHigh()            do { LATDbits.LATD3 = 1; } while(0)
#define LED_G1_SetLow()             do { LATDbits.LATD3 = 0; } while(0)
#define LED_G1_Toggle()             do { LATDbits.LATD3 = ~LATDbits.LATD3; } while(0)
#define LED_G1_GetValue()           PORTDbits.RD3


// get/set LED_R2 aliases
#define LED_R2_SetHigh()            do { LATDbits.LATD2 = 1; } while(0)
#define LED_R2_SetLow()             do { LATDbits.LATD2 = 0; } while(0)
#define LED_R2_Toggle()             do { LATDbits.LATD2 = ~LATDbits.LATD2; } while(0)
#define LED_R2_GetValue()           PORTDbits.RD2
// get/set LED_G2 aliases
#define LED_G2_SetHigh()            do { LATDbits.LATD4 = 1; } while(0)
#define LED_G2_SetLow()             do { LATDbits.LATD4 = 0; } while(0)
#define LED_G2_Toggle()             do { LATDbits.LATD4 = ~LATDbits.LATD4; } while(0)
#define LED_G2_GetValue()           PORTDbits.RD4
// get/set LED_B2 aliases
#define LED_B2_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define LED_B2_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define LED_B2_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define LED_B2_GetValue()           PORTAbits.RA4
/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/