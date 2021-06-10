#include	<string.h>
#include "pic_control_fun.h"
#include "mcc_generated_files/mcc.h"
#include "bq25895.h"
#include "pic_i2c.h"
#include "main.h"

uint8_t pmic_stat_reg = 0x0b;
uint8_t battery_level;
uint8_t I2C_read_buff[10];
uint8_t pic_active_mode = 1;
uint8_t power_connected = PW_DISCONNECTED;

void set_led_status(void)
{
    static uint16_t tx_led_toggle_count = TX_LED_TOGGLE_COUNT;
    
    tx_led_toggle_count += TX_LED_TOGGLE_INCREMENT_VAL;
    if(tx_led_toggle_count >= TX_LED_TOGGLE_COUNT)
    {
        tx_led_toggle_count = 0;
        if( power_connected == PW_DISCONNECTED)
        {
           LED_B1_SetLow();
           __delay_ms(2);
           LED_B1_SetHigh();
        }
        else
        {
            if(!LED_B1_GetValue())
            {
                LED_B1_SetHigh();
            }
        }
    }
}

void set_MSSSP1_mode(uint8_t mode)
{
    static uint8_t current_mode;
    
    if(current_mode != mode)
    {
        bool state = (bool)GIE;
        GIE = 0;
        PPSLOCK = 0x55;
        PPSLOCK = 0xAA;
        PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS
        if(mode == MSSP1_SPI)
        {
            #if (PRINT_LOG)
            printf_string("\n\rMSSSP set as SPI");
            #endif
            current_mode = MSSP1_SPI;
            
            RC3PPS = 0x0F;   
            RC4PPS = 0x0F; 
            SSP1STAT = 0x00;
            SSP1CON1 = 0x00;
            SSP1CON3 = 0x00;
            SSP1ADD = 0x00;
            PIR3bits.SSP1IF = 0;
            PIE3bits.SSP1IE = 0;
            SSP1BUF = 0x00;
            SSP1MSK = 0x00;
            
            
            SSP1CLKPPS = 0x12;   //RC2->MSSP1:SCK1;    
            RC1PPS = 0x15;   //RC1->MSSP1:SDO1;    
            RC2PPS = 0x14;   //RC2->MSSP1:SCK1;    
            SSP1DATPPS = 0x10;   //RC0->MSSP1:SDI1;
            
            //SPI setup
            SSP1STAT = 0x80;
            SSP1CON1 = 0x00;
            SSP1ADD = 0x01;
            
            TRISCbits.TRISC2 = 0;
            TRISCbits.TRISC0 = 1;
            SSP1CON1bits.SSPEN = 0;
            __delay_ms(2);
            SPI1_Open(0);
            __delay_ms(2);
        }
        else if(mode == MSSP1_I2C)
        {
            #if (PRINT_LOG)
            printf_string("\n\rMSSSP set as I2C");
            #endif
            current_mode = MSSP1_I2C;
           
            RC1PPS = 0x0F;      
            RC2PPS = 0x0F; 
            SSP1STAT = 0x00;
            SSP1CON1 = 0x00;
            SSP1CON3 = 0x00;
            SSP1ADD = 0x00;
            PIR3bits.SSP1IF = 0;
            PIE3bits.SSP1IE = 0;
            SSP1BUF = 0x00;
            SSP1MSK = 0x00;
            
            RC3PPS = 0x14;   //RC3->MSSP1:SCL1;
            RC4PPS = 0x15;   //RC4->MSSP1:SDA1;
            SSP1CLKPPS = 0x13;   //RC3->MSSP1:SCL1;
            SSP1DATPPS = 0x14;   //RC4->MSSP1:SDA1;
            
             i2c_init();
             __delay_ms(2);
        }
        PPSLOCK = 0x55;
        PPSLOCK = 0xAA;
        PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS
        GIE = state; 
    }
    
}
void pmic_status_read(void)
{
#if (1)
    bool interrupt_state = (bool)GIE;
    static uint16_t pmic_stat_read_time_count = PMIC_STATSU_READ_TIME;
    static uint8_t pmic_wdt_clr_count = 30;
    pmic_stat_read_time_count += PMIC_READ_INCREMENT_VAL;
    
    if(pmic_stat_read_time_count >= PMIC_STATSU_READ_TIME)//to read all registers in PMIC
    {
        pmic_stat_read_time_count = 0;
        pmic_wdt_clr_count++;
        set_MSSSP1_mode(MSSP1_I2C);
        interrupt_state = (bool)GIE;
        GIE = 0;
        
        if(pmic_wdt_clr_count > 5)
        {
            #if(PRINT_LOG)
            printf("\n\rpmic_wdt_clr");
            #endif
            pmic_wdt_clr_count = 0;
            I2C_read_buff[0] = i2c_single_read(I2C_SLAVE_ADDR_WR, 0x02);
            if((I2C_read_buff[0] & 0b00000001) == 0x01)
            {
               #if(PRINT_LOG)
                     printf("\n\rBQ25895_re_init");
                #endif
               init_bq25895();
               __delay_ms(5);
            }
            I2C_read_buff[0] = (uint8_t)((I2C_read_buff[0] & 0b00111111) | (uint8_t)0b10000000);
            i2c_single_write(I2C_SLAVE_ADDR_WR, 0x02, I2C_read_buff[0]);
            bq25895_WDT_reset();//PMIC WDT clear
        }
        
        #if(PRINT_LOG)
            printf("\n\rpmic_batt_read");
        #endif
        memset((void*)I2C_read_buff, 0, 10);//Battery read
        i2c_multi_read(I2C_SLAVE_ADDR_WR, pmic_stat_reg, I2C_read_buff, 10);  
        GIE = interrupt_state;
        
        process_pmic_status(I2C_read_buff, 10);
    } 
#endif
}
void process_pmic_status(uint8_t * status, uint8_t length)
{
    uint8_t VBUS_STAT = 0;
    uint8_t CHRG_STAT = 0;
    uint8_t PG_STAT = 0;
    uint8_t SDP_STAT = 0;
    uint8_t VSYS_STAT = 0;
    uint16_t battery_volt;
    
    VBUS_STAT   = (uint8_t)((status[0] & 0b11100000) >> 5);
    CHRG_STAT   = (uint8_t)((status[0] & 0b00011000) >> 3);
    PG_STAT     = (uint8_t)((status[0] & 0b00000100) >> 2);
    SDP_STAT    = (uint8_t)((status[0] & 0b00000010) >> 1);
    VSYS_STAT   = (uint8_t)((status[0] & 0b00000001));
    
   
    if(1 == PG_STAT)//power connected
    {
        #if (PRINT_LOG)
        //printf_string("\n\rPower_connected");
        #endif
        power_connected = PW_CONNECTED;
        if((0b00000001 == CHRG_STAT) || (0b00000010 == CHRG_STAT))//Pre-charge or Fast Charging
        {
            if(pic_active_mode == 1)
            {
                LED_R1_SetLow();
                LED_G1_SetLow();
            }
        }
        else if(0b00000011 == CHRG_STAT)//Charge Termination Done
        {
            if(pic_active_mode == 1)
            {
                LED_R1_SetHigh();
                LED_G1_SetLow();
            }
        }
        else//not charging 
        {
            if(pic_active_mode == 1)
            {
                LED_R1_SetLow();
                LED_G1_SetHigh();  
            }
        }
    }
    else//no power connected
    {
        #if (PRINT_LOG)
        //printf_string("\n\rPower_Disconnected");
        #endif
        power_connected = PW_DISCONNECTED;
        if(pic_active_mode == 1)
        {
            LED_R1_SetHigh(); 
            LED_G1_SetHigh();
        }
    }
    
    battery_volt = (uint16_t)((( status[3] & 0b01111111) * 20) + 2304); //Battery voltage is only in 7 bits of 0x0E reg. So &7F
    battery_level = lipo_battery_level(battery_volt);
}  

uint8_t lipo_battery_level(uint16_t battery_volt)
{
    uint8_t battery_level_cal;
 
    if(3500 >= battery_volt)
    {
        battery_level_cal = 0;
    }
    else if(4200 < battery_volt)
    {
        battery_level_cal = 100;
    }
    else
    {
        battery_level_cal =  (battery_volt - 3500)/7;
    }
    return battery_level_cal;
}

#if(0)
void pic_control_fun_variables_init(void)
{
    sw_read_time = 0;
    time_counter = 0;
    reset_counter = 0;

    pmic_stat_reg = 0x0b;
    battery_level = 0;
    ble_booting_status = 0;
    current_ble_device_id = 0;

    power_sw_debounce_count = 0;
    ble_power_up = 0;
}

void check_power_control(void)
{
    #if (HW_VERSION == 4)
    if((!P_SW_GetValue()) || pic_power_off)
    {
        power_sw_debounce_count++;
        if((power_sw_debounce_count > 4) || pic_power_off)
        {
            pic_power_off = 0;
            power_sw_debounce_count = 0;
            if(0 == ble_power_up)
            {
                diable_ble_wtd_reset = true;
                ble_power_up = 1;
                uint8_t led_data[5]={0x7F, 0, 0, 0, 0};
                set_led_config(led_data);
                
                LED_R1_SetLow();
                __delay_ms(100); 
                LED_R1_SetHigh();
                CLRWDT();
                __delay_ms(500);
                LED_R1_SetLow();
                __delay_ms(100); 
                LED_R1_SetHigh();
                CLRWDT();
                __delay_ms(500);
                LED_R1_SetLow();
                __delay_ms(100); 
                LED_R1_SetHigh();
                CLRWDT();
             
                EV_3V3_SetLow();//RC6
                P_DAC1_SetLow();//RA2
                RST_W1_SetLow();//RE2
                pin_initialize();
                enable_sleep = 1;
                INTERRUPT_PeripheralInterruptEnable();
                INTERRUPT_GlobalInterruptEnable();
            }
            else
            {
                ble_power_up = 0;
           
                diable_ble_wtd_reset = false;
                init_ble_status();
                PIN_MANAGER_Initialize();
                LED_G1_SetLow();
                __delay_ms(100); 
                LED_G1_SetHigh();
                CLRWDT();
                __delay_ms(500);
                LED_G1_SetLow();
                __delay_ms(100); 
                LED_G1_SetHigh();
                CLRWDT();
                __delay_ms(500);
                LED_G1_SetLow();
                __delay_ms(100); 
                LED_G1_SetHigh();
                CLRWDT();
                restart_pic(0);
                
                INTERRUPT_PeripheralInterruptEnable();
                INTERRUPT_GlobalInterruptEnable();
            }
        }
    }
    else
    {
       power_sw_debounce_count = 0; 
    }
    #elif (HW_VERSION == 5)
        if(power_sw_debounce_count)
        {
            power_sw_debounce_count++;
            if(power_sw_debounce_count > 4)
            {
                power_sw_debounce_count = 0;
                if(0 == ble_power_up)
                {
                    diable_ble_wtd_reset = true;
                    ble_power_up = 1;
                    uint8_t led_data[5]={0x7F, 0, 0, 0, 0};
                    set_led_config(led_data);

                    LED_R1_SetLow();
                    __delay_ms(100); 
                    LED_R1_SetHigh();
                    CLRWDT();
                    __delay_ms(500);
                    LED_R1_SetLow();
                    __delay_ms(100); 
                    LED_R1_SetHigh();
                    CLRWDT();
                    __delay_ms(500);
                    LED_R1_SetLow();
                    __delay_ms(100); 
                    LED_R1_SetHigh();
                    CLRWDT();

                    EV_3V3_SetLow();//RC6
                    P_DAC1_SetLow();//RA2
                    RST_W1_SetLow();//RE2
                    pin_initialize();
                    enable_sleep = 1;
                    INTERRUPT_PeripheralInterruptEnable();
                    INTERRUPT_GlobalInterruptEnable();
                }
                else
                {
                    ble_power_up = 0;

                    diable_ble_wtd_reset = false;
                    init_ble_status();
                    PIN_MANAGER_Initialize();
                    LED_G1_SetLow();
                    __delay_ms(100); 
                    LED_G1_SetHigh();
                    CLRWDT();
                    __delay_ms(500);
                    LED_G1_SetLow();
                    __delay_ms(100); 
                    LED_G1_SetHigh();
                    CLRWDT();
                    __delay_ms(500);
                    LED_G1_SetLow();
                    __delay_ms(100); 
                    LED_G1_SetHigh();
                    CLRWDT();
                    restart_pic(0);

                    INTERRUPT_PeripheralInterruptEnable();
                    INTERRUPT_GlobalInterruptEnable();
                }
            }
        }
    #endif
}
#endif

#if(0)
void restart_pic(uint8_t rst_source)
{  
    /*switch(rst_source)
    {
        #warning "led loop reset"
       case 0:
            while(P_SW_GetValue())
            {
                CLRWDT();
                LED_B2_SetHigh();
                LED_G2_SetHigh();
                __delay_ms(200); 
                LED_B2_SetLow();
                LED_G2_SetLow();
                __delay_ms(200);  
            }
            break;
        case 1:
            while(P_SW_GetValue())
            {
                CLRWDT();
                LED_B2_SetHigh();
                __delay_ms(200); 
                LED_B2_SetLow();
                __delay_ms(200);  
            }
            break;
        case 2:
            while(P_SW_GetValue())
            {
                CLRWDT();
                LED_R2_SetHigh();
                __delay_ms(200); 
                LED_R2_SetLow();
                __delay_ms(200);  
            }
            break;
        case 3:
            while(P_SW_GetValue())
            {
                CLRWDT();
                LED_R2_SetHigh();
                LED_G2_SetHigh();
                __delay_ms(200); 
                LED_R2_SetLow();
                LED_G2_SetLow();
                __delay_ms(200);  
            }
            break;  
        case 4:
            while(P_SW_GetValue())
            {
                CLRWDT();
                LED_G2_SetHigh();
                __delay_ms(200); 
                LED_G2_SetLow();
                __delay_ms(200);  
            }
            break;     
    }*/
    if(!block_pic_restart)
    {
        LED_R1_SetLow(); 
        LED_B1_SetLow();
        LED_G1_SetLow();
        __delay_ms(100);
        RESET();
        while(1);
    }
    else if(!rst_source)
    {
        LED_R1_SetLow(); 
        LED_B1_SetLow();
        LED_G1_SetLow();
        __delay_ms(100);
        RESET();
        while(1);
    }
}
#endif

#if(0)
void start_up(void)
{
    PIN_MANAGER_Initialize();
    #if (HW_VERSION == 5)
        EUSART_Initialize();
    #endif
    I2C2_Initialize();
    i2c_init();
    //#warning "TMR1_Initialize disabled"
    TMR1_Initialize();
    CLRWDT();//wdt clear
    
    bool interrupt_state = (bool)GIE;
    GIE = 0;// Disable the Global Interrupts
    init_bq25895();
    GIE = interrupt_state;
    
    pic_notification_disable = 0;
    diable_ble_wtd_reset = 0;
    enable_sleep = 0;
    init_ble_status();
    timer0_variables_init();
    i2c2_variables_init();
    pic_control_fun_variables_init();
    ble_power_up = 0;
    startup_config_cmp = true;
}
void check_enter_program_mode(void)
{
    if(!P_SW_GetValue())
    {
        LED_R1_SetLow();  
        LED_G1_SetHigh();
        #if(PRINT_LOG)
            printf("\n\rBLE reset");
        #endif
        
        EN_3V0_SetLow();
        CLRWDT();
        __delay_ms(300);
        EN_3V0_SetHigh();
        //--------------------------------
        RST_B1_SetHigh();
        RST_B2_SetHigh();
        #if(HW_VERSION == 4)
            
        #elif (HW_VERSION == 5)
            RST_B3_SetHigh();
        #endif
        //--------------------------------
        __delay_ms(100);
        while(!P_SW_GetValue())
        {
           CLRWDT();
           __delay_ms(50); 
        }
        //--------------------------------
        RST_B1_SetLow();
        CLRWDT();
        __delay_ms(800);
        RST_B2_SetLow();
        __delay_ms(100);
        #if (HW_VERSION == 5)
            //RST_B3_SetLow();
        #endif
        //--------------------------------
        LED_R1_SetHigh();
        LED_G1_SetLow();
        CLRWDT();
    }
    else
    { 
        //#warning "no led blink in program mode"
        //LED_B1_Toggle();
    }  
}
#endif