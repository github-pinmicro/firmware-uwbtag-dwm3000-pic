/*
 * decawave.h
 *
 *  Created on: 02-Dec-2020
 *      Author: VISHAL
 */

#ifndef APPLICATION_USER_DECAWAVE_H_
#define APPLICATION_USER_DECAWAVE_H_
typedef struct
{
    uint32_t Lsb;
    uint32_t Msb;
}uint64_t; 
//void start_dw_transmit(void);
uint64_t get_sys_timestamp_u64(void);
uint64_t get_tx_timestamp_u64(void);
uint64_t get_rx_timestamp_u64(void);
void dw_main(void);

#endif /* APPLICATION_USER_DECAWAVE_H_ */
