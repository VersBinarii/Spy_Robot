//inc.h
#ifndef _INC_H_
#define _INC_H_

typedef unsigned char  UBYTE;                    /* Unsigned  8 bit quantity                           */
typedef          char  BYTE;                     /* Signed    8 bit quantity                           */
typedef unsigned int   UWORD;                    /* Unsigned 16 bit quantity                           */
typedef signed   int   WORD;                     /* Signed   16 bit quantity                           */
typedef unsigned long  ULONG;
		

#define FALSE 	0
#define TRUE  	1

#define INPUT 	0
#define OUTPUT	1

#define INTSOFF asm("FCLR I")
#define INTSON  asm("FSET I")

#endif