#include "bq25895.h"
//#include "pic_control_fun.h"
#include "mcc_generated_files/mcc.h"
#include "pic_i2c.h"

//static uint8_t setbits8(uint8_t value, uint8_t mask) {return value | mask;}
//static uint8_t clearbits8(uint8_t value, uint8_t mask) {return (value & ~mask);}
static uint8_t copybits8(uint8_t value, uint8_t mask, uint8_t new_value) {return ((uint8_t)(value & (~mask)) | new_value);}


uint8_t init_bq25895(void)
{
    uint8_t I2C_rsp;
    uint8_t I2c_buff[9];
    
    i2c_multi_read(I2C_SLAVE_ADDR_WR, 0x00, I2c_buff, 9);
    I2c_buff[0] = copybits8(I2c_buff[0], 0b01111111, 0b00010100);//REG 00 EN_ILIM- 0, 0->5=IINLIM-010100 1000mA
	I2c_buff[1] = copybits8(I2c_buff[1], 0b00000000, 0b00000000);//REG 01 no modification required
	I2c_buff[2] = copybits8(I2c_buff[2], 0b00001101, 0b00000000);//REG 02 3=HVDCP_EN- 0, 2=MAXC_EN- 0, 0=AUTO_DPDM_EN- 0
	I2c_buff[3] = copybits8(I2c_buff[3], 0b01100000, 0b01000000);//REG 03 6=WD_RST- 1, 5=OTG Disable
	I2c_buff[4] = copybits8(I2c_buff[4], 0b01111111, 0b00010000);//REG 04 4=1024amp charging current
	I2c_buff[5] = copybits8(I2c_buff[5], 0b00001111, 0b00000011);//REG 05 0->3=ITERM-0011 256mA
	I2c_buff[6] = copybits8(I2c_buff[6], 0b00000000, 0b00000000);//REG 06 no modification required
	I2c_buff[7] = copybits8(I2c_buff[7], 0b01000110, 0b00000100);//REG 07 6=STAT_DIS-0, 1->2=CHG_TIMER- 00
	I2c_buff[8] = copybits8(I2c_buff[8], 0b00000011, 0b00000000);//REG 08 0->1=TREG- 00 60degree temperature limit 
    I2C_rsp = i2c_multi_write(I2C_SLAVE_ADDR_WR, 0x00, I2c_buff, 9);
    
    return I2C_rsp;
}
uint8_t bq25895_WDT_reset(void)
{
   //uint8_t I2C_rsp;
   uint8_t Reg_03_value;
   Reg_03_value =  i2c_single_read(I2C_SLAVE_ADDR_WR, 0x03); 

   Reg_03_value = copybits8(Reg_03_value, 0b01100000, 0b01000000);//REG 03 6=WD_RST- 1
  
   i2c_single_write(I2C_SLAVE_ADDR_WR, 0x03, Reg_03_value); 
   return 0;
}

uint8_t i2c_multi_write(uint8_t device_addr, uint8_t reg_addr, uint8_t * data, uint8_t length)
{
    uint8_t I2C_rsp;
    uint8_t j;
    i2c_start();
    I2C_rsp = i2c_write(device_addr);
    I2C_rsp = i2c_write(reg_addr);
    for(j=0; j<length; j++)
    {
        I2C_rsp = i2c_write(data[j]);
    }
    i2c_stop();
    return I2C_rsp;
}

uint8_t * i2c_multi_read(uint8_t device_addr, uint8_t reg_addr, uint8_t * data, uint8_t length)
{
    uint8_t I2C_rsp;
    uint8_t j;
    i2c_start();
    I2C_rsp = i2c_write(device_addr);
    I2C_rsp = i2c_write(reg_addr);
    i2c_restart();
    I2C_rsp = (uint8_t)i2c_write(device_addr + 1);
    
    for(j=0; j<length; j++)
    {
        if(j == (length - 1))
        {
            data[j] = i2c_read(1);//for last read send NACK
        }
        else
        {
            data[j] = i2c_read(0);//send ACK
        }
    }
    
    i2c_stop();
    
    return data;
}

uint8_t i2c_single_write(uint8_t device_addr, uint8_t reg_addr, uint8_t data)
{
    uint8_t I2C_rsp;
    i2c_start();
    I2C_rsp = i2c_write(device_addr);
    I2C_rsp = i2c_write(reg_addr);
    I2C_rsp = i2c_write(data);
    i2c_stop();
    return I2C_rsp;
}

uint8_t i2c_single_read(uint8_t device_addr, uint8_t reg_addr)
{
    uint8_t I2C_rsp;
    uint8_t I2C_data;
    i2c_start();
    I2C_rsp = i2c_write(device_addr);
    I2C_rsp = i2c_write(reg_addr);
    i2c_restart();
    I2C_rsp = (uint8_t)i2c_write(device_addr + 1);
    I2C_data = i2c_read(1);
    i2c_stop();
    return I2C_data;
}
