//adc.c
#include "sfr62p.h" 
#include "inc.h"
#include "adc.h"

void initADC(){
	adcon0 = 0x80;	// an0, one-shot, software trigger, f4 or f2
	adcon1 = 0x30; // 8bit mode, Vref connected, opamps disabled
	adcon2 = 0x10;	
}

UBYTE getADC(void){
	
	adst = 1;	//start converstion
	while(adst != 0){;}	//wait till its done	
	
	return ad0l;
}