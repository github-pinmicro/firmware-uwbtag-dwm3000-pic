#include"main.h"
#include <string.h>
#include <stdio.h>
#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_spi.h"
#include "port.h"
#include "decawave.h"
#include "mcc_generated_files/eusart.h"
#include "pic_control_fun.h"

/* Inter-ranging delay period, in milliseconds. */
#define RNG_DELAY_MS 1000
#define TX_DELAY (1000 - 3)
//#define RNG_DELAY_MS 200
/* Default communication configuration. We use here EVK1000's default mode (mode 3). */
static dwt_config_t config = {
		9,               /* Channel number. */
		DWT_PLEN_1024,    /* Preamble length. Used in TX only. */
		DWT_PAC32,        /* Preamble acquisition chunk size. Used in RX only. */
		9,               /* TX preamble code. Used in TX only. */
		9,               /* RX preamble code. Used in RX only. */
		1,               /* 0 to use standard 8 symbol SFD, 1 to use non-standard 8 symbol, 2 for non-standard 16 symbol SFD and 3 for 4z 8 symbol SDF type */
		DWT_BR_850K,      /* Data rate. */
		DWT_PHRMODE_STD, /* PHY header mode. */
		DWT_PHRRATE_STD, /* PHY header rate. */
		(1025 + 8 - 32),   /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
		DWT_STS_MODE_OFF, /* STS disabled */
		DWT_STS_LEN_64,/* STS length see allowed values in Enum dwt_sts_lengths_e */
		DWT_PDOA_M0      /* PDOA mode off */
};

dwt_txconfig_t txconfig_options_ch9 =
{
		0x34,           /* PG delay. */
		0xfefefefe,     /* TX power. */
		0x0             /*PG count*/
};

/* Default antenna delay values for 64 MHz PRF. See NOTE 1 below. */
//#define TX_ANT_DLY 16436
//#define RX_ANT_DLY 16436
#define TX_ANT_DLY 16385
#define RX_ANT_DLY 16385

/* Frames used in the ranging process. See NOTE 2 below. */
static uint8_t tx_blink_msg[] = { 0x00, 0x38, 0, 0xCA, 0xDE, //frame control 2, seq no 1, pan id 2
		0, 0, 0, 0, 0, 0, 0, 0, //tag address 8
		0, 0, 0, 0, 0, 0, 0, 0, //ts 8
		0, 0, 0, 0, 0, 0, 0, 0, 0, //battery level 1, secure key 8
		0, 0, 0, 0, 0, 0 }; //additional data 4, checksum 2
/* Indexes to access some of the fields in the frames defined above. */
#define DW_ADDR_RNG 9
#define DW_SN_IDX 2
#define DW_PAN 3
#define DW_ADDR_IDX 5
#define DW_TX_TS_IDX 9
#define DW_MSG_TS_LEN 8
#define DW_ID_LEN 4
#define DEVICE_IDX 26
#define ERROR_FACTOR 1000
#define TS_OFFSET 40
/* Buffer to store received response message.
 * Its size is adjusted to longest frame that this example code is supposed to handle. */
#define RX_BUF_LEN 21
static uint8_t rx_buffer[RX_BUF_LEN];
/* Hold copy of status register state here for reference so that it can be examined at a debug breakpoint. */
static uint32_t status_reg = 0;
/* Time-stamps of frames transmission/reception, expressed in device time units.
 * As they are 40-bit wide, we need to define a 64-bit int type to handle them. */
static uint64_t poll_tx_ts;
static uint64_t resp_rx_ts;
static uint64_t prev_ts;
static uint64_t add_time;
uint16_t TAG_ADDR;
uint16_t RNG_ADDR;
/*! ------------------------------------------------------------------------------------------------------------------
 * @fn main()
 *
 * @brief Application entry point.
 *
 * @param  none
 *
 * @return none
 */
void dw_main(void)
{
	char data[50];
    #if (PRINT_LOG)
    printf_string("\n\rdw_main");
    #endif
	/* Reset and initialise DW1000.
	 * For initialisation, DW1000 clocks must be temporarily set to crystal speed. After initialisation SPI rate can be increased for optimum
	 * performance. */
	reset_DWIC(); /* Target specific drive of RSTn line into DW3000 low for a period. */
	/* Configure SPI rate, DW3000 supports up to 38 MHz */
	//port_set_dw_ic_spi_fastrate();
	if (dwt_initialise(DWT_DW_INIT) == DWT_ERROR)
	{
        #if (PRINT_LOG)
		printf_string("\n\rDWM_ERROR");
        #endif
		while (1)
		{ };
	}
	if(dwt_configure(&config) < 0)
    {
        #if (PRINT_LOG)
		printf_string("\n\rCONFIG_ERROR");
        #endif
    }
	else
    {
        #if (PRINT_LOG)
		printf_string("\n\rCONFIG_SUCCESS");
        #endif
    }
	/* Configure the TX spectrum parameters (power, PG delay and PG count) */
	dwt_configuretxrf(&txconfig_options_ch9);
    //read_uwb_reg();
    //while(1);
	/* Configure sleep and wake-up parameters. */
	//dwt_configuresleep(DWT_CONFIG, DWT_PRES_SLEEP | DWT_WAKE_WUP | DWT_SLP_EN);
    //while(1);
	/* Configure DW IC to automatically enter sleep mode after transmission of a frame. */
	//dwt_entersleepaftertx(1);
	/* need to disable default interrupts, device will not go to sleep if interrupt line is high */
	//dwt_setinterrupt(SYS_ENABLE_LO_SPIRDY_ENABLE_BIT_MASK, 0, DWT_DISABLE_INT);
	/* Apply default antenna delay value. */
	dwt_setrxantennadelay(RX_ANT_DLY);
	dwt_settxantennadelay(TX_ANT_DLY);
	memcpy(&tx_blink_msg[DW_ADDR_IDX], &ADDR, 8);
	/* Loop forever initiating ranging exchanges. */
    #if (PRINT_LOG)
	printf_string("\n\rDWM_OK");
    #endif
	while (0)
	{	
        //dwt_wakeup_ic();
       // __delay_ms(1000);
        Sleep(1000); // Time needed for DW3000 to start up (transition from INIT_RC to IDLE_RC, or could wait for SPIRDY event)
//        while (!dwt_checkidlerc()) /* Need to make sure DW IC is in IDLE_RC before proceeding */
//        { };
        /* Restore the required configurations on wake */
       // dwt_restoreconfig();
        #if (PRINT_LOG)
        printf_string("\n\rDataSend");
        #endif
        tx_blink_msg[DW_SN_IDX]++;
        dwt_writetxdata(sizeof(tx_blink_msg), tx_blink_msg, 0); /* Zero offset in TX buffer. */
        dwt_writetxfctrl(sizeof(tx_blink_msg), 0, 0); /* Zero offset in TX buffer, ranging. */
        dwt_starttx(DWT_START_TX_IMMEDIATE);
//        Sleep(5);
        
        //char print_data[50];
//        uint32_t status_reg = dwt_read32bitreg(SYS_STATUS_ID);
//        if(status_reg & SYS_STATUS_TXFRS_BIT_MASK)
//        {
//            printf_string("\n\rTX SUCCESS");
//            dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS_BIT_MASK);
//            uint64_t tx_time = get_tx_timestamp_u64();
//            print_uint64_t(tx_time);
//        uint16_t msb = (status_reg>>16)&0x0000ffff;
//         uint16_t lsb = status_reg&0x0000ffff;
//         sprintf(print_data,"\n\rDeviceID: %04X%04X", msb, lsb);
//         printf_string(print_data); 
//        }
//        else
//        {
//          printf_string("\n\rTX_ERROR");
//        }
        
       // Sleep(TX_DELAY);
        
				//				start_dw_transmit();
	}
}

void transmitt_beacon_pkt(void)
{	
    #if (PRINT_LOG)
    printf_string("\n\rDataSend"); 
    #endif
    tx_blink_msg[DW_SN_IDX]++;
    dwt_writetxdata(sizeof(tx_blink_msg), tx_blink_msg, 0); /* Zero offset in TX buffer. */
    dwt_writetxfctrl(sizeof(tx_blink_msg), 0, 0); /* Zero offset in TX buffer, ranging. */
    dwt_starttx(DWT_START_TX_IMMEDIATE);       
}

uint64_t get_sys_timestamp_u64(void)
{
	uint64_t ts = {0, 0};
	dwt_readsystime((uint8_t*)&ts);
	return ts;
}

uint64_t get_tx_timestamp_u64(void)
{
	uint64_t ts = {0, 0};
	dwt_readtxtimestamp((uint8_t*)&ts);
//	if(ts < prev_ts)
//	{add_time++;}
//	prev_ts = ts;
//	ts += add_time << TS_OFFSET;
	return ts;
}

uint64_t get_rx_timestamp_u64(void)
{
	uint64_t ts = {0, 0};
//	dwt_readrxtimestamp((uint8_t*)&ts);
//	if(ts < prev_ts)
//	{add_time++;}
//	prev_ts = ts;
//	ts += add_time << TS_OFFSET;
	return ts;
}

