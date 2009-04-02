#include "TCPIP Stack/TCPIP.h"

#include "test.h"



volatile unsigned char mask_port_a 	= 0x00; 
volatile unsigned char mask_port_b 	= 0x00;
volatile unsigned char mask_port_c 	= 0x00;
volatile unsigned char mask_port_d 	= 0x7F;
volatile unsigned char mask_port_e 	= 0x00;
//volatile unsigned char mask_port_f 	= 0x80;
volatile unsigned char mask_port_f 	= 0xFF;
//volatile unsigned char mask_port_g 	= 0x2E;
volatile unsigned char mask_port_g 	= 0x0F;
//volatile unsigned char mask_port_h 	= 0x08;
volatile unsigned char mask_port_h 	= 0xF8;
volatile unsigned char mask_port_j 	= 0xE8;

volatile unsigned char mask_x_pf 	= 0x7F; 
volatile unsigned char mask_x_ph 	= 0xF0;

volatile unsigned char temp_porta 	= 0x0;
volatile unsigned char temp_portb 	= 0x0;
volatile unsigned char temp_portc 	= 0x0;
volatile unsigned char temp_portd 	= 0x0; 
volatile unsigned char temp_porte 	= 0x0;
volatile unsigned char temp_portf 	= 0x0;
volatile unsigned char temp_portg 	= 0x0;
volatile unsigned char temp_porth 	= 0x0; 
volatile unsigned char temp_portj 	= 0x0; 

void InitPortX(void);


unsigned char 	i		= 0;

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
	LED0_IO ^= 1;

	// Load TMRL and TMRH for one sec interrupt
	TMR1L = 0x00;
	TMR1H = 0x80;

	// clear
	PIR1bits.TMR1IF = 0;
}


// simple delay ~ 1 mS
void Delay(unsigned long a) { 
	 a *= 370;
	while (--a!=0); 
}

void InitPort(void) {

	// all as gpio
	ADCON1 	= 0xFF;

	// All as input
	TRISA 	= 0xFF;
	TRISB 	= 0xFF;
	TRISC 	= 0xFF;
	TRISD 	= 0xFF;
	TRISE 	= 0xFF;
	TRISF 	= 0xFF;
	TRISG 	= 0xFF;
	TRISH 	= 0xFF;
	TRISJ 	= 0xFF;


	//pull up - output
	TRISJ &= ~BIT4;
	//pull up - high
	PORTJ |= BIT4;

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
	temp_portf 	= PORTF;
	temp_portf 	&= mask_port_f;
	temp_portg 	= PORTG;
	temp_portg 	&= mask_port_g;
	temp_porth 	= PORTH;
	temp_porth 	&= mask_port_h;
	temp_portj 	= PORTJ;
	temp_portj 	&= mask_port_j;
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
	temp_portf 	= PORTF;
	temp_portf 	|= (~mask_port_f); 
	temp_portg 	= PORTG;
	temp_portg 	|= (~mask_port_g); 
	temp_porth 	= PORTH;
	temp_porth 	|= (~mask_port_h); 
	temp_portj 	= PORTJ;
	temp_portj 	|= (~mask_port_j); 

}


unsigned char TestExt(void) 
{

	unsigned char rtctemp;	

	// TEST EXTENSION PORT

	// Check for GND ======================================
	InitPort();
	InitPortX();

	GetPort_OR();

	if( ((temp_porta) != (0xFF)) ||  
		((temp_portb) != (0xFF)) ||  
		((temp_portc) != (0xFF)) ||  
		((temp_portd) != (0xFF)) ||  
		((temp_porte) != (0xFF)) ||  
		((temp_portf) != (0xFF)) ||
		((temp_portg) != (0xFF)) ||
		((temp_porth) != (0xFF)) ||
		((temp_portj) != (0xFF)) )  return TEST_GND;


	// Check for VCC =============================
	// all as input
	InitPort();

	//pull up - output
	TRISJ &= ~BIT4;
	//pull up - low
	PORTJ &= ~BIT4;
	
	InitPortX();
	PORTA &= ~BIT5;

	Delay(10);

	GetPort_AND();
 
	if( ((temp_porta) != (0x0)) ||
		((temp_portb) != (0x0)) ||
		((temp_portc) != (0x0)) ||
		((temp_portd) != (0x0)) ||
		((temp_porte) != (0x0)) ||
		((temp_portf) != (0x0)) ||
		((temp_portg) != (0x0)) ||
		((temp_porth) != (0x0)) ||
		((temp_portj) != (0x0)) ) return TEST_VCC;


	// Running zero =========================================
	
	// Port A
	// all as input
	InitPort();
	InitPortX();

	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_a)&(1<<i))) continue;
		
		// TRISA = ~((1<<i)|BIT2);
		// PORTA = ~(1<<i);

		TRISA = ~(1<<i);
		PORTA = ~(1<<i);

		Delay(10);

		GetPort_OR();
		

		// check for other zero at PortA
		if((temp_porta) != (0xFF&(~(1<<i)))) 	{ return 3; } 
		// check for other zero at PortB
		if((temp_portb) != (0xFF)) 				{ return TEST_ZERO_A; } 
		// check for other zero at PortC
		if((temp_portc) != (0xFF)) 				{ return TEST_ZERO_A; } 
		// check for other zero at PortD
		if((temp_portd) != (0xFF)) 				{ return TEST_ZERO_A; } 
		// check for other zero at PortE
		if((temp_porte) != (0xFF)) 				{ return TEST_ZERO_A; } 
		// check for other zero at PortF
		if((temp_portf) != (0xFF)) 				{ return TEST_ZERO_A; } 
		// check for other zero at PortG
		if((temp_portg) != (0xFF))   			{ return TEST_ZERO_A; } 
		// check for other zero at PortH
		if((temp_porth) != (0xFF)) 				{ return TEST_ZERO_A; } 
		// check for other zero at PortG
		if((temp_portj) != (0xFF)) 				{ return TEST_ZERO_A; } 
		
	}


	// Port B
	// all as input
	InitPort();
	InitPortX();

	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_b)&(1<<i))) continue;
		
		TRISB = ~(1<<i);
		PORTB = ~(1<<i);

		Delay(10);

		GetPort_OR();

		// check for other zero at PortA
		if((temp_porta) != (0xFF)) 				{ return TEST_ZERO_B; } 
		// check for other zero at PortB
		if((temp_portb) != (0xFF&(~(1<<i)))) 	{ return TEST_ZERO_B; } 
		// check for other zero at PortC
		if((temp_portc) != (0xFF)) 				{ return TEST_ZERO_B; } 
		// check for other zero at PortD
		if((temp_portd) != (0xFF)) 				{ return TEST_ZERO_B; } 
		// check for other zero at PortE
		if((temp_porte) != (0xFF)) 				{ return TEST_ZERO_B; } 
		// check for other zero at PortF
		if((temp_portf) != (0xFF)) 				{ return TEST_ZERO_B; } 
		// check for other zero at PortG
		if((temp_portg) != (0xFF))   			{ return TEST_ZERO_B; } 
		// check for other zero at PortH
		if((temp_porth) != (0xFF)) 				{ return TEST_ZERO_B; } 
		// check for other zero at PortG
		if((temp_portj) != (0xFF)) 				{ return TEST_ZERO_B; } 
		
	}


	// PortC
	// all as input
	InitPort();
	InitPortX();

	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_c)&(1<<i))) continue;
	
		TRISC = ~(1<<i);
		PORTC = ~(1<<i);
		
		Delay(10);

		GetPort_OR();	
		
		// check for other zero at PortA
		if((temp_porta) != (0xFF)) 				{ return TEST_ZERO_C; } 
		// check for other zero at PortB
		if((temp_portb) != (0xFF)) 				{ return TEST_ZERO_C; } 
		// check for other zero at PortC
		if((temp_portc) != (0xFF&(~(1<<i))))	{ return TEST_ZERO_C; } 
		// check for other zero at PortD
		if((temp_portd) != (0xFF)) 				{ return TEST_ZERO_C; } 
		// check for other zero at PortE
		if((temp_porte) != (0xFF)) 				{ return TEST_ZERO_C; } 
		// check for other zero at PortF
		if((temp_portf) != (0xFF)) 				{ return TEST_ZERO_C; } 
		// check for other zero at PortG
		if((temp_portg) != (0xFF))   			{ return TEST_ZERO_C; } 
		// check for other zero at PortH
		if((temp_porth) != (0xFF)) 				{ return TEST_ZERO_C; } 
		// check for other zero at PortG
		if((temp_portj) != (0xFF)) 				{ return TEST_ZERO_C; }	
	}


	// PortD
	// all as input
	InitPort();
	InitPortX();
	
	//loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_d)&(1<<i))) continue;
	
		TRISD = ~(1<<i);
		PORTD = ~(1<<i);
		
		Delay(10);

		GetPort_OR();
	
		// check for other zero at PortA
		if((temp_porta) != (0xFF)) 				{ return TEST_ZERO_D; } 
		// check for other zero at PortB
		if((temp_portb) != (0xFF)) 				{ return TEST_ZERO_D; } 
		// check for other zero at PortC
		if((temp_portc) != (0xFF))				{ return TEST_ZERO_D; } 
		// check for other zero at PortD
		if((temp_portd) != (0xFF&(~(1<<i)))) 	{ return TEST_ZERO_D; } 
		// check for other zero at PortE
		if((temp_porte) != (0xFF)) 				{ return TEST_ZERO_D; } 
		// check for other zero at PortF
		if((temp_portf) != (0xFF)) 				{ return TEST_ZERO_D; } 
		// check for other zero at PortG
		if((temp_portg) != (0xFF))   			{ return TEST_ZERO_D; } 
		// check for other zero at PortH
		if((temp_porth) != (0xFF)) 				{ return TEST_ZERO_D; } 
		// check for other zero at PortG
		if((temp_portj) != (0xFF)) 				{ return TEST_ZERO_D; }	
	}


	// PortE
	// all as input
	InitPort();
	InitPortX();

	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_e)&(1<<i))) continue;
	
		TRISE = ~(1<<i);
		PORTE = ~(1<<i);
		
		Delay(10);
	
		GetPort_OR();	

		// check for other zero at PortA
		if((temp_porta) != (0xFF)) 				{ return TEST_ZERO_E; } 
		// check for other zero at PortB
		if((temp_portb) != (0xFF)) 				{ return TEST_ZERO_E; } 
		// check for other zero at PortC
		if((temp_portc) != (0xFF))				{ return TEST_ZERO_E; } 
		// check for other zero at PortD
		if((temp_portd) != (0xFF)) 				{ return TEST_ZERO_E; } 
		// check for other zero at PortE
		if((temp_porte) != (0xFF&(~(1<<i))))	{ return TEST_ZERO_E; } 
		// check for other zero at PortF
		if((temp_portf) != (0xFF)) 				{ return TEST_ZERO_E; } 
		// check for other zero at PortG
		if((temp_portg) != (0xFF))   			{ return TEST_ZERO_E; } 
		// check for other zero at PortH
		if((temp_porth) != (0xFF)) 				{ return TEST_ZERO_E; } 
		// check for other zero at PortG
		if((temp_portj) != (0xFF)) 				{ return TEST_ZERO_E; }
	
	}

	// PortF
	// all as input
	InitPort();
	InitPortX();

	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_f)&(1<<i))) continue;
	
		TRISF = ~(1<<i);
		PORTF = ~(1<<i);
		
		Delay(10);
	
		GetPort_OR();	

		// check for other zero at PortA
		if((temp_porta) != (0xFF)) 				{ return TEST_ZERO_F; } 
		// check for other zero at PortB
		if((temp_portb) != (0xFF)) 				{ return TEST_ZERO_F; } 
		// check for other zero at PortC
		if((temp_portc) != (0xFF))				{ return TEST_ZERO_F; } 
		// check for other zero at PortD
		if((temp_portd) != (0xFF)) 				{ return TEST_ZERO_F; } 
		// check for other zero at PortE
		if((temp_porte) != (0xFF))				{ return TEST_ZERO_F; } 
		// check for other zero at PortF
		if((temp_portf) != (0xFF&(~(1<<i)))) 	{ return TEST_ZERO_F; } 
		// check for other zero at PortG
		if((temp_portg) != (0xFF))   			{ return TEST_ZERO_F; } 
		// check for other zero at PortH
		if((temp_porth) != (0xFF)) 				{ return TEST_ZERO_F; } 
		// check for other zero at PortG
		if((temp_portj) != (0xFF)) 				{ return TEST_ZERO_F; }
	
	}


	// PortG
	// all as input
	InitPort();
	InitPortX();

	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_g)&(1<<i))) continue;
	
		TRISG = ~(1<<i);
		PORTG = ~(1<<i);
		
		Delay(10);
	
		GetPort_OR();	

		// check for other zero at PortA
		if((temp_porta) != (0xFF)) 				{ return TEST_ZERO_G; } 
		// check for other zero at PortB
		if((temp_portb) != (0xFF)) 				{ return TEST_ZERO_G; } 
		// check for other zero at PortC
		if((temp_portc) != (0xFF))				{ return TEST_ZERO_G; } 
		// check for other zero at PortD
		if((temp_portd) != (0xFF)) 				{ return TEST_ZERO_G; } 
		// check for other zero at PortE
		if((temp_porte) != (0xFF))				{ return TEST_ZERO_G; } 
		// check for other zero at PortF
		if((temp_portf) != (0xFF)) 				{ return TEST_ZERO_G; } 
		// check for other zero at PortG
		if((temp_portg) != (0xFF&(~(1<<i))))   	{ return TEST_ZERO_G; } 
		// check for other zero at PortH
		if((temp_porth) != (0xFF)) 				{ return TEST_ZERO_G; } 
		// check for other zero at PortG
		if((temp_portj) != (0xFF)) 				{ return TEST_ZERO_G; }
	
	}


	// PortH
	// all as input
	InitPort();
	InitPortX();

	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_h)&(1<<i))) continue;
	
		TRISH = ~(1<<i);
		PORTH = ~(1<<i);
		
		Delay(10);
	
		GetPort_OR();	

		// check for other zero at PortA
		if((temp_porta) != (0xFF)) 				{ return TEST_ZERO_H; } 
		// check for other zero at PortB
		if((temp_portb) != (0xFF)) 				{ return TEST_ZERO_H; } 
		// check for other zero at PortC
		if((temp_portc) != (0xFF))				{ return TEST_ZERO_H; } 
		// check for other zero at PortD
		if((temp_portd) != (0xFF)) 				{ return TEST_ZERO_H; } 
		// check for other zero at PortE
		if((temp_porte) != (0xFF))				{ return TEST_ZERO_H; } 
		// check for other zero at PortF
		if((temp_portf) != (0xFF)) 				{ return TEST_ZERO_H; } 
		// check for other zero at PortG
		if((temp_portg) != (0xFF))   			{ return TEST_ZERO_H; } 
		// check for other zero at PortH
		if((temp_porth) != (0xFF&(~(1<<i)))) 	{ return TEST_ZERO_H; } 
		// check for other zero at PortG
		if((temp_portj) != (0xFF)) 				{ return TEST_ZERO_H; }
	
	}

	// PortJ
	// all as input
	InitPort();
	InitPortX();

	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_j)&(1<<i))) continue;
	
		TRISJ = ~((1<<i)|BIT4);
		PORTJ = ~(1<<i);
		
		Delay(10);
	
		GetPort_OR();	

		// check for other zero at PortA
		if((temp_porta) != (0xFF)) 				{ return TEST_ZERO_J; } 
		// check for other zero at PortB
		if((temp_portb) != (0xFF)) 				{ return TEST_ZERO_J; } 
		// check for other zero at PortC
		if((temp_portc) != (0xFF))				{ return TEST_ZERO_J; } 
		// check for other zero at PortD
		if((temp_portd) != (0xFF)) 				{ return TEST_ZERO_J; } 
		// check for other zero at PortE
		if((temp_porte) != (0xFF))				{ return TEST_ZERO_J; } 
		// check for other zero at PortF
		if((temp_portf) != (0xFF)) 				{ return TEST_ZERO_J; } 
		// check for other zero at PortG
		if((temp_portg) != (0xFF))   			{ return TEST_ZERO_J; } 
		// check for other zero at PortH
		if((temp_porth) != (0xFF)) 				{ return TEST_ZERO_J; } 
		// check for other zero at PortG
		if((temp_portj) != (0xFF&(~(1<<i)))) 	{ return TEST_ZERO_J; }
	
	}
	
	InitRTC();
	Delay(100);
	Delay(100);
	rtctemp = TMR1L;
	Delay(100);
	Delay(100);
	if (TMR1L == rtctemp) return TEST_RTC;
	
	return TEST_OK;
}




void InitPortX(void) {

	// all as gpio
	ADCON1 	= 0xFF;

	// All as input
	TRISF 	= 0xFF;
	TRISH 	= 0xFF;

	//pull up - output
	TRISA &= ~BIT5;
	//pull up - high
	PORTA |= BIT5;

	Delay(10);

}

void GetPortX_AND(void) {

	temp_portf 	= PORTF;
	temp_portf 	&= mask_x_pf;
	temp_porth 	= PORTH;
	temp_porth 	&= mask_x_ph;
}

void GetPortX_OR(void) {

	temp_portf 	= PORTF;
	temp_portf 	|= (~mask_x_pf); 
	temp_porth 	= PORTH;
	temp_porth 	|= (~mask_x_ph); 
}


unsigned char TestXPort(void) {


	// TEST EXTENSION PORT

	// Check for GND ======================================
	InitPortX();

	GetPortX_OR();

	if( ((temp_portf) != (0xFF)) ||  
		((temp_porth) != (0xFF)) )  return TEST_GND;


	// Check for VCC =============================
	// all as input
	InitPortX();

	//pull up - output
	TRISA &= ~BIT5;
	//pull up - low
	PORTA &= ~BIT5;

	Delay(10);

	GetPortX_AND();
 
	if( ((temp_portf) != (0x0)) ||
		((temp_porth) != (0x0)) ) return TEST_VCC;


	// Running zero =========================================
	
	// Port A
	// all as input
	InitPortX();

	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_x_pf)&(1<<i))) continue;
		
		TRISF = ~(1<<i);
		PORTF = ~(1<<i);

		Delay(10);

		GetPortX_OR();

		// check for other zero at PortF
		if((temp_portf) != (0xFF&(~(1<<i)))) 	{ return TEST_ZERO_A; } 
		// check for other zero at PortH
		if((temp_porth) != (0xFF)) 				{ return TEST_ZERO_A; } 
	}


	// Port B
	// all as input
	InitPortX();

	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_x_ph)&(1<<i))) continue;
		
		TRISH = ~(1<<i);
		PORTH = ~(1<<i);

		Delay(10);

		GetPortX_OR();

		// check for other zero at PortH
		if((temp_portf) != (0xFF)) 				{ return TEST_ZERO_B; } 
		// check for other zero at PortF
		if((temp_porth) != (0xFF&(~(1<<i)))) 	{ return TEST_ZERO_B; } 
	}

	return TEST_OK;

}




void TestOptrons(void) {

	unsigned int i;

	// all optrons as input
	// D_INT1
	TRISCbits.TRISC2 = 1;
	// D_INT2
	TRISGbits.TRISG4 = 1;
	// D_INT3
	TRISBbits.TRISB4 = 1;
	// D_INT4
	TRISAbits.TRISA4 = 1;

#ifdef USE_LCD
	LCDInit();
#endif

	while(1) {

		if((PORTCbits.RC2)==0) {
			#ifdef USE_LCD
				LCDErase();
				Delay(200);
				strcpypgm2ram((char*)LCDText, "Optron 1");
				LCDUpdate();
				Delay(200);
			#endif
			
			i++;
		}

		if((PORTGbits.RG4)==0) {
			#ifdef USE_LCD
				LCDErase();
				Delay(200);
				strcpypgm2ram((char*)LCDText, "Optron 2");
				LCDUpdate();
				Delay(200);
			#endif

			i++;
		}

		if((PORTBbits.RB4)==0) {
			#ifdef USE_LCD
				LCDErase();
				Delay(200);
				strcpypgm2ram((char*)LCDText, "Optron 3");
				LCDUpdate();
				Delay(200);
			#endif

			i++;
		}

		if((PORTAbits.RA4)==0) {
			#ifdef USE_LCD
				LCDErase();
				Delay(200);
				strcpypgm2ram((char*)LCDText, "Optron 4");
				LCDUpdate();
				Delay(200);
			#endif

			i++;
		}
	}
}

