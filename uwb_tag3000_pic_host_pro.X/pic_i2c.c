#include "pic_i2c.h"

// setup the MSSP module for i2c master mode.
// requires TRIS to be input for the pins
void i2c_init()
{
	// SMP Standard Speed; CKE disabled; 
    SSP1STAT = 0x80;
    // SSPEN enabled; CKP Idle:Low, Active:High; SSPM FOSC/4_SSPxADD_I2C; 
    SSP1CON1 = 0x28;
    // SBCDE disabled; BOEN disabled; SCIE disabled; PCIE disabled; DHEN disabled; SDAHT 300ns; AHEN disabled; 
    SSP1CON3 = 0x08;
    // Baud Rate Generator Value: SSPADD 9;   
    //SSP1ADD = 0x09;
    SSP1ADD = 79;
   
    // clear the master interrupt flag
    PIR3bits.SSP1IF = 0;
    // enable the master interrupt
    PIE3bits.SSP1IE = 0;
}


void i2c_start()
{
	SSP1CON2bits.SEN = 1;
	while(SSP1CON2bits.SEN);

	return;
}

void i2c_restart()
{
	SSP1CON2bits.RSEN = 1;
	while(SSP1CON2bits.RSEN);

	return;
}


void i2c_stop()
{
	SSP1CON2bits.PEN = 1;
	while(SSP1CON2bits.PEN);

	return;
}

char i2c_write(char data)
{
	SSP1BUF = data;
	while(SSP1STATbits.RW);

	return SSP1CON2bits.ACKSTAT;
}

char i2c_read(char ack)
{
	SSP1CON2bits.RCEN = 1;

	SSP1CON2bits.ACKDT = ack;

// ACKEN + ACKDT

	while(!SSP1STATbits.BF);

	SSP1CON2bits.RCEN = 0;

	SSP1CON2bits.ACKEN = 1;
	while(SSP1CON2bits.ACKEN);

	return SSP1BUF;
}

/*void i2c_resync()
{
	// Toggle SCL until SDA is observed to be high
	// SCL = RC3
	// SDA = RC4
	while(!PORTCbits.RC4) {
		LATCbits.LATC3 = 1;
		LATCbits.LATC3 = 0;
	}

	LATCbits.LATC3 = 1;
}*/