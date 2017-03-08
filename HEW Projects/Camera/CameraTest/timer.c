//timer.c
#include "sfr62p.h" 
#include "h-ware.h"
#include "inc.h"
#include "timer.h"
#include "adc.h"
#include "iic.h"

#define BAT_30s	30000
#define CHECKS		20




float battVolt = 0;
UWORD static adCount = 0;
UBYTE debouncedState = 0;
UBYTE index = 0;
/*
UBYTE debounceFL(){
    static UWORD State = 0; // Current debounce status
    State=(State<<1) | SW_FL | 0xe000;
    if(State==0xe000)return TRUE;
    return FALSE; 
}

UBYTE debounceFR(){
    static UWORD State = 0; // Current debounce status
    State=(State<<1) | SW_FR | 0xe000;
    if(State==0xe000)return TRUE;
    return FALSE; 
}

UBYTE debounceBL(){
    static UWORD State = 0; // Current debounce status
    State=(State<<1) | SW_BL | 0xe000;
    if(State==0xe000)return TRUE;
    return FALSE; 
}

UBYTE debounceBR(){
    static UWORD State = 0; // Current debounce status
    State=(State<<1) | SW_BR | 0xe000;
    if(State==0xe000)return TRUE;
    return FALSE; 
}*/

void initTimerA0(void){
	
	ta0s = 0;
	ta0mr = 0x80;	//f32 -> 1.333us per tick
	ta0 = 750;	//1.333us * 750 == 999.750us	
	ta0ic = 5;	//interrupt priority 5
    	ta0s = 1;      //start TA0 	
}

#pragma interrupt TimerA0_IntHandler

void TimerA0_IntHandler(void){
	UBYTE adNow = 0;
	
	
	adCount++;
	if(adCount == BAT_30s){
		adNow = (UBYTE)getADC();
	//	battVolt = 0.0127 * adNow + 0.0046;
		tellJava(3, 10);
		UART0puts("Battery: ");
		UART0putc(adNow);
		adCount = 0;
	}
	/*
	//limit switches check
	if(debounceFL() == TRUE){
		tellJava(3, 20);
		UART0puts("Obstacle: Front-Left");	//notify java
		UART2putc(0xf4);				//tell pic to referse
		UART2putc(15);
		Wait(500);
	}
	
	if(debounceFR() == TRUE){
		tellJava(3, 21);
		UART0puts("Obstacle: Front-Right");	//notify java
		UART2putc(0xf4);				//tell pic to referse
		UART2putc(15);
		Wait(500);
	}
	
	if(debounceBL() == TRUE){
		tellJava(3, 19);
		UART0puts("Obstacle: Back-Left");	//notify java
		UART2putc(0xf3);				//tell pic to referse
		UART2putc(15);
		Wait(500);
	}
	
	if(debounceBR() == TRUE){
		tellJava(3, 20);
		UART0puts("Obstacle: Back-Right");	//notify java
		UART2putc(0xf3);				//tell pic to referse
		UART2putc(15);
		Wait(500);
	}*/
}
