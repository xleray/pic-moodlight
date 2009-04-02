#include "TCPIP Stack/TCPIP.h"

#include "test.h" 

volatile unsigned char mask_port_a 	= 0x3C&(~BIT2); // pull up BIT2
volatile unsigned char mask_port_b 	= 0x00;
volatile unsigned char mask_port_c 	= 0x04;
volatile unsigned char mask_port_d 	= 0xDF;
volatile unsigned char mask_port_e 	= 0x07;


volatile unsigned char temp_porta 	= 0x0;
volatile unsigned char temp_portb 	= 0x0;
volatile unsigned char temp_portc 	= 0x0;
volatile unsigned char temp_portd 	= 0x0; 
volatile unsigned char temp_porte 	= 0x0; 


unsigned char 	i		= 0;

// simple delay ~ 1 mS
void Delay(unsigned long a) { 
	 a *= 370;
	while (--a!=0); 
}

void InitPort(void) {

	// all as input
	ADCON1 	= 0x06;
	TRISA 	= 0xFF;
	TRISC 	= 0xFF;
	TRISB 	= 0xFF;
	TRISD 	= 0xFF;
	TRISE 	= 0x07;

	//pull up - output
	TRISA &= ~BIT2;
	//pull up - high
	PORTA |= BIT2;

	Delay(10);

}

void GetPort_AND(void) {

	temp_porta 	= PORTA;
	temp_porta 	&= mask_port_a; 
	temp_portb 	= PORTB;
	temp_portb 	&= mask_port_b; 
	temp_portc 	= PORTC;
	temp_portc 	&= mask_port_c; 
	temp_portd 	= PORTD;
	temp_portd 	&= mask_port_d; 
	temp_porte 	= PORTE;
	temp_porte 	&= mask_port_e;
}

void GetPort_OR(void) {
	temp_porta 	= PORTA;
	temp_porta 	|= (~mask_port_a); 
	temp_portb 	= PORTB;
	temp_portb 	|= (~mask_port_b); 
	temp_portc 	= PORTC;
	temp_portc 	|= (~mask_port_c); 
	temp_portd 	= PORTD;
	temp_portd 	|= (~mask_port_d); 
	temp_porte 	= PORTE;
	temp_porte 	|= (~mask_port_e); 
}


void InitRTC(void)
{
	// Enable timer&external oscillator driver
	//T1CON = TMR1ON | TMR1CS | T1OSCEN;
	
	TRISC |= 0b11;
	
	T1CON = 0b00001011;
	
	// the first interrut will be in ~1 sec
	TMR1L = 0x00;
	TMR1H = 0x80;
	
	// enable timer1 interrupt on overflow
	PIE1bits.TMR1IE = 1;
	IPR1bits.TMR1IP = 1; //high priority
	
	INTCONbits.GIEH = 1;
	
}

void
InterruptHandlerHigh();

#pragma code InterruptVectorHigh = 0x08
void
InterruptVectorHigh (void)
{
  _asm
    goto InterruptHandlerHigh //jump to interrupt routine
  _endasm
}

//----------------------------------------------------------------------------
// High priority interrupt routine

#pragma code
#pragma interrupt InterruptHandlerHigh

void
InterruptHandlerHigh ()
{
		// Toggle led 
	LED0_TRIS = 0;
	LED0_IO ^= 1;

	// Load TMRL and TMRH for one sec interrupt
	TMR1L = 0x00;
	TMR1H = 0x80;

	// clear
	PIR1bits.TMR1IF = 0;
}


unsigned char TestExt(void) {

	unsigned char rtctemp;

	// TEST EXTENSION PORT

	// Check for GND ======================================
	InitPort();

	GetPort_OR();

	if( ((temp_porta) != (0xFF)) ||  
		((temp_portb) != (0xFF)) ||  
		((temp_portc) != (0xFF)) ||  
		((temp_portd) != (0xFF)) ||  
		((temp_porte) != (0xFF)) )  return TEST_GND;


	// Check for VCC =============================
	// all as input
	InitPort();

	//pull up - output
	TRISA &= ~BIT2;
	//pull up - low
	PORTA &= ~BIT2;

	Delay(10);

	GetPort_AND();
 
	if( ((temp_porta) != (0x0)) ||
		((temp_portb) != (0x0)) ||
		((temp_portc) != (0x0)) ||
		((temp_portd) != (0x0)) ||
		((temp_porte) != (0x0)) ) return TEST_VCC;



	// Running zero =========================================
	
	// Port A
	// all as input
	InitPort();
	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_a)&(1<<i))) continue;
		
		TRISA = ~((1<<i)|BIT2);
		PORTA = ~(1<<i);

		Delay(10);

		GetPort_OR();

		// check for other zero at PortA
		if((temp_porta) != (0xFF&(~(1<<i)))) {
			return TEST_ZERO_A;
		} 

		// check for other zero at PortC
		if((temp_portc) != (0xFF)) {
			return TEST_ZERO_A;
		} 

		// check for other zero at PortD
		if((temp_portd) != (0xFF)) {
			return TEST_ZERO_A;
		} 

		// check for other zero at PortE
		if((temp_porte) != (0xFF)) {
			return TEST_ZERO_A;
		} 
		
	}

	// PortC
	// all as input
	InitPort();

	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_c)&(1<<i))) continue;
	
		TRISC = ~(1<<i);
		PORTC = ~(1<<i);
		
		Delay(10);

		GetPort_OR();	
		
		// check for other zero at PortA
		if((temp_porta) != (0xFF)) {
			return TEST_ZERO_C;
		} 
	
		// check for other zero at PortC
		if((temp_portc) != (0xFF&(~(1<<i)))) {
			return TEST_ZERO_C;
		} 

		// check for other zero at PortB	 
		if((temp_portd) != (0xFF)) {
			return TEST_ZERO_C;
		} 
		
		// check for other zero at PortB	 
		if((temp_porte) != (0xFF)) {
			return TEST_ZERO_C;
		} 
	
	}


	// PortD
	// all as input
	InitPort();
	
	//loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_d)&(1<<i))) continue;
	
		TRISD = ~(1<<i);
		PORTD = ~(1<<i);
		
		Delay(10);

		GetPort_OR();
	
		// check for other zero at PortA
		if((temp_porta) != (0xFF)) {
			return TEST_ZERO_D;
		} 
	
		// check for other zero at PortC
		if((temp_portc) != (0xFF)) {
			return TEST_ZERO_D;
		} 

		// check for other zero at PortB	 
		if((temp_portd) != (0xFF&(~(1<<i)))) {
			return TEST_ZERO_D;
		} 
		
		// check for other zero at PortB	 
		if((temp_porte) != (0xFF)) {
			return TEST_ZERO_D;
		} 	
	}


	// PortE
	// all as input
	InitPort();

	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_e)&(1<<i))) continue;
	
		TRISE = ~(1<<i);
		PORTE = ~(1<<i);
		
		Delay(10);
	
		GetPort_OR();	

		// check for other zero at PortA 
		if((temp_porta) != (0xFF)) {
			return TEST_ZERO_E;
		} 

	
		// check for other zero at PortC
		if((temp_portc) != (0xFF)) {
			return TEST_ZERO_E;
		} 

		// check for other zero at PortD	 
		if((temp_portd) != (0xFF)) {
			return TEST_ZERO_E;
		} 
		
		// check for other zero at PortE	 
		if((temp_porte) != (0xFF&(~(1<<i)))) {
			return TEST_ZERO_E;
		} 
	
	}
	
	InitRTC();

	rtctemp = TMR1L;
		Delay(10);
	if (TMR1L == rtctemp) return TEST_RTC;
	
	return TEST_OK;
}




