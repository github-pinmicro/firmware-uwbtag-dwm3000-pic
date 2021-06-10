#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <xc.h>
#include "pic_i2c.h"

#define I2C_SLAVE_ADDR_WR  0xD4


uint8_t init_bq25895(void);
uint8_t bq25895_WDT_reset(void);
uint8_t i2c_multi_write(uint8_t device_addr, uint8_t reg_addr, uint8_t * data, uint8_t length);
uint8_t * i2c_multi_read(uint8_t device_addr, uint8_t reg_addr, uint8_t * data, uint8_t length);
uint8_t i2c_single_write(uint8_t device_addr, uint8_t reg_addr, uint8_t data);
uint8_t i2c_single_read(uint8_t device_addr, uint8_t reg_addr);