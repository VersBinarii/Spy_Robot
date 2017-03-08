#include "h-ware.h"

void senddata(void);
void sendtable(void);
void createheader(UBYTE *header, UWORD heigh, UWORD width);
void createinfoheader(UBYTE *infoheader, UWORD heigh, UWORD width);
void USARTputnumchars(UBYTE *header, UWORD num);
