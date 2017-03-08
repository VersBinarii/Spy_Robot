#ifndef RSKM16C62PDEF_H
#define RSKM16C62PDEF_H
/*********************************************************************************                                                                                   *
FILE NAME  		RSKm16C62Pdef.h
DESCRIPTION		Defines for RSKM16C29 starter kit
            
Copyright   : 2005 Renesas Technology Europe Ltd.
Copyright   : 2005 Renesas Technology Corporation.
All Rights Reserved
**********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
07.07.2005 RTE-MAB First Release
***********************************************************************************/

/***********************************************************************************
Defines
***********************************************************************************/

/* General Values */
#define LEDS_ON			(0x00)
#define LEDS_OFF		(0x0F)
#define SET_BIT_HIGH	(1)					
#define SET_BIT_LOW		(0)					
#define SET_BYTE_HIGH	(0xFF)				
#define SET_BYTE_LOW	(0x00)				
//#define TRUE			(1)					
//#define FALSE			(0)

/* LED port settings */
#define LED_PORT_DR		p4		/* LED Port data register */
#define LED_BIT			(0x0F)			/* Port bit to toggle for flashing LED */

/* Define switches to be polled if not available as interrupts */
//#define SW1 P1.PORT.BIT.B0

#define SW_val

#endif

