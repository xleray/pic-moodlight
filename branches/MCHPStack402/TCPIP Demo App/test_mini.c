#include "TCPIP Stack/TCPIP.h"

#include "test.h"

const unsigned char mask_port_a 	= 0x2F;
const unsigned char mask_port_b 	= 0x02&(~BIT1); // pull up BIT1;
const unsigned char mask_port_c 	= 0xC3;


volatile unsigned char temp_porta 	= 0x0;
volatile unsigned char temp_portb 	= 0x0;
volatile unsigned char temp_portc 	= 0x0;

/*void Delay_mS(unsigned long a) { 
	 a *= 170;
	while (--a!=0); 
}	*/


void Delay(unsigned long a) { 
	 a *= 370;
	while (--a!=0); 
}

unsigned char TestExt(void) 
{

	unsigned char i;
	// TEST EXTENSION PORT
	
//	return TEST_OK;

	// Check for GND ======================================
	// all as input
	ADCON1 	= 0x0F;
	TRISA 	= mask_port_a;
	TRISB 	= mask_port_b;
	TRISC 	= mask_port_c;

	//pull up - output
	TRISB &= ~BIT1;
	//pull up - high
	PORTB |= BIT1;

	Delay(10);

	temp_porta 	= PORTA;
	temp_porta 	|= (~mask_port_a); 
	if((temp_porta) != (0xFF)) {
		return TEST_GND;
	} 

	temp_portb 	= PORTB;
	temp_portb 	|=(~mask_port_b); 
	if((temp_portb) != (0xFF)) {
	// if((PORTB|(mask_port_b)) != 0xFF) {
		return TEST_GND;
	} 

	temp_portc 	= PORTC;
	temp_portc 	|=(~mask_port_c); 
	if((temp_portc) != (0xFF)) {
		return TEST_GND;
	} 

	// Check for VCC =============================

	// all as input
	ADCON1 	= 0x0F;
	TRISA 	= mask_port_a;
	TRISB 	= mask_port_b;
	TRISC 	= mask_port_c;

	//pull up - output
	TRISB &= ~BIT1;
	//pull up - low
	PORTB &= ~BIT1;

	Delay(10);

	temp_porta 	= PORTA;
	temp_porta 	&= mask_port_a; 
	if((temp_porta) != (0x0)) {
		return TEST_VCC;
	} 
 
	temp_portb 	= PORTB;
	temp_portb 	&= mask_port_b; 
	if((temp_portb) != (0x0)) {
		return TEST_VCC;
	} 

	temp_portc 	= PORTC;
	temp_portc 	&= mask_port_c; 
	if((temp_portc) != (0x0)) {
		return TEST_VCC;
	} 


	// Running zero =========================================
	// all as input
	ADCON1 	= 0x0F;
	TRISA 	= mask_port_a;
	TRISB 	= mask_port_b;
	TRISC 	= mask_port_c;


	// PortA
	//pull up - output
	TRISB &= ~BIT1;
	//pull up - high
	PORTB |= BIT1;
	
	Delay(10);

	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_a)&(1<<i))) continue;
		
		// TRISA = ~((1<<i)|BIT2);
		TRISA = ~(1<<i);
		PORTA = ~(1<<i);

		Delay(10);

		// check for other zero at PortA
		temp_porta 	= PORTA;
		temp_porta 	|= (~mask_port_a); 
		if((temp_porta) != (0xFF&(~(1<<i)))) {
			return TEST_ZERO_A;
		} 

		// check for other zero at PortB	 
		temp_portb 	= PORTB;
		temp_portb 	|=(~mask_port_b); 
		if((temp_portb) != (0xFF)) {
			return TEST_ZERO_A;
		} 
	
		// check for other zero at PortC
		temp_portc 	= PORTC;
		temp_portc 	|=(~mask_port_c); 
		if((temp_portc) != (0xFF)) {
			return TEST_ZERO_A;
		} 

	}


	// all as input
	ADCON1 	= 0x0F;
	TRISA 	= mask_port_a;
	TRISB 	= mask_port_b;
	TRISC 	= mask_port_c;

	// PortB
	//pull up - output
	TRISB &= ~BIT1;
	//pull up - high
	PORTB |= BIT1;

	Delay(10);

	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_b)&(1<<i))) continue;
	
	
		TRISB = ~((1<<i)|BIT1);
		PORTB = ~(1<<i);
		
		Delay(10);

		// check for other zero at PortA
		temp_porta 	= PORTA;
		temp_porta 	|= (~mask_port_a); 
		if((temp_porta) != (0xFF)) {
			return TEST_ZERO_B;
		} 

		// check for other zero at PortB	 
		temp_portb 	= PORTB;
		temp_portb 	|=(~mask_port_b); 
		if((temp_portb) != (0xFF&(~(1<<i)))) {
			return TEST_ZERO_B;
		} 
	
		// check for other zero at PortC
		temp_portc 	= PORTC;
		temp_portc 	|=(~mask_port_c); 
		if((temp_portc) != (0xFF)) {
			return TEST_ZERO_B;
		} 
	}


	// all as input
	ADCON1 	= 0x0F;
	TRISA 	= mask_port_a;
	TRISB 	= mask_port_b;
	TRISC 	= mask_port_c;


	// PortC
	//pull up - output
	TRISB &= ~BIT1;
	//pull up - high
	PORTB |= BIT1;

	Delay(10);

	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_c)&(1<<i))) continue;
	
		TRISC = ~(1<<i);
		PORTC = ~(1<<i);
		
		Delay(10);
	
		// check for other zero at PortA
		temp_porta 	= PORTA;
		temp_porta 	|= (~mask_port_a); 
		if((temp_porta) != (0xFF)) {
			return TEST_ZERO_C;
		} 

		// check for other zero at PortB	 
		temp_portb 	= PORTB;
		temp_portb 	|=(~mask_port_b); 
		if((temp_portb) != (0xFF)) {
			return TEST_ZERO_C;
		} 
	
		// check for other zero at PortC
		temp_portc 	= PORTC;
		temp_portc 	|=(~mask_port_c); 
		if((temp_portc) != (0xFF&(~(1<<i)))) {
			return TEST_ZERO_C;
		} 
 	
	}
	return TEST_OK;
}	