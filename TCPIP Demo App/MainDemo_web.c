/*********************************************************************
 *
 *  Main Application Entry Point and TCP/IP Stack Demo
 *  Module for Microchip TCP/IP Stack
 *   -Demonstrates how to call and use the Microchip TCP/IP stack
 *	 -Reference: AN833
 *
 *********************************************************************
 * FileName:        MainDemo.c
 * Dependencies:    string.H
 *                  StackTsk.h
 *                  Tick.h
 *                  http.h
 *                  MPFS.h
 *				   	mac.h
 * Processor:       PIC18, PIC24F, PIC24H, dsPIC30F, dsPIC33F
 * Complier:        Microchip C18 v3.02 or higher
 *					Microchip C30 v2.05 or higher
 *					HI-TECH PICC-18
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright � 2002-2007 Microchip Technology Inc.  All rights 
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and 
 * distribute: 
 * (i)  the Software when embedded on a Microchip microcontroller or 
 *      digital signal controller product (�Device�) which is 
 *      integrated into Licensee�s product; or
 * (ii) ONLY the Software driver source files ENC28J60.c and 
 *      ENC28J60.h ported to a non-Microchip device used in 
 *      conjunction with a Microchip ethernet controller for the 
 *      sole purpose of interfacing with the ethernet controller. 
 *
 * You should refer to the license agreement accompanying this 
 * Software for additional information regarding your rights and 
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT 
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL 
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR 
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF 
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS 
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE 
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER 
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT 
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * Author              Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Nilesh Rajbharti		4/19/01		Original (Rev. 1.0)
 * Nilesh Rajbharti		2/09/02		Cleanup
 * Nilesh Rajbharti		5/22/02		Rev 2.0 (See version.log for detail)
 * Nilesh Rajbharti		7/9/02		Rev 2.1 (See version.log for detail)
 * Nilesh Rajbharti		4/7/03		Rev 2.11.01 (See version log for detail)
 * Howard Schlunder		10/1/04		Beta Rev 0.9 (See version log for detail)
 * Howard Schlunder		10/8/04		Beta Rev 0.9.1 Announce support added
 * Howard Schlunder		11/29/04	Beta Rev 0.9.2 (See version log for detail)
 * Howard Schlunder		2/10/05		Rev 2.5.0
 * Howard Schlunder		1/5/06		Rev 3.00
 * Howard Schlunder		1/18/06		Rev 3.01 ENC28J60 fixes to TCP, 
 *									UDP and ENC28J60 files
 * Howard Schlunder		3/01/06		Rev. 3.16 including 16-bit micro support
 * Howard Schlunder		4/12/06		Rev. 3.50 added LCD for Explorer 16
 * Howard Schlunder		6/19/06		Rev. 3.60 finished dsPIC30F support, added PICDEM.net 2 support
 * Howard Schlunder		8/02/06		Rev. 3.75 added beta DNS, NBNS, and HTTP client (GenericTCPClient.c) services
 * Howard Schlunder		12/28/06	Rev. 4.00RC added SMTP, Telnet, substantially modified TCP layer
 * Howard Schlunder		04/09/07	Rev. 4.02 added TCPPerformanceTest, UDPPerformanceTest, Reboot and fixed some bugs
 ********************************************************************/
/*
 * Following define uniquely deines this file as main
 * entry/application In whole project, there should only be one such
 * definition and application file must define AppConfig variable as
 * described below.
 */
#define THIS_IS_STACK_APPLICATION

#define VERSION 		"v4.02"		// TCP/IP stack version
#define BAUD_RATE       (19200)		// bps


// These headers must be included for required defs.
#include "TCPIP Stack/TCPIP.h"
#if defined(__DEBUG)
	#include "test.h" // Olimex board test routines
#endif


// This is used by other stack elements.
// Main application must define this and initialize it with proper values.
APP_CONFIG AppConfig;
BYTE AN0String[8];
BYTE AN1String[8];
BYTE TEMPString[8];
BYTE tempCurrent = 0;
BYTE tempOld = 0;
#define STACK_USE_LED_BOARDS
#if defined(STACK_USE_LED_BOARDS)
	BYTE ledValue[4][4][3];
#endif
int secondsPassed = 0;
BYTE myDHCPBindCount = 0xFF;

#if !defined(STACK_USE_DHCP_CLIENT)
	#define DHCPBindCount	(0xFF)
#endif


// Set configuration fuses
#if defined(__18CXX)
	#if defined(__18F8722)
		// PICDEM HPC Explorer board
		#pragma config OSC=HSPLL, FCMEN=OFF, IESO=OFF, PWRT=OFF, WDT=OFF, LVP=OFF
	#elif defined(_18F8722)	// HI-TECH PICC-18 compiler
		// PICDEM HPC Explorer board
		__CONFIG(1, HSPLL);
		__CONFIG(2, WDTDIS);
		__CONFIG(3, MCLREN);
		__CONFIG(4, XINSTDIS & LVPDIS);
	#elif defined(__18F87J10) || defined(__18F86J15) || defined(__18F86J10) || defined(__18F85J15) || defined(__18F85J10) || defined(__18F67J10) || defined(__18F66J15) || defined(__18F66J10) || defined(__18F65J15) || defined(__18F65J10)
		// PICDEM HPC Explorer board
		#pragma config XINST=OFF, WDTEN=OFF, FOSC2=ON, FOSC=HSPLL
	#elif defined(__18F97J60) || defined(__18F96J65) || defined(__18F96J60) || defined(__18F87J60) || defined(__18F86J65) || defined(__18F86J60) || defined(__18F67J60) || defined(__18F66J65) || defined(__18F66J60) 
		// PICDEM.net 2 and PICDEM.net 2 mini boards
		#pragma config XINST=OFF, WDT=OFF, FOSC2=ON, FOSC=HSPLL, ETHLED=ON
	#elif defined(_18F97J60) || defined(_18F96J65) || defined(_18F96J60) || defined(_18F87J60) || defined(_18F86J65) || defined(_18F86J60) || defined(_18F67J60) || defined(_18F66J65) || defined(_18F66J60) 
		// PICDEM.net 2 board with HI-TECH PICC-18 compiler
		__CONFIG(1, WDTDIS & XINSTDIS);
		__CONFIG(2, HSPLL);
	#elif defined(__18F4620)	
		// PICDEM.net board
		#pragma config OSC=HS, WDT=OFF, MCLRE=ON, PBADEN=OFF, LVP=OFF, XINST=OFF
		// PICDEM Z board
		//#pragma config OSC=HSPLL, WDT=OFF, MCLRE=ON, PBADEN=OFF, LVP=OFF, XINST=OFF
	#elif defined(__18F452)	
		// PICDEM.net board
		#if defined(__DEBUG)
		    #pragma config OSC=HSPLL, WDT=OFF, LVP=OFF, PWRT=OFF
		#else
		    #pragma config OSC=HSPLL, WDT=OFF, LVP=OFF, PWRT=ON
		#endif
	#endif
#elif defined(__PIC24F__)
	// Explorer 16 board
	_CONFIG2(FNOSC_PRIPLL & POSCMOD_XT)		// Primary XT OSC with 4x PLL
	_CONFIG1(JTAGEN_OFF & FWDTEN_OFF)		// JTAG off, watchdog timer off
#elif defined(__dsPIC33F__) || defined(__PIC24H__)
	// Explorer 16 board
	_FOSCSEL(FNOSC_PRIPLL)			// PLL enabled
	_FOSC(OSCIOFNC_OFF & POSCMD_XT)	// XT Osc
	_FWDT(FWDTEN_OFF)				// Disable Watchdog timer
	// JTAG should be disabled as well
#elif defined(__dsPIC30F__)
	// dsPICDEM 1.1 board
	_FOSC(XT_PLL16)					// XT Osc + 16X PLL
	_FWDT(WDT_OFF)					// Disable Watchdog timer
	_FBORPOR(MCLR_EN & PBOR_OFF & PWRT_OFF)
#endif


// Private helper functions.
// These may or may not be present in all applications.
static void InitAppConfig(void);
static void InitializeBoard(void);
static void ProcessIO(void);
#if defined(STACK_USE_SMTP_CLIENT)
	static void SMTPDemo(void);
#endif
#if defined(STACK_USE_ICMP_CLIENT)
	static void PingDemo(void);
#endif

static void DisplayIPValue(IP_ADDR IPVal);
static void FormatNetBIOSName(BYTE Name[16]);

#if defined(STACK_USE_UART)
	static void SetConfig(void);
#endif

#if defined(MPFS_USE_EEPROM) && defined(STACK_USE_MPFS)
	static void SaveAppConfig(void);
	#if defined(STACK_USE_UART)
		static void DownloadMPFS(void);
	#endif
#else
	#define SaveAppConfig()
#endif


// NOTE: Several PICs, including the PIC18F4620 revision A3 have a RETFIE FAST/MOVFF bug
// The interruptlow keyword is used to work around the bug when using C18
extern void MACISR(void);
#if defined(HI_TECH_C)
void interrupt low_priority LowISR(void)
#else
#pragma interruptlow LowISR
void LowISR(void)
#endif
{
#ifdef __18CXX
    TickUpdate();
#endif

#if defined(STACK_USE_SLIP)	
    MACISR();
#endif
}

#if defined(__18CXX) && !defined(HI_TECH_C)
#pragma code lowVector=0x18
void LowVector(void){_asm goto LowISR _endasm}
#pragma code // Return to default code section
#endif


// Main application entry point.
#ifdef __C30__ 
int main(void)
#else
void main(void)
#endif
{
    static TICK t = 0;

	// Required to allow EEPROM to initialize
	Delay10KTCYx(0); // Delay of about 250mS

    // Initialize any application specific hardware.
    InitializeBoard();
    
	#ifdef USE_LCD
		// Initialize and display the stack version on the LCD
		LCDInit();
		DelayMs(100);
		strcpypgm2ram((char*)LCDText, "TCPStack " VERSION "  "
									  "                ");
		LCDUpdate();
	#endif

    // Initialize all stack related components.
    // Following steps must be performed for all applications using
    // the Microchip TCP/IP Stack.
    TickInit();

	#if defined(STACK_USE_MPFS)
	    // Initialize Microchip File System module
	    MPFSInit();
	#endif

    // Initialize Stack and application related NV variables into AppConfig.
    InitAppConfig();

    // Initiates board setup process if button is depressed 
	// on startup
	
	  if(BUTTON0_IO == 0u)
    {
		#if defined(MPFS_USE_EEPROM) && defined(STACK_USE_MPFS)
		// Invalidate the EEPROM contents if BUTTON0 is held down for more than 4 seconds
		TICK StartTime = TickGet();

		while(BUTTON0_IO == 0u)
		{
			if(TickGet() - StartTime > 4*TICK_SECOND)
			{
			    XEEBeginWrite(0x0000);
			    XEEWrite(0xFF);
			    XEEEndWrite();
				#if defined(STACK_USE_UART)
				putrsUART("\r\n\r\nBUTTON0 held for more than 4 seconds.  EEPROM contents erased.\r\n\r\n");
				#endif
				LED0_TRIS = 0;
				LED0_IO = 1;
				while((LONG)(TickGet() - StartTime) <= (LONG)(9*TICK_SECOND/2));
				Reset();
				break;
			}
		}
		#endif

		#if defined(STACK_USE_UART)
        SetConfig();
		#endif
    }

	// Initialize core stack layers (MAC, ARP, TCP, UDP)
    StackInit();
	
	#if defined(STACK_USE_HTTP_SERVER)
	    HTTPInit();
	#endif
	
	#if defined(STACK_USE_FTP_SERVER) && defined(MPFS_USE_EEPROM) && defined(STACK_USE_MPFS)
	    FTPInit();
	#endif
	
	#if defined(STACK_USE_SNMP_SERVER)
		SNMPInit();
	#endif
	
	#if defined(STACK_USE_DHCP_CLIENT) || defined(STACK_USE_IP_GLEANING)
	    if(!AppConfig.Flags.bIsDHCPEnabled)
	    {
	        // Force IP address display update.
	        myDHCPBindCount = 1;
	#if defined(STACK_USE_DHCP_CLIENT)
	        DHCPDisable();
	#endif
	    }
	#endif

	#if !defined(STACK_USE_LED_BOARDS)
	// Ivan - print out the IP adress
		#if defined(STACK_USE_UART)
			putrsUART((ROM char*)"New IP Addres: ");
		#endif
	    DisplayIPValue(AppConfig.MyIPAddr);	// Print to UART
		#if defined(STACK_USE_UART)
			putrsUART((ROM char*)"\r\n");
		#endif
	#endif

    // Once all items are initialized, go into infinite loop and let
    // stack items execute their tasks.
    // If application needs to perform its own task, it should be
    // done at the end of while loop.
    // Note that this is a "co-operative mult-tasking" mechanism
    // where every task performs its tasks (whether all in one shot
    // or part of it) and returns so that other tasks can do their
    // job.
    // If a task needs very long time to do its job, it must be broken
    // down into smaller pieces so that other tasks can have CPU time.
    while(1)
    {
        // This task performs normal stack task including checking
        // for incoming packet, type of packet and calling
        // appropriate stack entity to process it.
        StackTask();

		#if defined(STACK_USE_HTTP_SERVER)
		        // This is a TCP application.  It listens to TCP port 80
		        // with one or more sockets and responds to remote requests.
		        HTTPServer();
		#endif
		
		#if defined(STACK_USE_FTP_SERVER) && defined(MPFS_USE_EEPROM) && defined(STACK_USE_MPFS)
		        FTPServer();
		#endif
		
		#if defined(STACK_USE_SNMP_SERVER)
				SNMPTask();
		#endif
		
		#if defined(STACK_USE_ANNOUNCE)
				DiscoveryTask();
		#endif
		
		#if defined(STACK_USE_NBNS)
				NBNSTask();
		#endif
		
		#if defined(STACK_USE_DHCP_SERVER)
				DHCPServerTask();
		#endif
		
		#if defined(STACK_USE_GENERIC_TCP_CLIENT_EXAMPLE)
				GenericTCPClient();
		#endif
		
		#if defined(STACK_USE_GENERIC_TCP_SERVER_EXAMPLE)
				GenericTCPServer();
		#endif
		
		#if defined(STACK_USE_TELNET_SERVER)
				TelnetTask();
		#endif
		
		#if defined(STACK_USE_REBOOT_SERVER)
				RebootTask();
		#endif
		
		#if defined(STACK_USE_UDP_PERFORMANCE_TEST)
				UDPPerformanceTask();
		#endif
		
		#if defined(STACK_USE_TCP_PERFORMANCE_TEST)
				TCPPerformanceTask();
		#endif
		
		#if defined(STACK_USE_SMTP_CLIENT)
				SMTPTask();
				SMTPDemo();
		#endif
		
		#if defined(STACK_USE_ICMP_CLIENT)
				PingDemo();
		#endif

        // Add your application specific tasks here.
        ProcessIO();


        // For DHCP information, display how many times we have renewed the IP
        // configuration since last reset.
        if(DHCPBindCount != myDHCPBindCount)
        {
            myDHCPBindCount = DHCPBindCount;

			#if !defined(STACK_USE_LED_BOARDS)
				#if defined(STACK_USE_UART)
					putrsUART((ROM char*)"New IP Addres: ");
				#endif
	            DisplayIPValue(AppConfig.MyIPAddr);	// Print to UART
				#if defined(STACK_USE_UART)
					putrsUART((ROM char*)"\r\n");
				#endif
			#endif

			#if defined(STACK_USE_ANNOUNCE)
				AnnounceIP();
			#endif
        }

    }
}


static void DisplayIPValue(IP_ADDR IPVal)
{
//	printf("%u.%u.%u.%u", IPVal.v[0], IPVal.v[1], IPVal.v[2], IPVal.v[3]);
    BYTE IPDigit[4];
	BYTE i;
#ifdef USE_LCD
	BYTE j;
	BYTE LCDPos=16;
#endif

	for(i = 0; i < sizeof(IP_ADDR); i++)
	{
	    uitoa((WORD)IPVal.v[i], IPDigit);

		#if defined(STACK_USE_UART)
			putsUART(IPDigit);
		#endif

		#ifdef USE_LCD
			for(j = 0; j < strlen((char*)IPDigit); j++)
			{
				LCDText[LCDPos++] = IPDigit[j];
			}
			if(i == sizeof(IP_ADDR)-1)
				break;
			LCDText[LCDPos++] = '.';
		#else
			if(i == sizeof(IP_ADDR)-1)
				break;
		#endif

		#if defined(STACK_USE_UART)
			while(BusyUART());
			WriteUART('.');
		#endif
	}

	#ifdef USE_LCD
		if(LCDPos < 32)
			LCDText[LCDPos] = 0;
		LCDUpdate();
	#endif
}


static void ProcessIO(void)
{
	signed int temp;
    WORD_VAL ADCResult;

    /*
     * Select AN0 channel, Fosc/64 clock
     * Works for both compatible and regular A/D modules
     */
    ADCON0      = 0b10000001;

    /*
     * Wait for acquisition time.
     * Here, rather than waiting for exact time, a simple wait is
     * used.  Real applications requiring high accuracy should
     * calculate exact acquisition time and wait accordingly.
     */
    ADCResult.v[0] = 100;
    while( ADCResult.v[0]-- );

    /*
     * First convert AN0 channel.
     * AN0 is already setup as an analog input.
     */
    ADCON0bits.GO   = 1;

    /*
     * Wait until conversion is done.
     */
    while( ADCON0bits.GO );

    /*
     * Save the result.
     */
    ADCResult.v[0] = ADRESL;
    ADCResult.v[1] = ADRESH;

    /*
     * Convert 10-bit value into ASCII String.
     */
    itoa(ADCResult.Val, AN0String);

    /*
     * Now, convert AN1 channel.
     *
     * In PICDEM.net board, RA2 thru RA7 should be digital or else
     * LED, LCD and NIC would not operate correctly.
     * Since there is no mode where only AN0 and AN1 be analog inputs
     * while rests are digial pins, we will temperoraily switch
     * select a mode where RA2 becomes analog input while we do
     * conversion of RA1.  Once conversion is done, we will convert
     * RA2 back to digital pin.
     */
//#if defined(USE_COMPATIBLE_AD)
	// Change AN1 to be an analog input
    ADCON1      = 0b11000100;

    // Select AN1 channel.
    ADCON0      = 0b10001001;
//#else
//    // Select AN1 channel.
//	ADCON0 		= 0b00000101;
//#endif


    /*
     * Wait for acquisition time.
     * Here, rather than waiting for exact time, a simple wait is
     * used.  Real applications requiring high accuracy should
     * calculate exact acquisition time and wait accordingly.
     */
    ADCResult.v[0] = 100;
    while( ADCResult.v[0]-- );

    /*
     * Start the conversion.
     */
    ADCON0bits.GO   = 1;

    /*
     * Wait until it is done.
     */
    while( ADCON0bits.GO );

    /*
     * Save the result.
     */
    ADCResult.v[0] = ADRESL;
    ADCResult.v[1] = ADRESH;

	temp = ADCResult.Val;

	if(temp > 512) {
		temp = temp - 512;
		temp /= 9;		
		temp = 20 + temp;
	}
	else {
		temp = 512 - temp;
		temp /= 9;		
		temp = 20 - temp;
	}


    /*
     * Convert 10-bit value into ASCII String.
     */
    itoa(ADCResult.Val, AN1String);
	itoa(temp, TEMPString);
	

    /*
     * Reset RA2 pin back to digital output.
     */
    
    ADCON1      = 0b11001110;       // RA0 as analog input.

}


#if defined(STACK_USE_SMTP_CLIENT)
static void SMTPDemo(void)
{
	// Send an email once if someone pushes BUTTON0
	// This is a multi-part message example, where the message 
	// body is dynamically generated and need not fit in RAM.
	// LED1 will be used as a busy indicator
	// LED2 will be used as a mail sent successfully indicator
	static enum
	{
		MAIL_HOME = 0,
		MAIL_BEGIN,
		MAIL_PUT_DATA,
		MAIL_PUT_DATA2,
		MAIL_SMTP_FINISHING,
		MAIL_DONE
	} MailState = MAIL_HOME;
	static BYTE *MemPtr;
	static TICK WaitTime;
	     
	switch(MailState)
	{
		case MAIL_HOME:
		    if(BUTTON0_IO == 0u)
			{
				// Start sending an email
				LED1_IO = 1;
				MailState++;
				LED2_IO = 0;
			}
			break;
		
		case MAIL_BEGIN:
			if(SMTPBeginUsage())
			{
				// Note that these strings must stay allocated in 
				// memory until SMTPIsBusy() returns FALSE.  To 
				// guarantee that the C compiler does not reuse this 
				// memory, you must allocate the strings as static.

				//static BYTE RAMStringServer[] = "mail";	// SMTP server address
				static BYTE RAMStringTo[] = "joe@picsaregood.com";
				//static BYTE RAMStringCC[] = "foo@picsaregood.com, \"Jane Smith\" <jane.smith@picsaregood.com>";
		
				//SMTPClient.Server.szRAM = RAMStringServer;
				SMTPClient.To.szRAM = RAMStringTo;
				//SMTPClient.CC.szRAM = RAMStringCC;
				SMTPClient.From.szROM = (ROM BYTE*)"\"SMTP Service\" <mchpboard@picsaregood.com>";
				SMTPClient.ROMPointers.From = 1;
				SMTPClient.Subject.szROM = (ROM BYTE*)"Hello world!  SMTP Test.";
				SMTPClient.ROMPointers.Subject = 1;
				SMTPSendMail();
				MailState++;
			}
			break;
		
		case MAIL_PUT_DATA:
			// Check to see if a failure occured
			if(!SMTPIsBusy())
			{
				// Finished sending mail
				LED1_IO = 0;
				MailState = MAIL_DONE;
				WaitTime = TickGet();
				LED2_IO = (SMTPEndUsage() == SMTP_SUCCESS);
				break;
			}
		
			if(SMTPIsPutReady() >= 121u)
			{
				SMTPPutROMString((ROM BYTE*)"Hello!\r\n\r\nThis mail was automatically generated by Microchip TCP/IP Stack " VERSION ".\r\n\r\nThe following is a snapshot of RAM:\r\n");
				SMTPFlush();
				
				MemPtr = 0x0000;
				MailState++;
			}
			break;
		
		case MAIL_PUT_DATA2:
			// Check to see if a failure occured
			if(!SMTPIsBusy())
			{
				// Finished sending mail
				LED1_IO = 0;
				MailState = MAIL_DONE;
				WaitTime = TickGet();
				LED2_IO = (SMTPEndUsage() == SMTP_SUCCESS);
				break;
			}
		
			if(SMTPIsPutReady() >= 75u)
			{
				BYTE i, c;
				WORD_VAL w;
		
				// Write line address
				w.Val = (WORD)MemPtr;
				SMTPPut(btohexa_high(w.v[1]));
				SMTPPut(btohexa_low(w.v[1]));
				SMTPPut(btohexa_high(w.v[0]));
				SMTPPut(btohexa_low(w.v[0]));
				SMTPPut(' ');
		
				// Write data bytes in hex
				for(i = 0; i < 16u; i++)
				{
					SMTPPut(' ');
					c = *MemPtr++;
					SMTPPut(btohexa_high(c));
					SMTPPut(btohexa_low(c));
					if(i == 7u)
						SMTPPut(' ');
				}
		
				SMTPPut(' ');
				SMTPPut(' ');
		
				// Write data bytes in ASCII
				MemPtr -= 16;
				for(i = 0; i < 16u; i++)
				{
					c = *MemPtr++; 
					if(c < ' ' || c > '~')
						c = '.';
					SMTPPut(c);
		
					if(i == 7u)
						SMTPPut(' ');
				}
		
				SMTPPut('\r');
				SMTPPut('\n');
				SMTPFlush();
		
				// Make sure not to read from memory above address 0x0E7F.
				// Doing so would disrupt volatile pointers, ERDPT, FSR0, FSR1, FSR2, etc.
				if((WORD)MemPtr >= 0xE7Fu)
				{
					SMTPPutDone();
					MailState++;
				}
			}
			break;
		
		case MAIL_SMTP_FINISHING:
			// Check to see if we are done communicating with the SMTP server
			if(!SMTPIsBusy())
			{
				// Finished sending mail
				LED1_IO = 0;
				MailState = MAIL_DONE;
				WaitTime = TickGet();
				LED2_IO = (SMTPEndUsage() == SMTP_SUCCESS);
			}
			break;
		
		case MAIL_DONE:
			// Wait for the user to release BUTTON0 and for at 
			// least 1 second to pass before allowing another 
			// email to be sent.  This is merely to prevent 
			// accidental flooding of email boxes while 
			// developing code.  Your application may wish to 
			// remove this.
			if(BUTTON0_IO)
			{
				if(TickGet() - WaitTime > TICK_SECOND)
					MailState = MAIL_HOME;
			}
			break;
	}
}
#endif //#if defined(STACK_USE_SMTP_CLIENT)


// ICMP Echo (Ping) example code
#if defined(STACK_USE_ICMP_CLIENT)
static void PingDemo(void)
{
	static enum
	{
		SM_HOME = 0,
		SM_GET_RESPONSE
	} PingState = SM_HOME;
	static TICK Timer;
	SHORT ret;
	IP_ADDR RemoteIP;

	switch(PingState)
	{
		case SM_HOME:
			// Send a ping request out if the user pushes BUTTON3 (left-most one)
			if(BUTTON3_IO == 0)
			{
				// Don't ping flood: wait at least 1 second between ping requests
				if((DWORD)(TickGet()-Timer) > 1u*TICK_SECOND)
				{
					// Obtain ownership of the ICMP module
					if(ICMPBeginUsage())
					{
						Timer = TickGet();
						PingState = SM_GET_RESPONSE;
	
						// Send the ping request to 4.78.194.159 (ww1.microchip.com)
						RemoteIP.v[0] = 4;
						RemoteIP.v[1] = 78;
						RemoteIP.v[2] = 194;
						RemoteIP.v[3] = 159;
						ICMPSendPing(RemoteIP.Val);
					}
				}
			}
			break;

		case SM_GET_RESPONSE:
			// Get the status of the ICMP module
			ret = ICMPGetReply();					
			if(ret == -2)
			{
				// Do nothing: still waiting for echo
				break;
			}
			else if(ret == -1)
			{
				// Request timed out
				#if defined(USE_LCD)
				memcpypgm2ram((void*)&LCDText[16], (ROM void *)"Ping timed out", 15);
				LCDUpdate();
				#endif
				PingState = SM_HOME;
			}
			else
			{
				// Echo received.  Time elapsed is stored in ret (units of TICK).
				#if defined(USE_LCD)
				memcpypgm2ram((void*)&LCDText[16], (ROM void *)"Reply: ", 7);
				uitoa(ret, &LCDText[16+7]);
				strcatpgm2ram((char*)&LCDText[16+7], (ROM char*)"ms");
				LCDUpdate();
				#endif
				PingState = SM_HOME;
			}
			
			// Finished with the ICMP module, release it so other apps can begin using it
			ICMPEndUsage();
			break;
	}
}
#endif	//#if defined(STACK_USE_ICMP_CLIENT)

#if defined(STACK_USE_LED_BOARDS)
	signed int Char2Num(unsigned char c)
	{
	   if (c < '0') return -1;
	   if (c <= '9') return c - '0';
	   if (c < 'A') return -1;
	   if (c <= 'F') return c - 'A' + 10;
	   if (c < 'a') return -1;
	   if (c <= 'f') return c - 'a' + 10;
	   return -1;
	}
#endif 

// CGI Command Codes
#define CGI_CMD_DIGOUT      (0)
#define CGI_CMD_LCDOUT      (1)
#define CGI_CMD_RECONFIG	(2)
#define CGI_CMD_LEDOUT      (3)

// CGI Variable codes. - There could be 00h-FFh variables.
// NOTE: When specifying variables in your dynamic pages (.cgi),
//       use the hexadecimal numbering scheme and always zero pad it
//       to be exactly two characters.  Eg: "%04", "%2C"; not "%4" or "%02C"
#define VAR_LED0			(0x00)	// LED Outputs
#define VAR_LED1			(0x01)
#define VAR_LED2			(0x10)
#define VAR_LED3			(0x11)
#define VAR_LED4			(0x12)
#define VAR_LED5			(0x13)
#define VAR_LED6			(0x14)
#define VAR_LED7			(0x15)
#define VAR_ANAIN_AN0       (0x02)	// Analog Inputs (POT, temp, etc)
#define VAR_ANAIN_AN1       (0x03)
#define VAR_TEMP		    (0x18)
#define VAR_DIGIN0       	(0x04)	// Momentary push button inputs
#define VAR_DIGIN1       	(0x0D)
#define VAR_DIGIN2       	(0x0E)
#define VAR_DIGIN3       	(0x0F)
#define VAR_STACK_VERSION	(0x16)	// Stack constants
#define VAR_STACK_DATE		(0x17)
#define VAR_STROUT_LCD      (0x05)	// LCD Display output
#define VAR_MAC_ADDRESS     (0x06)	// Stack configuration variables
#define VAR_SERIAL_NUMBER   (0x07)
#define VAR_IP_ADDRESS      (0x08)
#define VAR_SUBNET_MASK     (0x09)
#define VAR_GATEWAY_ADDRESS (0x0A)
#define VAR_DHCP	        (0x0B)	// Use this variable when the web page is updating us
#define VAR_DHCP_TRUE       (0x0B)	// Use this variable when we are generating the web page
#define VAR_DHCP_FALSE      (0x0C)	// Use this variable when we are generating the web page
#define VAR_LEDVALUES0		(0x20)  // LED Values for Board 0
#define VAR_LEDVALUES1		(0x21)  // LED Values for Board 1
#define VAR_LEDVALUES2		(0x22)  // LED Values for Board 2
#define VAR_LEDVALUES3		(0x23)  // LED Values for Board 3
#define VAR_LEDVALUES00		(0x40)  // LED Value 0 for Board 0
#define VAR_LEDVALUES01		(0x41)  // LED Value 1 for Board 0
#define VAR_LEDVALUES02		(0x42)  // LED Value 2 for Board 0
#define VAR_LEDVALUES03		(0x43)  // LED Value 3 for Board 0
#define VAR_LEDVALUES10		(0x50)  // LED Value 0 for Board 0
#define VAR_LEDVALUES11		(0x51)  // LED Value 1 for Board 0
#define VAR_LEDVALUES12		(0x52)  // LED Value 2 for Board 0
#define VAR_LEDVALUES13		(0x53)  // LED Value 3 for Board 0
#define VAR_LEDVALUES20		(0x60)  // LED Value 0 for Board 0
#define VAR_LEDVALUES21		(0x61)  // LED Value 1 for Board 0
#define VAR_LEDVALUES22		(0x62)  // LED Value 2 for Board 0
#define VAR_LEDVALUES23		(0x63)  // LED Value 3 for Board 0
#define VAR_LEDVALUES30		(0x70)  // LED Value 0 for Board 0
#define VAR_LEDVALUES31		(0x71)  // LED Value 1 for Board 0
#define VAR_LEDVALUES32		(0x72)  // LED Value 2 for Board 0
#define VAR_LEDVALUES33		(0x73)  // LED Value 3 for Board 0


// CGI Command codes (CGI_CMD_DIGOUT).
// Should be a one digit numerical value
#define CMD_LED1			(0x0)
#define CMD_LED2			(0x1)
#define CMD_LEDOUT1			(0x01)
#define CMD_LEDOUT2			(0x02)
#define CMD_LEDOUT3			(0x03)
#define CMD_LEDOUT4			(0x04)
#define CMD_LEDOUTALL		(0xFF)


/*********************************************************************
 * Function:        void HTTPExecCmd(BYTE** argv, BYTE argc)
 *
 * PreCondition:    None
 *
 * Input:           argv        - List of arguments
 *                  argc        - Argument count.
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a "callback" from HTTPServer
 *                  task.  Whenever a remote node performs
 *                  interactive task on page that was served,
 *                  HTTPServer calls this functions with action
 *                  arguments info.
 *                  Main application should interpret this argument
 *                  and act accordingly.
 *
 *                  Following is the format of argv:
 *                  If HTTP action was : thank.htm?name=Joe&age=25
 *                      argv[0] => thank.htm
 *                      argv[1] => name
 *                      argv[2] => Joe
 *                      argv[3] => age
 *                      argv[4] => 25
 *
 *                  Use argv[0] as a command identifier and rests
 *                  of the items as command arguments.
 *
 * Note:            THIS IS AN EXAMPLE CALLBACK.
 ********************************************************************/
#if defined(STACK_USE_HTTP_SERVER)

ROM char COMMANDS_OK_PAGE[] = "INDEX.CGI";
ROM char CONFIG_UPDATE_PAGE[] = "CONFIG.CGI";
ROM char CMD_UNKNOWN_PAGE[] = "INDEX.CGI";

// Copy string with NULL termination.
#define COMMANDS_OK_PAGE_LEN  	(sizeof(COMMANDS_OK_PAGE))
#define CONFIG_UPDATE_PAGE_LEN  (sizeof(CONFIG_UPDATE_PAGE))
#define CMD_UNKNOWN_PAGE_LEN    (sizeof(CMD_UNKNOWN_PAGE))

void HTTPExecCmd(BYTE** argv, BYTE argc)
{
    BYTE command;
    BYTE var;
	#if defined(ENABLE_REMOTE_CONFIG)
		DWORD_VAL dwVal;
	    BYTE CurrentArg;
	    WORD_VAL TmpWord;
	#endif
	BYTE x, y, c, boardLEDx, boardLEDy;
    /*
     * Design your pages such that they contain command code
     * as a one character numerical value.
     * Being a one character numerical value greatly simplifies
     * the job.
     */
    command = argv[0][0] - '0';

    /*
     * Find out the cgi file name and interpret parameters
     * accordingly
     */
    switch(command)
    {
    case CGI_CMD_DIGOUT:	// ACTION=0
        /*
         * Identify the parameters.
         * Compare it in upper case format.
         */
        var = argv[1][0] - '0';

        switch(var)
        {
        case CMD_LED1:	// NAME=0
            // Toggle LED.
            LED0_IO ^= 1;
            break;
         }

         memcpypgm2ram((void*)argv[0], (ROM void*)COMMANDS_OK_PAGE, COMMANDS_OK_PAGE_LEN);
         break;

	#if defined(USE_LCD)
	    case CGI_CMD_LCDOUT:	// ACTION=1
			if(argc > 2u)	// Text provided in argv[2]
			{
				// Convert %20 to spaces, and other URL transformations
				UnencodeURL(argv[2]);
	
				// Write 32 received characters or less to LCDText
				if(strlen((char*)argv[2]) < 32u)
				{
					memset(LCDText, ' ', 32);
					strcpy((char*)LCDText, (char*)argv[2]);
				}
				else
				{
					memcpy(LCDText, (void*)argv[2], 32);
				}
	
				// Write LCDText to the LCD
				LCDUpdate();
			}
			else			// No text provided
			{
				LCDErase();
			}
			memcpypgm2ram((void*)argv[0], (ROM void*)COMMANDS_OK_PAGE, COMMANDS_OK_PAGE_LEN);
	        break;
	#endif

	#if defined(ENABLE_REMOTE_CONFIG)
	// Possibly useful code for remotely reconfiguring the board through 
	// HTTP
		case CGI_CMD_RECONFIG:	// ACTION=2
			// Loop through all variables that we've been given
			CurrentArg = 1;
			while(argc > CurrentArg)
			{
				// Get the variable identifier (HTML "name"), and 
				// increment to the variable's value
				TmpWord.byte.MSB = argv[CurrentArg][0];
				TmpWord.byte.LSB = argv[CurrentArg++][1];
		        var = hexatob(TmpWord);
		        
		        // Make sure the variable's value exists
		        if(CurrentArg >= argc)
		        	break;
		        
		        // Take action with this variable/value
		        switch(var)
		        {
		        case VAR_IP_ADDRESS:
		        case VAR_SUBNET_MASK:
		        case VAR_GATEWAY_ADDRESS:
		        	{
			        	// Convert the returned value to the 4 octect 
			        	// binary representation
				        if(!StringToIPAddress(argv[CurrentArg], (IP_ADDR*)&dwVal))
				        	break;
	
						// Reconfigure the App to use the new values
				        if(var == VAR_IP_ADDRESS)
				        {
					        // Cause the IP address to be rebroadcast
					        // through Announce.c or the RS232 port since
					        // we now have a new IP address
					        if(dwVal.Val != *(DWORD*)&AppConfig.MyIPAddr)
						        DHCPBindCount++;
						    
						    // Set the new address
				        	memcpy((void*)&AppConfig.MyIPAddr, (void*)&dwVal, sizeof(AppConfig.MyIPAddr));
				        }
				        else if(var == VAR_SUBNET_MASK)
				        	memcpy((void*)&AppConfig.MyMask, (void*)&dwVal, sizeof(AppConfig.MyMask));
				        else if(var == VAR_GATEWAY_ADDRESS)
				        	memcpy((void*)&AppConfig.MyGateway, (void*)&dwVal, sizeof(AppConfig.MyGateway));
			        }
		            break;
		
		        case VAR_DHCP:
		        	if(AppConfig.Flags.bIsDHCPEnabled)
		        	{
			        	if(!(argv[CurrentArg][0]-'0'))
			        	{
			        		AppConfig.Flags.bIsDHCPEnabled = FALSE;
			        	}
			        }
			        else
		        	{
			        	if(argv[CurrentArg][0]-'0')
			        	{
							AppConfig.MyIPAddr.Val = 0x00000000ul;
			        		AppConfig.Flags.bIsDHCPEnabled = TRUE;
					        AppConfig.Flags.bInConfigMode = TRUE;
				        	DHCPReset();
			        	}
			        }
		            break;
		    	}
	
				// Advance to the next variable (if present)
				CurrentArg++;	
	        }
			
			// Save any changes to non-volatile memory
	      	SaveAppConfig();
	
			// Return the same CONFIG.CGI file as a result.
	        memcpypgm2ram((void*)argv[0],
	             (ROM void*)CONFIG_UPDATE_PAGE, CONFIG_UPDATE_PAGE_LEN);
			break;
	#endif
	
	#if defined(STACK_USE_LED_BOARDS)
	    case CGI_CMD_LEDOUT:	// ACTION=3
			if(argc > 2u)	// Text provided in argv[2]
			{
				var = 0;
				//putsUART((char*)argv[2]);
				c = 0;
				while(argv[2][c] != '\0')
				{  // Transmit a byte
					while(BusyUSART());
					putcUART((Char2Num(argv[2][c])<<4)+Char2Num(argv[2][c+1]));
					c=c+2;
				};

				if(strlen((char*)argv[2]) < 32u)
				{
					switch(Char2Num(argv[2][0]))
    				{
    					case 1:
							var = (Char2Num(argv[2][14]) << 4) + Char2Num(argv[2][15]);
						break;
    					case 2:
							var = (Char2Num(argv[2][4]) << 4) + Char2Num(argv[2][5]);
						break;
    					case 8:
							var = (Char2Num(argv[2][26]) << 4) + Char2Num(argv[2][27]);
						break;
    					case 9:
							var = (Char2Num(argv[2][8]) << 4) + Char2Num(argv[2][9]);
						break;
					}

					switch(var)
					{
						case CMD_LEDOUT1:
							boardLEDx = 0; boardLEDy = 0;
						break;
						case CMD_LEDOUT2:
							boardLEDx = 2; boardLEDy = 0;
						break;
						case CMD_LEDOUT3:
							boardLEDx = 0; boardLEDy = 2;
						break;
						case CMD_LEDOUT4:
							boardLEDx = 2; boardLEDy = 2;
						break;
						case CMD_LEDOUTALL:
							boardLEDx = 0; boardLEDy = 0;
						break;
					}

					switch(Char2Num(argv[2][0]))
    				{
    					case 1:
							/*Update individual LEDs using 4-bit exponential update
								Byte 1, Nibble 1 = led1Red;   Byte 1, Nibble 2 = led1Green
								Byte 2, Nibble 1 = led1Blue;  Byte 2, Nibble 2 = led2Red
								Byte 3, Nibble 1 = led2Green; Byte 3, Nibble 2 = led2Blue
								Byte 4, Nibble 1 = led3Red;   Byte 4, Nibble 2 = led3Green
								Byte 5, Nibble 1 = led3Blue;  Byte 5, Nibble 2 = led4Red
								Byte 6, Nibble 1 = led4Green; Byte 6, Nibble 2 = led4Blue
								Byte 7 = theAddress
							*/
							ledValue[boardLEDx][boardLEDy][0] = Char2Num(argv[2][2]) << 4;
							ledValue[boardLEDx][boardLEDy][1] = Char2Num(argv[2][3]) << 4;
							ledValue[boardLEDx][boardLEDy][2] = Char2Num(argv[2][4]) << 4;
							ledValue[boardLEDx+1][boardLEDy][0] = Char2Num(argv[2][5]) << 4;
							ledValue[boardLEDx+1][boardLEDy][1] = Char2Num(argv[2][6]) << 4;
							ledValue[boardLEDx+1][boardLEDy][2] = Char2Num(argv[2][7]) << 4;
							ledValue[boardLEDx][boardLEDy+1][0] = Char2Num(argv[2][8]) << 4;
							ledValue[boardLEDx][boardLEDy+1][1] = Char2Num(argv[2][9]) << 4;
							ledValue[boardLEDx][boardLEDy+1][2] = Char2Num(argv[2][10]) << 4;
							ledValue[boardLEDx+1][boardLEDy+1][0] = Char2Num(argv[2][11]) << 4;
							ledValue[boardLEDx+1][boardLEDy+1][1] = Char2Num(argv[2][12]) << 4;
							ledValue[boardLEDx+1][boardLEDy+1][2] = Char2Num(argv[2][13]) << 4;
				        break;

    					case 2:
							/*Update all LEDs using 4-bit exponential update
								Byte 0, Nibble 2 = led1Red = led2Red = led3Red = led4Red
								Byte 1, Nibble 1 = led1Green = led2Green = led3Green = led4Green
								Byte 1, Nibble 2 = led1Blue = led2Blue = led3Blue = led4Blue
								Byte 2 = theAddress
							*/
							for (y=boardLEDy; y<boardLEDy+2; y++)
							{
								for (x=boardLEDx; x<boardLEDx+2; x++)
								{
									ledValue[x][y][0] = Char2Num(argv[2][1]) << 4;
									ledValue[x][y][1] = Char2Num(argv[2][2]) << 4;
									ledValue[x][y][2] = Char2Num(argv[2][3]) << 4;
								}
							}
				        break;

    					case 8:
							/* Update individual LEDs using 6-bit update
								Byte 1 = led1Red;    Byte 2 = led1Green
								Byte 3 = led1Blue;   Byte 4 = led2Red
								Byte 5 = led2Green;  Byte 6 = led2Blue
								Byte 7 = led3Red;    Byte 8 = led3Green
								Byte 9 = led3Blue;   Byte 10 = led4Red
								Byte 11 = led4Green; Byte 12 = led4Blue
								Byte 13 = theAddress
							*/
							ledValue[boardLEDx][boardLEDy][0] = (Char2Num(argv[2][2]) << 4) + Char2Num(argv[2][3]) << 2;
							ledValue[boardLEDx][boardLEDy][1] = (Char2Num(argv[2][4]) << 4) + Char2Num(argv[2][5]) << 2;
							ledValue[boardLEDx][boardLEDy][2] = (Char2Num(argv[2][6]) << 4) + Char2Num(argv[2][7]) << 2;
							ledValue[boardLEDx+1][boardLEDy][0] = (Char2Num(argv[2][8]) << 4) + Char2Num(argv[2][9]) << 2;
							ledValue[boardLEDx+1][boardLEDy][1] = (Char2Num(argv[2][10]) << 4) + Char2Num(argv[2][11]) << 2;
							ledValue[boardLEDx+1][boardLEDy][2] = (Char2Num(argv[2][12]) << 4) + Char2Num(argv[2][13]) << 2;
							ledValue[boardLEDx][boardLEDy+1][0] = (Char2Num(argv[2][14]) << 4) + Char2Num(argv[2][15]) << 2;
							ledValue[boardLEDx][boardLEDy+1][1] = (Char2Num(argv[2][16]) << 4) + Char2Num(argv[2][17]) << 2;
							ledValue[boardLEDx][boardLEDy+1][2] = (Char2Num(argv[2][18]) << 4) + Char2Num(argv[2][19]) << 2;
							ledValue[boardLEDx+1][boardLEDy+1][0] = (Char2Num(argv[2][20]) << 4) + Char2Num(argv[2][21]) << 2;
							ledValue[boardLEDx+1][boardLEDy+1][1] = (Char2Num(argv[2][22]) << 4) + Char2Num(argv[2][23]) << 2;
							ledValue[boardLEDx+1][boardLEDy+1][2] = (Char2Num(argv[2][24]) << 4) + Char2Num(argv[2][25]) << 2;
				        break;

    					case 9:
							/* Update all LEDs using 6-bit update
								Byte 1 = led1Red = led2Red = led3Red = led4Red
								Byte 2 = led1Green = led2Green = led3Green = led4Green
								Byte 3 = led1Blue = led2Blue = led3Blue = led4Blue
								Byte 4 = theAddress
							*/
							for (y=boardLEDy; y<boardLEDy+2; y++)
							{
								for (x=boardLEDx; x<boardLEDx+2; x++)
								{
									ledValue[x][y][0] = (Char2Num(argv[2][2]) << 4) + Char2Num(argv[2][3]) << 2;
									ledValue[x][y][1] = (Char2Num(argv[2][4]) << 4) + Char2Num(argv[2][5]) << 2;
									ledValue[x][y][2] = (Char2Num(argv[2][6]) << 4) + Char2Num(argv[2][7]) << 2;
								}
							}
				        break;
					}

					// If the address byte was 0xFF, then we want to copy the
					// values from the first board to the other 3
					if (var == CMD_LEDOUTALL)
					{
						for (y=boardLEDy; y<boardLEDy+2; y++)
						{
							for (x=boardLEDx; x<boardLEDx+2; x++)
							{
								ledValue[x+2][y][0] = ledValue[x][y][0];
								ledValue[x+2][y][1] = ledValue[x][y][1];
								ledValue[x+2][y][2] = ledValue[x][y][2];
								ledValue[x][y+2][0] = ledValue[x][y][0];
								ledValue[x][y+2][1] = ledValue[x][y][1];
								ledValue[x][y+2][2] = ledValue[x][y][2];
								ledValue[x+2][y+2][0] = ledValue[x][y][0];
								ledValue[x+2][y+2][1] = ledValue[x][y][1];
								ledValue[x+2][y+2][2] = ledValue[x][y][2];
							}
						}
					}
				}
				else
				{
					putcUART(0x10);
					putcUART(0x00);
					putcUART(0xFF);
				}
			}
			memcpypgm2ram((void*)argv[0], (ROM void*)COMMANDS_OK_PAGE, COMMANDS_OK_PAGE_LEN);
	        break;
	#endif

    default:
		memcpypgm2ram((void*)argv[0], (ROM void*)COMMANDS_OK_PAGE, COMMANDS_OK_PAGE_LEN);
        break;
    }

}
#endif


/*********************************************************************
 * Function:        WORD HTTPGetVar(BYTE var, WORD ref, BYTE* val)
 *
 * PreCondition:    None
 *
 * Input:           var         - Variable Identifier
 *                  ref         - Current callback reference with
 *                                respect to 'var' variable.
 *                  val         - Buffer for value storage.
 *
 * Output:          Variable reference as required by application.
 *
 * Side Effects:    None
 *
 * Overview:        This is a callback function from HTTPServer() to
 *                  main application.
 *                  Whenever a variable substitution is required
 *                  on any html pages, HTTPServer calls this function
 *                  8-bit variable identifier, variable reference,
 *                  which indicates whether this is a first call or
 *                  not.  Application should return one character
 *                  at a time as a variable value.
 *
 * Note:            Since this function only allows one character
 *                  to be returned at a time as part of variable
 *                  value, HTTPServer() calls this function
 *                  multiple times until main application indicates
 *                  that there is no more value left for this
 *                  variable.
 *                  On begining, HTTPGetVar() is called with
 *                  ref = HTTP_START_OF_VAR to indicate that
 *                  this is a first call.  Application should
 *                  use this reference to start the variable value
 *                  extraction and return updated reference.  If
 *                  there is no more values left for this variable
 *                  application should send HTTP_END_OF_VAR.  If
 *                  there are any bytes to send, application should
 *                  return other than HTTP_START_OF_VAR and
 *                  HTTP_END_OF_VAR reference.
 *
 *                  THIS IS AN EXAMPLE CALLBACK.
 *                  MODIFY THIS AS PER YOUR REQUIREMENTS.
 ********************************************************************/
#if defined(STACK_USE_HTTP_SERVER)
WORD HTTPGetVar(BYTE var, WORD ref, BYTE* val)
{
	// Temporary variables designated for storage of a whole return 
	// result to simplify logic needed since one byte must be returned
	// at a time.
	static BYTE VarString[64];
#if defined(ENABLE_REMOTE_CONFIG)
	static BYTE VarStringLen;
	BYTE *VarStringPtr;

	BYTE i;
	BYTE *DataSource;
#endif
	BYTE x, y, xMin, xMax, yMin, yMax, c, nibble;
	BYTE colorDigit[4];
	
	// Identify variable
    switch(var)
    {
    case VAR_LED0:
        *val = LED0_IO ? '1':'0';
        break;

    case VAR_ANAIN_AN0:
        *val = AN0String[(BYTE)ref];
        if(AN0String[(BYTE)ref] == '\0')
            return HTTP_END_OF_VAR;
		else if(AN0String[(BYTE)++ref] == '\0' )
            return HTTP_END_OF_VAR;
        return ref;

    case VAR_ANAIN_AN1:
        *val = AN1String[(BYTE)ref];
        if(AN1String[(BYTE)ref] == '\0')
            return HTTP_END_OF_VAR;
		else if(AN1String[(BYTE)++ref] == '\0' )
            return HTTP_END_OF_VAR;
        return ref;

	case VAR_TEMP:
        *val = TEMPString[(BYTE)ref];
        if(TEMPString[(BYTE)ref] == '\0')
            return HTTP_END_OF_VAR;
				else if(TEMPString[(BYTE)++ref] == '\0' )
            return HTTP_END_OF_VAR;
        return ref;

    case VAR_DIGIN0:
        *val = BUTTON0_IO ? '1':'0';
        break;

	case VAR_STACK_VERSION:
        if(ref == HTTP_START_OF_VAR)
		{
			strncpypgm2ram((char*)VarString, VERSION, sizeof(VarString));
		}
        *val = VarString[(BYTE)ref];
        if(VarString[(BYTE)ref] == '\0')
            return HTTP_END_OF_VAR;
		else if(VarString[(BYTE)++ref] == '\0' )
            return HTTP_END_OF_VAR;
        return ref;

	case VAR_STACK_DATE:
        if(ref == HTTP_START_OF_VAR)
		{
			strncpypgm2ram((char*)VarString, __DATE__ " " __TIME__, sizeof(VarString));
		}
        *val = VarString[(BYTE)ref];
        if(VarString[(BYTE)ref] == '\0')
            return HTTP_END_OF_VAR;
		else if(VarString[(BYTE)++ref] == '\0' )
            return HTTP_END_OF_VAR;
        return ref;

#if defined(ENABLE_REMOTE_CONFIG)
    case VAR_MAC_ADDRESS:
        if ( ref == HTTP_START_OF_VAR )
        {
            VarStringLen = 2*6+5;	// 17 bytes: 2 for each of the 6 address bytes + 5 octet spacers

	        // Format the entire string
            i = 0;
            VarStringPtr = VarString;
            while(1)
            {
	            *VarStringPtr++ = btohexa_high(AppConfig.MyMACAddr.v[i]);
	            *VarStringPtr++ = btohexa_low(AppConfig.MyMACAddr.v[i]);
	            if(++i == 6)
	            	break;
	            *VarStringPtr++ = '-';
	        }
        }

		// Send one byte back to the calling function (the HTTP Server)
		*val = VarString[(BYTE)ref];
		
        if ( (BYTE)++ref == VarStringLen )
            return HTTP_END_OF_VAR;

        return ref;
    		
    case VAR_IP_ADDRESS:
    case VAR_SUBNET_MASK:
    case VAR_GATEWAY_ADDRESS:
    	// Check if ref == 0 meaning that the first character of this 
    	// variable needs to be returned
        if ( ref == HTTP_START_OF_VAR )
        {
	        // Decide which 4 variable bytes to send back
	        if(var == VAR_IP_ADDRESS)
		    	DataSource = (BYTE*)&AppConfig.MyIPAddr;
		    else if(var == VAR_SUBNET_MASK)
		    	DataSource = (BYTE*)&AppConfig.MyMask;
		    else if(var == VAR_GATEWAY_ADDRESS)
		    	DataSource = (BYTE*)&AppConfig.MyGateway;
	        
	        // Format the entire string
	        VarStringPtr = VarString;
	        i = 0;
	        while(1)
	        {
		        uitoa((WORD)*DataSource++, VarStringPtr);
		        VarStringPtr += strlen(VarStringPtr);
		        if(++i == 4)
		        	break;
		        *VarStringPtr++ = '.';
		    }
		    VarStringLen = strlen(VarString);
        }

		// Send one byte back to the calling function (the HTTP Server)
		*val = VarString[(BYTE)ref];
		
		// If this is the last byte to be returned, return 
		// HTTP_END_OF_VAR so the HTTP server won't keep calling this 
		// application callback function
        if ( (BYTE)++ref == VarStringLen )
            return HTTP_END_OF_VAR;

        return ref;
    	
    case VAR_DHCP_TRUE:
    case VAR_DHCP_FALSE:
    	// Check if ref == 0 meaning that the first character of this 
    	// variable needs to be returned
        if ( ref == HTTP_START_OF_VAR )
        {
	        if((var == VAR_DHCP_TRUE) ^ AppConfig.Flags.bIsDHCPEnabled)
	        	return HTTP_END_OF_VAR;

            VarStringLen = 7;
			memcpypgm2ram(VarString, (rom void *)"checked", 7);
        }

		*val = VarString[(BYTE)ref];
		
        if ( (BYTE)++ref == VarStringLen )
            return HTTP_END_OF_VAR;

        return ref;
#endif

#if defined(STACK_USE_LED_BOARDS)
	case VAR_LEDVALUES0:
	case VAR_LEDVALUES1:
	case VAR_LEDVALUES2:
	case VAR_LEDVALUES3:
	case VAR_LEDVALUES00:
	case VAR_LEDVALUES01:
	case VAR_LEDVALUES02:
	case VAR_LEDVALUES03:
	case VAR_LEDVALUES10:
	case VAR_LEDVALUES11:
	case VAR_LEDVALUES12:
	case VAR_LEDVALUES13:
	case VAR_LEDVALUES20:
	case VAR_LEDVALUES21:
	case VAR_LEDVALUES22:
	case VAR_LEDVALUES23:
	case VAR_LEDVALUES30:
	case VAR_LEDVALUES31:
	case VAR_LEDVALUES32:
	case VAR_LEDVALUES33:
        if(ref == HTTP_START_OF_VAR)
		{
			switch(var)
    		{
				case VAR_LEDVALUES0:	xMin = 0; xMax = 2; yMin = 0; yMax = 2; break;
				case VAR_LEDVALUES1:	xMin = 2; xMax = 4; yMin = 0; yMax = 2; break;
				case VAR_LEDVALUES2:	xMin = 0; xMax = 2; yMin = 2; yMax = 4; break;
				case VAR_LEDVALUES3:	xMin = 2; xMax = 4; yMin = 2; yMax = 4; break;
				case VAR_LEDVALUES00:	xMin = 0; xMax = 1; yMin = 0; yMax = 1; break;
				case VAR_LEDVALUES01:	xMin = 1; xMax = 2; yMin = 0; yMax = 1; break;
				case VAR_LEDVALUES02:	xMin = 0; xMax = 1; yMin = 1; yMax = 2; break;
				case VAR_LEDVALUES03:	xMin = 1; xMax = 2; yMin = 1; yMax = 2; break;
				case VAR_LEDVALUES10:	xMin = 2; xMax = 3; yMin = 0; yMax = 1; break;
				case VAR_LEDVALUES11:	xMin = 3; xMax = 4; yMin = 0; yMax = 1; break;
				case VAR_LEDVALUES12:	xMin = 2; xMax = 3; yMin = 1; yMax = 2; break;
				case VAR_LEDVALUES13:	xMin = 3; xMax = 4; yMin = 1; yMax = 2; break;
				case VAR_LEDVALUES20:	xMin = 0; xMax = 1; yMin = 2; yMax = 3; break;
				case VAR_LEDVALUES21:	xMin = 1; xMax = 2; yMin = 2; yMax = 3; break;
				case VAR_LEDVALUES22:	xMin = 0; xMax = 1; yMin = 3; yMax = 4; break;
				case VAR_LEDVALUES23:	xMin = 1; xMax = 2; yMin = 3; yMax = 4; break;
				case VAR_LEDVALUES30:	xMin = 2; xMax = 3; yMin = 2; yMax = 3; break;
				case VAR_LEDVALUES31:	xMin = 3; xMax = 4; yMin = 2; yMax = 3; break;
				case VAR_LEDVALUES32:	xMin = 2; xMax = 3; yMin = 3; yMax = 4; break;
				case VAR_LEDVALUES33:	xMin = 3; xMax = 4; yMin = 3; yMax = 4; break;
			}
			VarString[0] = '\0';
			for (y=yMin; y<yMax; y++)
			{
				for (x=xMin; x<xMax; x++)
				{
					for (c = 0; c<3; c++)
					{
						//uitoa(ledValue[x][y][c], colorDigit);
						nibble = ledValue[x][y][c] >> 4;
						colorDigit[0] = ((nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10));
						nibble = ledValue[x][y][c] & 0x0F;
						colorDigit[1] = ((nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10));
						colorDigit[2] = '\0';
						strcat(VarString, colorDigit);
						//if (c != 2)
						//	strcatpgm2ram(VarString, (rom char *)",\0");
					}
					//if ((x != 3) & 
					//	strcatpgm2ram(VarString, (rom char *)" \0");
				}
				//strcatpgm2ram(VarString, (rom char *)"<BR>\0");
			}
		}
        *val = VarString[(BYTE)ref];
        if(VarString[(BYTE)ref] == '\0')
            return HTTP_END_OF_VAR;
		else if(VarString[(BYTE)++ref] == '\0' )
            return HTTP_END_OF_VAR;
        return ref;
#endif
    }

    return HTTP_END_OF_VAR;
}
#endif


#if defined(STACK_USE_FTP_SERVER) && defined(MPFS_USE_EEPROM) && defined(STACK_USE_MPFS)
ROM char FTP_USER_NAME[]    = "ftp";
ROM char FTP_USER_PASS[]    = "olimex";
#undef FTP_USER_NAME_LEN
#define FTP_USER_NAME_LEN   (sizeof(FTP_USER_NAME)-1)
#define FTP_USER_PASS_LEN   (sizeof(FTP_USER_PASS)-1)

BOOL FTPVerify(BYTE *login, BYTE *password)
{
    if ( !memcmppgm2ram(login, (ROM void*)FTP_USER_NAME, FTP_USER_NAME_LEN) )
    {
        if ( !memcmppgm2ram(password, (ROM void*)FTP_USER_PASS, FTP_USER_PASS_LEN) )
            return TRUE;
    }
    return FALSE;
}
#endif




/*********************************************************************
 * Function:        void InitializeBoard(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Initialize board specific hardware.
 *
 * Note:            None
 ********************************************************************/
static void InitializeBoard(void)
{
	// LEDs
	LED0_TRIS = 0;
	LED0_IO = 0;

#ifdef __C30__
	#if defined(__dsPIC33F__) || defined(__PIC24H__)
	// Crank up the core frequency
	PLLFBD = 38;				// Multiply by 40 for 160MHz VCO output (8MHz XT oscillator)
	CLKDIV = 0x0000;			// FRC: divide by 2, PLLPOST: divide by 2, PLLPRE: divide by 2

	// Port I/O
	AD1PCFGHbits.PCFG23 = 1;	// Make RA7 (BUTTON1) a digital input
	#endif

	// ADC
	AD1CON1 = 0x84E4;			// Turn on, auto sample start, auto-convert, 12 bit mode (on parts with a 12bit A/D)
	AD1CON2 = 0x0404;			// AVdd, AVss, int every 2 conversions, MUXA only, scan
	AD1CON3 = 0x1003;			// 16 Tad auto-sample, Tad = 3*Tcy
    AD1CHS = 0;					// Input to AN0 (potentiometer)
	AD1PCFGbits.PCFG5 = 0;		// Disable digital input on AN5 (potentiometer)
	AD1PCFGbits.PCFG4 = 0;		// Disable digital input on AN4 (TC1047A temp sensor)
	AD1CSSL = 1<<5;				// Scan pot

//	// Enable ADC interrupt
//	IFS0bits.AD1IF = 0;
//	IEC0bits.AD1IE = 1;


	// UART
	UARTTX_TRIS = 0;
	UARTRX_TRIS = 1;
	UBRG = (INSTR_FREQ+8ul*BAUD_RATE)/16/BAUD_RATE-1;
	UMODE = 0x8000;		// UARTEN set
	USTA = 0x0400;		// UTXEN set
#else
	// Enable 4x/5x PLL on PIC18F87J10, PIC18F97J60, etc.
	// Ivan  - No program-configurable oscillator here
    //OSCTUNE = 0x40;

	// Set up analog features of PORTA

	// PICDEM.net 2 board has POT on AN2, Temp Sensor on AN3
	#if defined(PICDEMNET2) || defined(OLIMEX_MAXI)
		ADCON0 = 0x09;		// ADON, Channel 2
		ADCON1 = 0x0B;		// Vdd/Vss is +/-REF, AN0, AN1, AN2, AN3 are analog
	    TRISA = 0x2F;
	#elif defined(__18F452)
		ADCON0 = 0x81;		// ADON, Channel 0, Fosc/32
		ADCON1 = 0x8E;		// Right justified, Fosc/32, AN0 only anlog, VREF+/VREF- are VDD/VSS
	    TRISA = 0x23;	
	#elif defined(PICDEMZ)
		ADCON0 = 0x81;		// ADON, Channel 0, Fosc/32
		ADCON1 = 0x0F;		// Vdd/Vss is +/-REF, AN0, AN1, AN2, AN3 are all digital
	#else
		ADCON0 = 0x01;		// ADON, Channel 0
		ADCON1 = 0x0E;		// Vdd/Vss is +/-REF, AN0 is analog
	    TRISA = 0x23;
	#endif
	//Ivan not here...
	//	ADCON2 = 0xBE;			// Right justify, 20TAD ACQ time, Fosc/64 (~21.0kHz)


    // Enable internal PORTB pull-ups
    INTCON2bits.RBPU = 0;

	// Configure USART
    TXSTA = 0x20;
#if defined(FS_USB)
    RCSTA = 0x80;		// PICDEM FS USB demo board has UART RX pin multipled with SPI, we must not enable the UART RX functionality
#else
    RCSTA = 0x90;
#endif

	// See if we can use the high baud rate setting
#if ((INSTR_FREQ+2*BAUD_RATE)/BAUD_RATE/4 - 1) <= 255
	SPBRG = (INSTR_FREQ+2*BAUD_RATE)/BAUD_RATE/4 - 1;
	TXSTAbits.BRGH = 1;
#else	// Use the low baud rate setting
	SPBRG = (INSTR_FREQ+8*BAUD_RATE)/BAUD_RATE/16 - 1;
#endif


	// Enable Interrupts
	RCONbits.IPEN = 1;		// Enable interrupt priorities
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;

    // Do a calibration A/D conversion
	#if defined(__18F87J10) || defined(__18F86J15) || defined(__18F86J10) || defined(__18F85J15) || defined(__18F85J10) || defined(__18F67J10) || defined(__18F66J15) || defined(__18F66J10) || defined(__18F65J15) || defined(__18F65J10) || defined(__18F97J60) || defined(__18F96J65) || defined(__18F96J60) || defined(__18F87J60) || defined(__18F86J65) || defined(__18F86J60) || defined(__18F67J60) || defined(__18F66J65) || defined(__18F66J60) || \
	defined(_18F87J10)  || defined(_18F86J15)  || defined(_18F86J10)  || defined(_18F85J15)  || defined(_18F85J10)  || defined(_18F67J10)  || defined(_18F66J15)  || defined(_18F66J10)  || defined(_18F65J15)  || defined(_18F65J10)  || defined(_18F97J60)  || defined(_18F96J65)  || defined(_18F96J60)  || defined(_18F87J60)  || defined(_18F86J65)  || defined(_18F86J60)  || defined(_18F67J60)  || defined(_18F66J65)  || defined(_18F66J60)
		ADCON0bits.ADCAL = 1;
	    ADCON0bits.GO = 1;
		while(ADCON0bits.GO);
		ADCON0bits.ADCAL = 0;
	#endif

//	// Enable ADC interrupt
//	PIR1bits.ADIF = 0;
//	PIE1bits.ADIE = 1;

#endif

#if defined(DSPICDEM11)
	// Deselect the LCD controller (PIC18F252 onboard) to ensure there is no SPI2 contention
	LCDCTRL_CS_TRIS = 0;
	LCDCTRL_CS_IO = 1;

	// Hold the codec in reset to ensure there is no SPI2 contention
	CODEC_RST_TRIS = 0;
	CODEC_RST_IO = 0;
#elif defined(DSPICDEMNET1) || defined(DSPICDEMNET2)
	// Hold Si3021 in reset to keep the speaker as quiet as possible
	TRISGbits.TRISG8 = 0;
	LATGbits.LATG8 = 0;

	// Ensure that the SRAM does not interfere with RTL8019AS communications
	SRAM_CE_ADPCFG = 1;
	SRAM_OE_ADPCFG = 1;
	SRAM_CE_IO = 1;
	SRAM_OE_IO = 1;
	SRAM_CE_TRIS = 0;
	SRAM_OE_TRIS = 0;
#elif defined(PIC18F67J60_TEST_BOARD)
	// Make PIC SPI1 clock (SCK) not cause contention with U2:D level shifter. U2:D drives RC3 as well, by default on board revision 1.
	TRISEbits.TRISE1 = 0;
	LATEbits.LATE1 = 1;
#endif

}

/*********************************************************************
 * Function:        void InitAppConfig(void)
 *
 * PreCondition:    MPFSInit() is already called.
 *
 * Input:           None
 *
 * Output:          Write/Read non-volatile config variables.
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
static void InitAppConfig(void)
{
#if defined(MPFS_USE_EEPROM) && defined(STACK_USE_MPFS)
    BYTE c;
    BYTE *p;
#endif

	AppConfig.Flags.bIsDHCPEnabled = TRUE;
	AppConfig.Flags.bInConfigMode = TRUE;
	AppConfig.MyMACAddr.v[0] = MY_DEFAULT_MAC_BYTE1;
	AppConfig.MyMACAddr.v[1] = MY_DEFAULT_MAC_BYTE2;
	AppConfig.MyMACAddr.v[2] = MY_DEFAULT_MAC_BYTE3;
	AppConfig.MyMACAddr.v[3] = MY_DEFAULT_MAC_BYTE4;
	AppConfig.MyMACAddr.v[4] = MY_DEFAULT_MAC_BYTE5;
	AppConfig.MyMACAddr.v[5] = MY_DEFAULT_MAC_BYTE6;
	AppConfig.MyIPAddr.Val = MY_DEFAULT_IP_ADDR_BYTE1 | MY_DEFAULT_IP_ADDR_BYTE2<<8ul | MY_DEFAULT_IP_ADDR_BYTE3<<16ul | MY_DEFAULT_IP_ADDR_BYTE4<<24ul;
	AppConfig.DefaultIPAddr.Val = AppConfig.MyIPAddr.Val;
	AppConfig.MyMask.Val = MY_DEFAULT_MASK_BYTE1 | MY_DEFAULT_MASK_BYTE2<<8ul | MY_DEFAULT_MASK_BYTE3<<16ul | MY_DEFAULT_MASK_BYTE4<<24ul;
	AppConfig.DefaultMask.Val = AppConfig.MyMask.Val;
	AppConfig.MyGateway.Val = MY_DEFAULT_GATE_BYTE1 | MY_DEFAULT_GATE_BYTE2<<8ul | MY_DEFAULT_GATE_BYTE3<<16ul | MY_DEFAULT_GATE_BYTE4<<24ul;
	AppConfig.PrimaryDNSServer.Val = MY_DEFAULT_PRIMARY_DNS_BYTE1 | MY_DEFAULT_PRIMARY_DNS_BYTE2<<8ul  | MY_DEFAULT_PRIMARY_DNS_BYTE3<<16ul  | MY_DEFAULT_PRIMARY_DNS_BYTE4<<24ul;
	AppConfig.SecondaryDNSServer.Val = MY_DEFAULT_SECONDARY_DNS_BYTE1 | MY_DEFAULT_SECONDARY_DNS_BYTE2<<8ul  | MY_DEFAULT_SECONDARY_DNS_BYTE3<<16ul  | MY_DEFAULT_SECONDARY_DNS_BYTE4<<24ul;

	// Load the default NetBIOS Host Name
	memcpypgm2ram(AppConfig.NetBIOSName, (ROM void*)MY_DEFAULT_HOST_NAME, 16);
	FormatNetBIOSName(AppConfig.NetBIOSName);

#if defined(MPFS_USE_EEPROM) && defined(STACK_USE_MPFS)
    p = (BYTE*)&AppConfig;


    XEEBeginRead(0x0000);
    c = XEERead();
    XEEEndRead();

    // When a record is saved, first byte is written as 0x57 to indicate
    // that a valid record was saved.  Note that older stack versions 
	// used 0x55.  This change has been made to so old EEPROM contents 
	// will get overwritten.  The AppConfig() structure has been changed,
	// resulting in parameter misalignment if still using old EEPROM 
	// contents.
    if(c == 0x60u)
    {
        XEEBeginRead(0x0001);
        for ( c = 0; c < sizeof(AppConfig); c++ )
            *p++ = XEERead();
        XEEEndRead();
    }
    else
        SaveAppConfig();
#endif
}

#if defined(MPFS_USE_EEPROM) && defined (STACK_USE_MPFS)
static void SaveAppConfig(void)
{
    BYTE c;
    BYTE *p;

    p = (BYTE*)&AppConfig;
    XEEBeginWrite(0x0000);
    XEEWrite(0x60);
    for ( c = 0; c < sizeof(AppConfig); c++ )
    {
        XEEWrite(*p++);
    }

    XEEEndWrite();
}
#endif



#if defined(STACK_USE_UART)
#define MAX_USER_RESPONSE_LEN   (20u)
static void SetConfig(void)
{
    BYTE response[MAX_USER_RESPONSE_LEN];
    IP_ADDR tempIPValue;
    IP_ADDR *destIPValue;
	WORD_VAL wvTemp;
    BOOL bQuit = FALSE;
    

	while(!bQuit)
	{
		// Display the menu
	    putrsUART("\r\n\r\n\rMicrochip TCP/IP Config Application ("VERSION", " __DATE__ ")\r\n\r\n");
	    putrsUART("\t1: Change serial number:\t\t");
		wvTemp.v[1] = AppConfig.MyMACAddr.v[4];
		wvTemp.v[0] = AppConfig.MyMACAddr.v[5];
		uitoa(wvTemp.Val, response);
		putsUART(response);
		putrsUART("\r\n\t2: Change host name:\t\t\t");
		putsUART(AppConfig.NetBIOSName);
	    putrsUART("\r\n\t3: Change static IP address:\t\t");
	    DisplayIPValue(AppConfig.MyIPAddr);
	    putrsUART("\r\n\t4: Change static gateway address:\t");
	    DisplayIPValue(AppConfig.MyGateway);
	    putrsUART("\r\n\t5: Change static subnet mask:\t\t");
	    DisplayIPValue(AppConfig.MyMask);
		putrsUART("\r\n\t6: Change static primary DNS server:\t");
	    DisplayIPValue(AppConfig.PrimaryDNSServer);
		putrsUART("\r\n\t7: Change static secondary DNS server:\t");
	    DisplayIPValue(AppConfig.SecondaryDNSServer);
		#if defined(STACK_USE_DHCP_CLIENT) || defined(STACK_USE_DHCP_SERVER)
			putrsUART("\r\n\t8: ");
			putrsUART((ROM BYTE*)(AppConfig.Flags.bIsDHCPEnabled ? "Dis" : "En"));
			putrsUART("able DHCP & IP Gleaning:\t\tDHCP is currently ");
			putrsUART((ROM BYTE*)(AppConfig.Flags.bIsDHCPEnabled ? "enabled" : "disabled"));
		#endif
	    putrsUART("\r\n\t9: Download MPFS image.");
	    putrsUART("\r\n\t0: Save & Quit.");
	    putrsUART("\r\nEnter a menu choice: ");
	
	
		// Wait for the user to press a key
	    while(!DataRdyUART());
	
		putrsUART((ROM char*)"\r\n");
	
		// Execute the user selection
	    switch(ReadUART())
	    {
			#if defined(__DEBUG)
			    // Ivan - test code here
			    case 'T':
				    putrsUART("Put extension header and press any key");
				    while(!DataRdyUART());
				    putrsUART("\r\n");
				    switch(TestExt())
				    {
					   	case TEST_OK:
					   		putrsUART("Extension OK");
					   		LED0_TRIS = 0;
					   		LED0_IO = 1;
					   		
					   		//InitRTC();
					   		// test OK, Lit the LED
					   		while (1);
					   		break;
					   	case TEST_GND:
					   		putrsUART("Pin to GND!!!!!!!!!");
					   		break;
					   	case TEST_VCC:
					   		putrsUART("Pin to VCC!!!!!!!!!");
					   		break;
					   	case TEST_ZERO_A:
					   		putrsUART("Short on PORT A!!!!!!!");
					   		break;
					   	case TEST_ZERO_C:
					   		putrsUART("Short on PORT C!!!!!!!");
					   		break;
					   	case TEST_ZERO_D:
					   		putrsUART("Short on PORT D!!!!!!!");
					   		break;
					   	case TEST_ZERO_E:
					   		putrsUART("Short on PORT E!!!!!!!");
					   		break;
					   	case TEST_RTC:
					   		putrsUART("RTC fault");
					   		break;			    	
						}
						// test fault - do nothing
						LED0_TRIS = 0;
						LED0_IO = 0;
						while(1);
					break;
				#endif
							    	
		    case '1':
				putrsUART("New setting: ");
				if(ReadStringUART(response, sizeof(response)))
				{
					wvTemp.Val = atoi((char*)response);
			        AppConfig.MyMACAddr.v[4] = wvTemp.v[1];
		    	    AppConfig.MyMACAddr.v[5] = wvTemp.v[0];
				}
		        break;
		
			case '2':
				putrsUART("New setting: ");
		        ReadStringUART(response, sizeof(response) > sizeof(AppConfig.NetBIOSName) ? sizeof(AppConfig.NetBIOSName) : sizeof(response));
				if(response[0] != '\0')
				{
					memcpy(AppConfig.NetBIOSName, (void*)response, sizeof(AppConfig.NetBIOSName));
			        FormatNetBIOSName(AppConfig.NetBIOSName);
				}
				break;
		
		    case '3':
		        destIPValue = &AppConfig.MyIPAddr;
		        goto ReadIPConfig;
		
		    case '4':
		        destIPValue = &AppConfig.MyGateway;
		        goto ReadIPConfig;
		
		    case '5':
		        destIPValue = &AppConfig.MyMask;
		        goto ReadIPConfig;
		
		    case '6':
		        destIPValue = &AppConfig.PrimaryDNSServer;
		        goto ReadIPConfig;
	
			case '7':
		        destIPValue = &AppConfig.SecondaryDNSServer;
		        goto ReadIPConfig;
		
ReadIPConfig:
				putrsUART("New setting: ");
		        ReadStringUART(response, sizeof(response));
		
		        if(StringToIPAddress(response, &tempIPValue))
		            destIPValue->Val = tempIPValue.Val;
				else
		            putrsUART("Invalid input.\r\n");

		        break;
		
		    case '8':
				#if defined(STACK_USE_DHCP_CLIENT) || defined(STACK_USE_DHCP_SERVER)
		        	AppConfig.Flags.bIsDHCPEnabled = !AppConfig.Flags.bIsDHCPEnabled;
				#else
					//putrsUART("DCHP client and server are disabled in firmware.\r\n");
				#endif
		        break;
		
		    case '9':
				#if defined(MPFS_USE_EEPROM) && defined(STACK_USE_MPFS)
		        	DownloadMPFS();
				#endif
		        break;
		
		    case '0':
			    bQuit = TRUE;
				#if defined(MPFS_USE_EEPROM) && defined(STACK_USE_MPFS)
		        	SaveAppConfig();
					putrsUART("Settings saved.\r\n");
				#else
					putrsUART("External MPFS not enabled -- settings will be lost at reset.\r\n");
				#endif
		        break;
		}
	}
}
#endif //#if defined(STACK_USE_UART)


#if defined(MPFS_USE_EEPROM) && defined(STACK_USE_MPFS) && defined(STACK_USE_UART)
/*********************************************************************
 * Function:        BOOL DownloadMPFS(void)
 *
 * PreCondition:    MPFSInit() is already called.
 *
 * Input:           None
 *
 * Output:          TRUE if successful
 *                  FALSE otherwise
 *
 * Side Effects:    This function uses 128 bytes of Bank 4 using
 *                  indirect pointer.  This requires that no part of
 *                  code is using this block during or before calling
 *                  this function.  Once this function is done,
 *                  that block of memory is available for general use.
 *
 * Overview:        This function implements XMODEM protocol to
 *                  be able to receive a binary file from PC
 *                  applications such as HyperTerminal.
 *
 * Note:            In current version, this function does not
 *                  implement user interface to set IP address and
 *                  other informations.  User should create their
 *                  own interface to allow user to modify IP
 *                  information.
 *                  Also, this version implements simple user
 *                  action to start file transfer.  User may
 *                  evaulate its own requirement and implement
 *                  appropriate start action.
 *
 ********************************************************************/
#define XMODEM_SOH      0x01u
#define XMODEM_EOT      0x04u
#define XMODEM_ACK      0x06u
#define XMODEM_NAK      0x15u
#define XMODEM_CAN      0x18u
#define XMODEM_BLOCK_LEN 128u

static void DownloadMPFS(void) {
    enum SM_MPFS {
        SM_MPFS_WAITING,
        SM_MPFS_SOH,
        SM_MPFS_BLOCK,
        SM_MPFS_BLOCK_CMP,
        SM_MPFS_DATA,
        SM_MPFS_DATA_CS,
        SM_MPFS_EOT
    } state;


    BYTE c;
    BYTE checksum;
    MPFS handle;
    BOOL keepLooping;
    BYTE blockLen;
    BYTE tempData[XMODEM_BLOCK_LEN];
    TICK lastTick;
    TICK currentTick;


    state = SM_MPFS_WAITING;


    // Notify the host that we are ready to receive data
    lastTick = TickGet();
    do {
        // After about half a second, blink the LED and send a NAK to the host
        currentTick = TickGet();
        if (TickGetDiff(currentTick, lastTick) >= (TICK_SECOND / 2)) {
            lastTick = TickGet();
            LED0_IO ^= 1;
            while (BusyUART());
            WriteUART(XMODEM_NAK);
        }
    } while (!DataRdyUART());


    // Initialize file system and point to the first byte of EEPROM storage. Do this
    // after we receive a character from the host to give the board a chance to
    // be reset without losing the current EEPROM image.
    handle = MPFSFormat();


    keepLooping = TRUE;
    while (keepLooping) {


        if (DataRdyUART()) {
            // Toggle LED as data is received from host.
            LED0_IO ^= 1;


            // Get the next character from the host
            c = ReadUART();
            if (c == XMODEM_SOH && state == SM_MPFS_WAITING) {
              // Beginning of transfer
              state = SM_MPFS_SOH;
            } else if (c == XMODEM_EOT && state == SM_MPFS_WAITING) {
              // End of transfer
              state = SM_MPFS_EOT;
            }
        } else {
            continue;
        }


        switch (state) {
            // Start of data header
            case SM_MPFS_SOH:
                blockLen = 0;
                checksum = 0;
                state = SM_MPFS_BLOCK;
                break;


            // Routine doesn't use the data block at the present time
            case SM_MPFS_BLOCK:
                state = SM_MPFS_BLOCK_CMP;
                break;


            // Routine doesn't use the complement of the data block either
            case SM_MPFS_BLOCK_CMP:
                state = SM_MPFS_DATA;
                break;


            // Receive a block of data from the host and generate a running checksum
            case SM_MPFS_DATA:
                checksum += c;
                tempData[blockLen++] = c;
                if (blockLen == XMODEM_BLOCK_LEN) {
                  state = SM_MPFS_DATA_CS;
                }
                break;


            // A block of data has been received, verify checksum, save
            // the block to EEPROM on success, and ACK/NAK the host
            case SM_MPFS_DATA_CS:
                while(BusyUART());


                if (c == checksum) {
                    MPFSPutBegin(handle);
                    for (c = 0; c < XMODEM_BLOCK_LEN; c++)
                        MPFSPut(tempData[c]);
                    handle = MPFSPutEnd();


                   // Tell host we got the block and to send the next
                   WriteUART(XMODEM_ACK);
                } else {
                   // Tell the host to resend block
                   WriteUART(XMODEM_NAK);
                }


                state = SM_MPFS_WAITING;
                break;


            case SM_MPFS_EOT:
                // LED off
                LED0_IO = 0;
                MPFSClose();
                while(BusyUART());
                WriteUART(XMODEM_ACK);
                keepLooping = FALSE;
            break;


            default:
                // Invalid state
                while(BusyUART());
                WriteUART(XMODEM_NAK);
                break;
        }
    }
}
#endif	// #if defined(MPFS_USE_EEPROM) && defined(STACK_USE_MPFS)

// NOTE: Name[] must be at least 16 characters long.
// It should be exactly 16 characters, as defined by NetBIOS spec.
static void FormatNetBIOSName(BYTE Name[])
{
	BYTE i;

	Name[15] = '\0';
	strupr((char*)Name);
	i = 0;
	while(i < 15u)
	{
		if(Name[i] == '\0')
		{
			while(i < 15u)
			{
				Name[i++] = ' ';
			}
			break;
		}
		i++;
	}
}



#if defined(STACK_USE_SNMP_SERVER)

/*
 * Trap information.
 * This table maintains list of intereseted receivers
 * who should receive notifications when some interesting
 * event occurs.
 */
#define TRAP_TABLE_SIZE         (2)
#define MAX_COMMUNITY_LEN       (8)
typedef struct _TRAP_INFO
{
    BYTE Size;
    struct
    {
        BYTE communityLen;
        char community[MAX_COMMUNITY_LEN];
        IP_ADDR IPAddress;
        struct
        {
            unsigned int bEnabled : 1;
        } Flags;
    } table[TRAP_TABLE_SIZE];
} TRAP_INFO;

/*
 * Initialize trap table with no entries.
 */
TRAP_INFO trapInfo = { TRAP_TABLE_SIZE };


BOOL SNMPValidate(SNMP_ACTION SNMPAction, char* community)
{
    return TRUE;
}



// Only dynamic variables with ASCII_STRING or OCTET_STRING data type
// needs to be handled.
BOOL SNMPIsValidSetLen(SNMP_ID var, BYTE len)
{
    switch(var)
    {
    case TRAP_COMMUNITY:
        if ( len < MAX_COMMUNITY_LEN+1 )
            return TRUE;
        break;

#if defined(USE_LCD)
    case LCD_DISPLAY:
        if ( len < sizeof(LCDText)+1 )
            return TRUE;
        break;
#endif
    }
    return FALSE;
}


// Only dynamic read-write variables needs to be handled.
BOOL SNMPSetVar(SNMP_ID var, SNMP_INDEX index, BYTE ref, SNMP_VAL val)
{
    switch(var)
    {
    case LED_D5:
        LED1_IO = val.byte;
        return TRUE;

    case LED_D6:
        LED2_IO = val.byte;
        return TRUE;

    case TRAP_RECEIVER_IP:
        // Make sure that index is within our range.
        if ( index < trapInfo.Size )
        {
            // This is just an update to an existing entry.
            trapInfo.table[index].IPAddress.Val = val.dword;
            return TRUE;
        }
        else if ( index < TRAP_TABLE_SIZE )
        {
            // This is an addition to table.
            trapInfo.table[index].IPAddress.Val = val.dword;
            trapInfo.table[index].communityLen = 0;
            trapInfo.Size++;
            return TRUE;
        }
        break;

    case TRAP_RECEIVER_ENABLED:
        // Make sure that index is within our range.
        if ( index < trapInfo.Size )
        {
            // Value of '1' means Enabled".
            if ( val.byte == 1 )
                trapInfo.table[index].Flags.bEnabled = 1;
            // Value of '0' means "Disabled.
            else if ( val.byte == 0 )
                trapInfo.table[index].Flags.bEnabled = 0;
            else
                // This is unknown value.
                return FALSE;
            return TRUE;
        }
        // Given index is more than our current table size.
        // If it is within our range, treat it as an addition to table.
        else if ( index < TRAP_TABLE_SIZE )
        {
            // Treat this as an addition to table.
            trapInfo.Size++;
            trapInfo.table[index].communityLen = 0;
        }

        break;

    case TRAP_COMMUNITY:
        // Since this is a ASCII_STRING data type, SNMP will call with
        // SNMP_END_OF_VAR to indicate no more bytes.
        // Use this information to determine if we just added new row
        // or updated an existing one.
        if ( ref ==  SNMP_END_OF_VAR )
        {
            // Index equal to table size means that we have new row.
            if ( index == trapInfo.Size )
                trapInfo.Size++;

            // Length of string is one more than index.
            trapInfo.table[index].communityLen++;

            return TRUE;
        }

        // Make sure that index is within our range.
        if ( index < trapInfo.Size )
        {
            // Copy given value into local buffer.
            trapInfo.table[index].community[ref] = val.byte;
            // Keep track of length too.
            // This may not be NULL terminate string.
            trapInfo.table[index].communityLen = (BYTE)ref;
            return TRUE;
        }
        break;

#if defined(USE_LCD)
    case LCD_DISPLAY:
        // Copy all bytes until all bytes are transferred
        if ( ref != SNMP_END_OF_VAR )
        {
            LCDText[ref] = val.byte;
            LCDText[ref+1] = 0;
        }
        else
        {
			LCDUpdate();
        }

        return TRUE;
#endif

    }

    return FALSE;
}

// Only sequence index needs to be handled in this function.
BOOL SNMPGetNextIndex(SNMP_ID var, SNMP_INDEX *index)
{
    SNMP_INDEX tempIndex;

    tempIndex = *index;

    switch(var)
    {
    case TRAP_RECEIVER_ID:
        // There is no next possible index if table itself is empty.
        if ( trapInfo.Size == 0 )
            return FALSE;

        // INDEX_INVALID means start with first index.
        if ( tempIndex == SNMP_INDEX_INVALID )
        {
            *index = 0;
            return TRUE;
        }
        else if ( tempIndex < (trapInfo.Size-1) )
        {
            *index = tempIndex+1;
            return TRUE;
        }
        break;
    }
    return FALSE;
}


BOOL SNMPGetVar(SNMP_ID var, SNMP_INDEX index, BYTE *ref, SNMP_VAL* val)
{
    BYTE myRef;

    myRef = *ref;

    switch(var)
    {
    case SYS_UP_TIME:
        val->dword = TickGet();
        return TRUE;

    case LED_D5:
        val->byte = LED1_IO;
        return TRUE;

    case LED_D6:
        val->byte = LED2_IO;
        return TRUE;

    case PUSH_BUTTON:
        // There is only one button - meaning only index of 0 is allowed.
        val->byte = BUTTON0_IO;
        return TRUE;

    case ANALOG_POT0:
        val->word = atoi((char*)AN0String);
        return TRUE;

    case TRAP_RECEIVER_ID:
        if ( index < trapInfo.Size )
        {
            val->byte = index;
            return TRUE;
        }
        break;

    case TRAP_RECEIVER_ENABLED:
        if ( index < trapInfo.Size )
        {
            val->byte = trapInfo.table[index].Flags.bEnabled;
            return TRUE;
        }
        break;

    case TRAP_RECEIVER_IP:
        if ( index < trapInfo.Size )
        {
            val->dword = trapInfo.table[index].IPAddress.Val;
            return TRUE;
        }
        break;

    case TRAP_COMMUNITY:
        if ( index < trapInfo.Size )
        {
            if ( trapInfo.table[index].communityLen == 0 )
                *ref = SNMP_END_OF_VAR;
            else
            {
                val->byte = trapInfo.table[index].community[myRef];

                myRef++;

                if ( myRef == trapInfo.table[index].communityLen )
                    *ref = SNMP_END_OF_VAR;
                else
                    *ref = myRef;
            }
            return TRUE;
        }
        break;

#if defined(USE_LCD)
    case LCD_DISPLAY:
        if ( LCDText[0] == 0 )
            myRef = SNMP_END_OF_VAR;
        else
        {
            val->byte = LCDText[myRef++];
            if ( LCDText[myRef] == 0 )
                myRef = SNMP_END_OF_VAR;
        }

        *ref = myRef;
        return TRUE;
#endif
    }

    return FALSE;
}
#endif	//#if defined(STACK_USE_SNMP_SERVER)
