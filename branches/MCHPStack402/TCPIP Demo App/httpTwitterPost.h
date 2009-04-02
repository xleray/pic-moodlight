/*********************************************************************
 *
 *  httpTwitterPost (based on GenericTCPClient.h)
 *
 *********************************************************************
 * FileName:        httpTwitterPost.h
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
#ifndef __HTTP_TWITTER_POST_H
#define __HTTP_TWITTER_POST_H

void sendbase64encodedData(TCP_SOCKET MySocket, BYTE *data);
void httpTwitterPost(BOOL runNow);

#endif
