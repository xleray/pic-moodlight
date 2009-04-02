/*********************************************************************
 *
 *  httpTwitterPost (based on GenericTCPClient.c)
 *
 *********************************************************************
 * FileName:        httpTwitterPost.c
 * Dependencies:    TCP.h, DNS.h
 * Processor:       PIC18, PIC24F, PIC24H, dsPIC30, dsPIC33F
 * Complier:        Microchip C18 v3.03 or higher
 * 					Microchip C30 v2.02 or higher
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT 
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
 ********************************************************************/
#define __HTTPTWITTERPOST_C

#include "TCPIP Stack/TCPIP.h"

#if defined(STACK_USE_HTTP_TWITTER_POST)

	BYTE alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	extern BYTE AN0String[8];
	extern BYTE TEMPString[8];
	void sendbase64encodedData(TCP_SOCKET MySocket, BYTE *data)
	{
	    unsigned int char_count, i;
	    DWORD bits;
	
	    char_count = 0;
	    bits = 0;
		i = 0;
	    while( data[i] != '\0') {
			bits += data[i];
			char_count++;
			if (char_count == 3) {
				while(BusyUSART()); putcUART(alphabet[(bits >> 18) & 0x3f]); TCPPut(MySocket, alphabet[(bits >> 18) & 0x3f]);
			    while(BusyUSART()); putcUART(alphabet[(bits >> 12) & 0x3f]); TCPPut(MySocket, alphabet[(bits >> 12) & 0x3f]);
			    while(BusyUSART()); putcUART(alphabet[(bits >> 6) & 0x3f]); TCPPut(MySocket, alphabet[(bits >> 6) & 0x3f]);
			    while(BusyUSART()); putcUART(alphabet[bits & 0x3f]);  TCPPut(MySocket, alphabet[bits & 0x3f]);
			    bits = 0;
			    char_count = 0;
			} else {
			    bits <<= 8;
			}
			i++;
	    }
	    if (char_count != 0) {
			bits <<= 16 - (8 * char_count);
			while(BusyUSART()); putcUART(alphabet[(bits >> 18) & 0x3f]); TCPPut(MySocket, alphabet[(bits >> 18) & 0x3f]);
			while(BusyUSART()); putcUART(alphabet[(bits >> 12) & 0x3f]); TCPPut(MySocket, alphabet[(bits >> 12) & 0x3f]);
			if (char_count == 1) {
			    while(BusyUSART()); putcUART('='); TCPPut(MySocket, '=');
			    while(BusyUSART()); putcUART('='); TCPPut(MySocket, '=');
			} else {
			    while(BusyUSART()); putcUART(alphabet[(bits >> 6) & 0x3f]); TCPPut(MySocket, alphabet[(bits >> 6) & 0x3f]);
			    while(BusyUSART()); putcUART('='); TCPPut(MySocket, '=');
			}
		}
	}
	
	WORD ServerPort = 80;
	BYTE ServerName[] = "www.twitter.com";
	BYTE usernamePass[] = "username:password";
	void httpTwitterPost(BOOL runNow)
	{
		BYTE 				i;
		BYTE				c;
		BYTE 				*StringPtr;
		static TICK			Timer;
		static TCP_SOCKET	MySocket = INVALID_SOCKET;
		static NODE_INFO	Server;
		static enum _httpTwitterPostState
		{
			SM_HOME = 0,
			SM_NAME_RESOLVE,
			SM_ARP_START_RESOLVE,
			SM_ARP_RESOLVE,
			SM_SOCKET_OBTAIN,
			SM_SOCKET_OBTAINED,
			SM_PROCESS_RESPONSE,
			SM_DISCONNECT,
			SM_DONE
		} httpTwitterPostState = SM_DONE;
	
		if (runNow) {
			httpTwitterPostState = SM_HOME;
		}
		switch(httpTwitterPostState)
		{
			case SM_HOME:
				LED0_IO = 1;
				// Obtain ownership of the DNS resolution module
				if(!DNSBeginUsage())
					break;
	
				// Obtain the IP address associated with the common ServerName
				DNSResolve(ServerName, DNS_TYPE_A);
				httpTwitterPostState++;
				break;
	
			case SM_NAME_RESOLVE:
				// Wait for the DNS server to return the requested IP address
				if(!DNSIsResolved(&Server.IPAddr))
					break;
	
				// Release the DNS module, we no longer need it
				if(!DNSEndUsage())
				{
					// An invalid IP address was returned from the DNS 
					// server.  Quit and fail permanantly if host is not valid.
					httpTwitterPostState = SM_DONE;
					break;
				}
	
				httpTwitterPostState++;
	
			case SM_ARP_START_RESOLVE:
				// Obtain the MAC address associated with the server's IP address (either direct MAC address on same subnet, or the MAC address of the Gateway machine)
				ARPResolve(&Server.IPAddr);
				Timer = TickGet();
				httpTwitterPostState++;
				break;
	
			case SM_ARP_RESOLVE:
				// Wait for the MAC address to finish being obtained
				if(!ARPIsResolved(&Server.IPAddr, &Server.MACAddr))
				{
					// Time out if too much time is spent in this state
					if(TickGet()-Timer > 1*TICK_SECOND)
					{
						// Retransmit ARP request
						httpTwitterPostState--;
					}
					break;
				}
				httpTwitterPostState++;
	
			case SM_SOCKET_OBTAIN:
				// Connect a socket to the remote TCP server
				MySocket = TCPConnect(&Server, ServerPort);
				
				// Abort operation if no TCP sockets are available
				// If this ever happens, incrementing MAX_TCP_SOCKETS in 
				// StackTsk.h may help (at the expense of more global memory 
				// resources).
				if(MySocket == INVALID_SOCKET)
					break;
	
				httpTwitterPostState++;
				Timer = TickGet();
				break;
	
			case SM_SOCKET_OBTAINED:
				// Wait for the remote server to accept our connection request
				if(!TCPIsConnected(MySocket))
				{
					// Time out if too much time is spent in this state
					if(TickGet()-Timer > 5*TICK_SECOND)
					{
						// Close the socket so it can be used by other modules
						TCPDisconnect(MySocket);
						MySocket = INVALID_SOCKET;
						httpTwitterPostState--;
					}
					break;
				}
	
				Timer = TickGet();
	
				// Make certain the socket can be written to
				if(!TCPIsPutReady(MySocket))
					break;
	
				putrsUART("\r\nSending Information:\r\n");
				// Place the application protocol data into the transmit buffer.  For this example, we are connected to an HTTP server, so we'll send an HTTP GET request.
				TCPPutROMString(MySocket, (ROM BYTE*)"POST /statuses/update.xml?status=");
								putrsUART((ROM BYTE*)"POST /statuses/update.xml?status=");
				/*TCPPutString(MySocket, TEMPString);
							  putsUART(TEMPString);*/
				c = 0;
				while( TEMPString[c] != '\0')
				{  // Transmit a byte
				    TCPPut(MySocket, TEMPString[c]);
					while(BusyUSART()); putcUART(TEMPString[c]);
					c++;
				};
				TCPPutROMString(MySocket, (ROM BYTE*)"C+temperature HTTP/1.0\r\nHost: ");
								putrsUART((ROM BYTE*)"C+temperature HTTP/1.0\r\nHost: ");
				TCPPutString(MySocket, ServerName);
							  putsUART(ServerName);
				TCPPutROMString(MySocket, (ROM BYTE*)"\r\nAuthorization: Basic ");
								putrsUART((ROM BYTE*)"\r\nAuthorization: Basic ");
				sendbase64encodedData(MySocket, usernamePass);
				TCPPutROMString(MySocket, (ROM BYTE*)"\r\nContent-Type: application/x-www-form-urlencoded");
								putrsUART((ROM BYTE*)"\r\nContent-Type: application/x-www-form-urlencoded");
				TCPPutROMString(MySocket, (ROM BYTE*)"\r\n\r\n");
								putrsUART((ROM BYTE*)"\r\n\r\n");
	
				// Send the packet
				//TCPFlush(MySocket);
				httpTwitterPostState++;
	
			case SM_PROCESS_RESPONSE:
				// Check to see if the remote node has disconnected from us or sent us any application data
				// If application data is available, write it to the UART
				if(!TCPIsConnected(MySocket))
				{
					httpTwitterPostState++;
				}
		
				if(!TCPIsGetReady(MySocket))
					break;
		
				// Obtain the server reply
				while(TCPGet(MySocket, &i))
				{
					while(BusyUART());
					WriteUART(i);
				}
		
				break;
		
			case SM_DISCONNECT:
				LED0_IO = 0;
				// Close the socket so it can be used by other modules
				// For this application, we wish to stay connected, but this state will still get entered if the remote server decides to disconnect
				TCPDisconnect(MySocket);
				MySocket = INVALID_SOCKET;
				httpTwitterPostState = SM_DONE;
				break;
		
			case SM_DONE:
				// Do nothing unless the user pushes BUTTON1 and wants to restart the whole connection/download process
				if(BUTTON0_IO == 0u)
					httpTwitterPostState = SM_HOME;
				break;
		}
	}

#endif	//#if defined(STACK_USE_HTTP_TWITTER_POST)
