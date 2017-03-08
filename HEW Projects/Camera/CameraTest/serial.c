#include "inc.h"
#include "serial.h" 



// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +    UBYTE putfifo(Fifo * fifo, UBYTE databyte)
// +	Add databyte to fifo. 
// +    return
// +	FALSE fifo is full no can do
// +	TRUE  character in ok
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

UBYTE putfifo(Fifo * fifo, UBYTE databyte)
{
   UBYTE f,e;
    f=fifo->fillptr;
    e=fifo->empptr;
    if (f+1 == e) return FALSE;
    if ((f == fifo->size-1) && (e ==0)) return FALSE;
   
   fifo->data[fifo->fillptr] = databyte;

   ((fifo->fillptr + 1) >= fifo->size)?(fifo->fillptr = 0):(fifo->fillptr++);

   return TRUE;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +    UBYTE getfifo(Fifo * fifo)
// +    returns
// +	next byte from buffer
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

UBYTE getfifo(Fifo * fifo)
{
   UBYTE databyte;
   databyte = fifo->data[fifo->empptr];
   ((fifo->empptr + 1) >= fifo->size)?(fifo->empptr = 0):(fifo->empptr++);
   return(databyte);
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +    UBYTE peekfifo(Fifo * fifo)
// +    returns
// +	has look at next byte without taking it out
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

UBYTE peekfifo(Fifo * fifo)         //peek into the fifo without taking the byte/char
{
	return fifo->data[fifo->empptr];
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +    UBYTE DataInFifo(Fifo * fifo)
// +	How many bytes are in fifo now 
// +    returns
// +	0 if empty
// +	!=0 no of bytes left to be taken out
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


UBYTE DataInFifo(Fifo * fifo)
{
   if (fifo->fillptr == fifo->empptr)  return(0);
   if (fifo->fillptr > fifo->empptr)   return(fifo->fillptr - fifo->empptr);
   if (fifo->fillptr < fifo->empptr)   return(fifo->fillptr + (fifo->size - fifo->empptr));
    return(0);
}



UBYTE   RoomInFifo(Fifo * fifo)
{
   if (fifo->fillptr == fifo->empptr)  return(fifo->size - 1);
   if (fifo->fillptr > fifo->empptr)   return(fifo->size - (fifo->fillptr - fifo->empptr) - 1);
   if (fifo->fillptr < fifo->empptr)   return((fifo->empptr - fifo->fillptr) -1);
    return(0);
}

void     Resetfifo(Fifo * fifo)
{
   fifo->fillptr=0; fifo->empptr=0;// fifo->status.all=0;
}
/*

UBYTE GetFifoStatus(Fifo * fifo)
{
   return (fifo->status.all);
}


void ZeroAlignFifo(Fifo * fifo)
{
   int i,j,k;
   j=fifo->empptr;
   k=fifo->fillptr;
   if (j < k) {            //If the fill pointer has passed the end of the buffer we will just reset
      for (i=0; j < k ; i++,j++) {
         fifo->data[i]=fifo->data[j];
      }
      fifo->empptr=0;
      fifo->fillptr=i;
   }
   else  Resetfifo(fifo);
}
*/