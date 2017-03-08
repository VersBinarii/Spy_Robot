#ifndef _IIC_H_
#define _IIC_H_


#define CAM_ADD_READ 	0xC1
#define CAM_ADD_WRITE 	0xC0
#define SCL_pin 		p7_5
#define SCL_pin_dir		pd7_5
#define SDA_pin		p7_4
#define SDA_pin_dir		pd7_4

void TOGGLE_SCL(void);
void SCL_HIGH(void);
void SCL_LOW(void);
void Wait(UWORD tim);

UBYTE Look_for_ACK(void);
void SDA_LOW(void);
void SDA_HIGH(void);
UBYTE SDA_PIN(void);
void SDA_OUT(void);
void SDA_IN(void);
void SCL_OUT(void);
void I2C_send_ACK(void);
UBYTE I2C_read_data(void);
UBYTE I2C_byte_read(UBYTE addr);
void I2C_byte_write(unsigned char addr, unsigned char data);
char I2C_send_data(unsigned char data);
void I2C_start(void);
void I2C_restart(void);
void I2C_stop(void);
void initI2C(void);

#endif