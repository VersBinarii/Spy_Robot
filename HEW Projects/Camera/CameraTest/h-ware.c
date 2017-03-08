//h-ware.c
#include "inc.h"
#include "iic.h"
#include "sfr62p.h"
#include "serial.h"
#include "h-ware.h"
#include "rskM16C62Pdef.h"

const UBYTE BaudRates[] = {B9600, B19200, B38400,B57600, B115200, B19200, B19200, B19200};

UBYTE   	RxBuffer[UART_BUF_SIZE];
Fifo    	RxFifo = {0, 0, UART_BUF_SIZE, RxBuffer};

UBYTE	TxBuffer[UART_BUF_SIZE];
Fifo		TxFifo = {0, 0, UART_BUF_SIZE, TxBuffer};

typedef struct {
	UBYTE	KickTx:1;
	UBYTE	TxBuf :1;		// whether there is any data in tx buffer to tx
	UBYTE   fError:1;
} UARTDATA;

UARTDATA UART2Status;

void initHW(void){
	int x;
	
	prc0 = 1;		/* Protect off */
//	cm13 = 1;		/* Xin Xout */
	cm15 = 1;		/* XCIN-XCOUT drive capacity select bit : HIGH */
	cm05 = 0; 		/* Xin on */
	cm16 = 0; 		/* Main clock = No division mode */
	cm17 = 0;
	cm06 = 0; 		/* CM16 and CM17 enable */	
	pm20 = 0;		/*two wait states*/
	plc0 = 0x92;	/*PLL on multiply by 4*/
	for(x=0;x<1000;x++){
		asm("nop"); 	/* Waiting for stable of oscillation */
		asm("nop");
		asm("nop");
		asm("nop");
	}
	cm11 = 1;		/*PLL on*/
	prc0 = 0; 		/* Protect on */
	}

void initPins(void){
	pd0 = 0x00;	/*all input, used for data read from cam*/
	
	pd1_0 = INPUT;	//pinHref
	pd1_1 = INPUT;	//pin Vsync
	pd1_2 = INPUT;	//pin Dclk	
	
	pd6_2 = INPUT;     //input for Rx uart pin
	pd6_3 = OUTPUT;	//output for Tx pin
	
	pd7_0 = OUTPUT;	//Tx output
	pd7_1 = INPUT;	//Rx input
	
	pd2 = 0xff;
	pd4 = 0x0f;//leds port output
	
	pd10_0 = INPUT;	//AN0 pin to measure battery
	
	//limit switches
	pd2_0 = INPUT;
	pd2_1 = INPUT;
	pd2_2 = INPUT;
	pd2_3 = INPUT;
	
}
/****************************************************************************************
/All functions asociated wit UART0
/***************************************************************************************/

void initUART0(UBYTE br){
	//gonna use uart 0 for PC communiaction
	re_u0c1 = 0;                /* receiver off */
	te_u0c1 = 0;                /* transmitter off */
	
	u0brg = BaudRates[br & 0x7];      // set the baud rate 
	
	u0mr = 0x05;	//uart on 8bit long, no parity, 1 stop bit, no reverse, internal clock
	u0c0 = 0x10;   //cts disabled
	u0c1 = 0x00;		
//	u0irs = 0x01;	//interrupt on empty transmit buffer
	
	re_u0c1 = 1;                /* receiver on */
	te_u0c1 = 1;                /* transmitter on */
	
	s0ric = 0x0c;		/* uart0 Rx at int level 4 */ 
}

void UART0putc(UBYTE b){
	
	while(ti_u0c1 == 0);	//wait for buffer to become empty
	u0tb = b;
}

void UART0puts(const UBYTE *data) {
  int len, count;
  
  len = strlen(data);
  for (count = 0; count < len; count++) 
    UART0putc(*(data++));
   // Wait(1);
}

void tellJava(UBYTE type, UWORD size){
	UART0putc(type);
	UART0putc((size >> 24) & 0xff);
	UART0putc((size >> 16) & 0xff);
	UART0putc((size >> 8) & 0xff);
	UART0putc((size >> 0) & 0xff);
}

#pragma interrupt UART0Rx_IntHandler

void UART0Rx_IntHandler( void ){
	UWORD zxy;

    	zxy = u0rb;
	
	if((zxy & 0x8000) != 0){		//any errors??
		
		re_u0c1 = 0;               
		re_u0c1 = 1;               //restart to clear errors
		
	}else{
		
		putfifo(&RxFifo, (UBYTE)zxy & 0xff);	// store character in buffer
		
	}


}

/****************************************************************************************
/All functions asociated with UART2
/***************************************************************************************/
void initUART2(UBYTE br){
	//gonna use uart 2 for communiaction with the PIC
	re_u2c1 = 0;                /* receiver off */
	te_u2c1 = 0;                /* transmitter off */
	
	u2brg = BaudRates[br & 0x7];      // set the baud rate 
	
	u2mr = 0x05;	//uart on 8bit long, no parity, 1 stop bit, no reverse, internal clock
	u2c0 = 0x10;   //cts disabled
	u2c1 = 0x00;		
	//u2irs = 0x01;	//interrupt on empty transmit buffer
	
	re_u2c1 = 1;                /* receiver on */
	te_u2c1 = 1;                /* transmitter on */
	
	s2tic= 0x0b;		/* uart2 Tx at int level 3 */ 
}

#pragma interrupt UART2Tx_IntHandler

void UART2Tx_IntHandler(void){
	
	if(DataInFifo(&TxFifo) == 0 ){
		
		UART2Status.KickTx = 0;
//		UART2Status.TxBuf = Free;
		
	}else{
		
		u2tb = getfifo(&TxFifo);
	}
}

UBYTE UART2putc(UBYTE ch){
	// if we need to kick the thing off then we can write the byte directly to the uart

	INTSOFF;
	if (UART2Status.KickTx == 0)
	{ // we write directly
		UART2Status.KickTx = 1;
//		UART2Status.TxBuf = InUse;

		INTSON;
			
		u2tb = ch;
		return TRUE;
	}
	
	else
	{	// we're writing already
		ch = putfifo(&TxFifo,  ch);
		INTSON;
		return ch;
	}
}