#include "bmp.h"
#include "inc.h"


void createheader(UBYTE *header, UWORD heigh, UWORD width){
	  UBYTE *p;
	  unsigned long sizefile = 1078 +((unsigned long )heigh*(unsigned long )width);

	  p = header;
  
	  //2 Bytes --BM Starting
	  *p = 'B';
	  p++;
	  *p = 'M';
	  p++;
	  //4 Bytes -- Size of file in bytes = 14 + 40 +1024 + HEIGH * WIDTH (100*100) = 2078 = 0x049A
	  *p = (UBYTE)(sizefile & 0xFF);
	  p++;
	  *p = (UBYTE)((sizefile & 0xFF00)>>8);
	  p++; 
	  *p = (UBYTE)((sizefile & 0xFF0000)>>16);
	  p++;
	  *p = 0;
	  p++;
	  //4 Bytes of reserved (= 0)
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  //4 Bytes of offset to the init of the data
	  *p = 0x36;
	  p++; 
	  *p = 0x04;
	  p++;
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
  
}

void createinfoheader(UBYTE *infoheader, UWORD heigh, UWORD width){
	  UBYTE *p;
	  UBYTE heighlow;
	  UBYTE heighhigh;
	  UBYTE widthlow;
	  UBYTE widthhigh;
  
	  heighlow = heigh & 0x00FF;
	  heighhigh = (heigh & 0xFF00)>>8;
	  widthlow = width & 0x00FF;
	  widthhigh = (width & 0xFF00)>>8;
  
	  p = infoheader;
  
	  //4 Bytes -- Size of InfoHeader =40 
	  *p = 40;
	  p++; 
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  //4 Bytes -- specifies the width of the image, in pixels.
	  *p = widthlow;
	  p++; 
	  *p = widthhigh;
	  p++;
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  //4 Bytes -- specifies the heigth of the image, in pixels.
	  *p = heighlow;
	  p++;
	  *p = heighhigh;
	  p++;
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  //2 Bytes -- Number of planes of the image = 1
	  *p = 1;
	  p++;
	  *p = 0;
	  p++;
	  //2 Bytes Bits per Pixel  -- In our case 8.
	  *p = 8;
	  p++;
	  *p = 0;
	  p++;
	  //4 bytes -- Type of Compression 0 = BI_RGB   no compression   
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  //4 bytes -- ImageSize  (compressed) It is valid to set this =0 
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  //XpixelsPerM 4 bytes horizontal resolution: Pixels/meter 
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  //YpixelsPerM 4 bytes vertical resolution: Pixels/meter 
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  //ColorsUsed 4 bytes Number of actually used colors =256
	  *p = 0;
	  p++;
	  *p = 1;//was 1
	  p++;
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  //ColorsImportant 4 bytes Number of important colors  0 = all 
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
	  *p = 0;
	  p++;
}

void USARTputnumchars(UBYTE *header, UWORD num){
  	UBYTE *p;
  	UWORD i;
  	p = header;
  	for(i=0; i<num;i++){
    		UART0putc(*p);
    		p++;
  	}

}

void sendtable(void){
	UWORD i;
  	for(i=0; i<256;i++){
    	UART0putc(i);
		UART0putc(i);
		UART0putc(i);
		UART0putc(0);
  	}

}
void senddata(void)
{
UWORD i, j;
	for(i=0; i<244;i++)
	{
    	for(j=0; j<176;j++)
		{
			UART0putc((UBYTE)j);
			UART0putc((UBYTE)j);

		}
  	}
}
