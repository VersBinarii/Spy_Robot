#include "sfr62p.h" 
#include "inc.h"
#include "iic.h"



#define  	_1ms	700
#define CAM_REG_CLK 0x11
#define CAM_REG_CONTROL_A 0x12
#define CAM_RESET 0x80
#define CAM_FREQ_LOW 0x10
#define CAM_FREQ_MED 0x01
#define CAM_REG_CONTROL_C 0x14
#define CAM_DAT_QCIF 0x20



//--------------------------------------------------------------------------------------
// Name:		I2C_byte_read
// Parameters:	Address (byte) and data to write (byte)
// Returns:	None
// Description: It reads a byte in the pointed memory position
//--------------------------------------------------------------------------------------
UBYTE I2C_byte_read(UBYTE addr){

	UBYTE data;
	I2C_start();
	I2C_send_data(CAM_ADD_WRITE);
	I2C_send_data(addr);
	I2C_stop();

	I2C_start();
	I2C_send_data(CAM_ADD_WRITE | 1);
	data = I2C_read_data();
	I2C_stop();
	
	return data;
}

//--------------------------------------------------------------------------------------
// Name:		I2C_byte_write
// Parameters:	Address (byte) and data to write (byte)
// Returns:	None
// Description: It writes a byte in the pointed memory position
//--------------------------------------------------------------------------------------
void I2C_byte_write(UBYTE addr, UBYTE data){
	
	I2C_start();
	I2C_send_data(CAM_ADD_WRITE);
	I2C_send_data(addr);
	I2C_send_data(data);
	I2C_stop();
}

//--------------------------------------------------------------------------------------
// Name:		I2C_send_data
// Parameters:	Byte to be sent
// Returns:	1 = NACK received, 0 = ACK received
// Description: Send a byte through the serial line 
//--------------------------------------------------------------------------------------
UBYTE I2C_send_data(UBYTE data){
	
	UBYTE i;
	for (i = 8; i>0; i--)
	{
		if (data & (1 << (i-1)))
			SDA_HIGH();
		else
			SDA_LOW();
		Wait(50);
		TOGGLE_SCL();
	}
	return Look_for_ACK();	
}

//--------------------------------------------------------------------------------------
// Name:		I2C_read_data
// Parameters:	reads just one byte
// Returns: byte
//--------------------------------------------------------------------------------------
UBYTE I2C_read_data(void){
	
	UBYTE i, data;
	SDA_IN();
	data = 0;
	
	for (i = 8; i>0; i--)
	{
		Wait(50);
		SCL_HIGH();
		Wait(25);
		data = data << 1;
		if (SDA_PIN()==1)
			data |= 1;
		Wait(20);
		SCL_LOW();
	}
	return data;	
}


//--------------------------------------------------------------------------------------
// Name:		I2C_send_ACK
//--------------------------------------------------------------------------------------
void I2C_send_ACK(void){
	
	SDA_OUT();
	SCL_LOW();
	Wait(50);
	SDA_LOW();
	Wait(50);
	TOGGLE_SCL();
}

//--------------------------------------------------------------------------------------
// Name:		Look_for_ACK
// Parameters:	None
// Returns:	ACK/NACK
// Description: returns ACK/NACK state
//--------------------------------------------------------------------------------------
UBYTE Look_for_ACK(void){
	
	UBYTE i;
	Wait(30);
	SDA_IN();
	SCL_HIGH();
	Wait(10);
	i = SDA_PIN();
	Wait(30);
	SCL_LOW();
	SDA_OUT();
	return i;
}


//--------------------------------------------------------------------------------------
// Name:		I2C_start
// Parameters:	None
// Returns:	None
// Description: Generates the start condition in the i2c line
//--------------------------------------------------------------------------------------
void I2C_start(void){
	
	SDA_OUT();
	SCL_HIGH();
	SDA_HIGH();
	Wait(10);
	SDA_LOW();
	Wait(100);
	SCL_LOW();
	Wait(100);

}

//--------------------------------------------------------------------------------------
// Name:		I2C_stop
// Parameters:	None
// Returns:	None
// Description: Generates the stop condition in the i2c line
//--------------------------------------------------------------------------------------
void I2C_stop(void){
	
	SDA_OUT();
	SDA_LOW();
	SCL_LOW();
	Wait(50);
	SCL_HIGH();
	Wait(50);
	SDA_HIGH();
}

//--------------------------------------------------------------------------------------
// Name:		initI2C
// Parameters:	None
// Returns:	None
// Description: This routine configures pins as i2c
//--------------------------------------------------------------------------------------
void initI2C(void){
	
	SDA_OUT();
	SDA_HIGH();
	SCL_OUT();
	SCL_HIGH();
	
	// initializes timer 2 - used to time controls of i2c 
	ta2mr = 0x80;				// Set mode of timer A2
								// clk =24MHz/32 = 750kHz -> T = 1,333us
	ta2 = 0xFFFF;
	ta2s = 1;					// start counting
}


void TOGGLE_SCL(void){
	SCL_HIGH();
	Wait(100);
	SCL_LOW();
	Wait(100);
}
void SCL_OUT(void){
	SCL_pin_dir = 1;
}

void SCL_HIGH(void){
	SCL_pin = 1;
}
void SCL_LOW(void){
	SCL_pin = 0;
}

void Wait(UWORD time){
	UWORD dly_tmr;
	dly_tmr = ta2; 
	while((dly_tmr - ta2) < time ); // delay
}

void SDA_OUT(void){
	SDA_pin_dir = 1;
}


void SDA_IN(void){
	SDA_pin_dir = 0;
}

UBYTE SDA_PIN(void){	
	if (SDA_pin == 1)
		return 1;
	else
		return 0;
}

void SDA_HIGH(void){	
	SDA_pin = 1;
}

void SDA_LOW(void){
	SDA_pin = 0;
}
