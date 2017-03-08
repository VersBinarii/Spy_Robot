
#ifndef __SERIAL_H__
#define __SERIAL_H__
#include "inc.h"


typedef struct {
   UBYTE fillptr;
   UBYTE empptr;
   UWORD size;
   UBYTE * data;
} Fifo;

//typedef Fifo*   Astream;

//Prototypes for fifo
UBYTE	putfifo(Fifo *, UBYTE);
UBYTE	getfifo(Fifo *);
UBYTE	peekfifo(Fifo *);	//peek into the fifo without taking the byte/char
UBYTE	DataInFifo(Fifo *);
UBYTE	RoomInFifo(Fifo *);
void		Resetfifo(Fifo *);
//UBYTE	GetFifoStatus(Fifo * fifo);
//void	ZeroAlignFifo(Fifo * fifo);	//moves the data in fifo back so that empty pointer is 0 




/*
//Example usage

#define SIZE_OF_MY_DATA 64
UBYTE   mydata[SIZE_OF_MY_DATA];
Fifo    myfifo = {0,0,SIZE_OF_MY_FIFO,0,&mydata};

Astream out,in;

//then use as follows
if (RoomInFifo(out)) putfifo(out,char);
if (DataInFifo(in))  getfifo(in,char);
//or
if (putfifo(out,char) move on
else block and wait for room to occur

//use the status to support the connection
*/

#endif
