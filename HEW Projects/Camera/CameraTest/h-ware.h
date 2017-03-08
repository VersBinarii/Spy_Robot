//h-ware.h

#ifndef _HWARE_H_
#define _HWARE_H_

#include "inc.h"


#define B115200 		12    	/* 24MHz/( 13*16) = 115385 */
#define B57600 		25    	/* 24MHz/( 26*16) = 57692.3 */
#define B38400 		38    	/* 24MHz/( 39*16) = 38461.5 */
#define B19200 		77    	/* 24MHz/( 78*16) = 19230.8 */
#define B9600 			155    	/* 24MHz/(156*16) =  9615.38 */

#define UART_BUF_SIZE     8		// can be any size

void initHW(void);
void initPins(void);
void initUART0(UBYTE br);
void UART0putc(UBYTE b);
void UART0puts(const UBYTE *data);
void tellJava(UBYTE type, UWORD size);
void UART0Rx_IntHandler(void);

void initUART2(UBYTE br);
void UART2Tx_IntHandler(void);
UBYTE UART2putc(UBYTE ch);
#endif